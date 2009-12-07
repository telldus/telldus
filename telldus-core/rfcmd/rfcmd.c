/****************************************************************************
 ** rfcmd.c ***********************************************************
 ****************************************************************************
 *
 * rfcmd - utility to control NEXA and other RF remote receivers through a TellStick
 * USB interface
 *
 * Copyright (C) 2007 Tord Andersson <tord.o.andersson@gmail.com>
 *
 * License: GPL v. 2
 *
 * Authors:
 *   Tord Andersson <tord.o.andersson@gmail.com>
 *   Micke Prag <micke.prag@telldus.se>
 *   Gudmund Berggren
 *
 *   Tapani Rintala / userspace libusb / libftdi - version 02-2009
 */

/*******************************************************************************
 * Modifications from rfcmd.c ver 0.2 done by Gudmund
 *  Added support for IKEA
 * Note:
 * 1. System code
 * 2. Level 0 == Off (For dimmers and non dimmers)
 *    Level 1== 10%. (for dimmers)
 *     ....
 *    Level 9 == 90 % (for dimmers)
 *    Level 10 == 100 %  (or ON for non dimmers)
 * 3. Command line syntax:
 *    /usr/local/bin/rfcmd  /dev/ttyUSB0  IKEA 0 1 10 1"
 *    Arg 1: device
 *    Arg 2: Protocoll
 *    Arg 3: System code
 *    Arg 4: Device code
 *    Arg 5: Level (0=off, 1 = 10%, 2=20%,...9=90%, 10=100%)
 *    Arg 6: DimStyle (0=instant change, 1=gradually change)
 ******************************************************************************/

/*******************************************************************************
 * Modifications to rfcmd.c ver 2.1.0 done by Tord Andersson
 *  Introduced semaphore protection to avoid problems with simultaneous port 
 *  access from several processes (typically cron jobs). Note! Need rt lib. 
 ******************************************************************************/

/*******************************************************************************
 * Modifications from rfcmd.c ver 2.1.0 done by Leo
 *  Added support for RISINGSUN
 * Note:
 * 1. Command line syntax:
 *    /usr/local/bin/rfcmd  /dev/ttyUSB0  RISINGSUN 4 1 0" 
 *    Arg 1: device
 *    Arg 2: protocol
 *    Arg 3: code 
 *    Arg 4: device number
 *    Arg 5: Level (0=off, 1 = on)
 ******************************************************************************/

/*******************************************************************************
 * Modifications from rfcmd ver 2.1.1 done by Johan Ström
 *  Default disabled semaphores for FreeBSD.
 *  Added status readback in ftdi.c, instead of wasting time in sleep.
 *
 * FreeBSD does not have support in the GENERIC kernel semaphore.
 * To enable usage of them, you'll have have the following option in your
 * kernel configuration:
 *
 *   options P1003_1B_SEMAPHORES
 *
 * However, on FreeBSD only libftdi seems to be working (at least on my system),
 * since the device is not identified as a ucom device. And as we're accessing it
 * via libftdi, libftdi makes sure simultaneous access is impossible.
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#ifndef NO_SEMAPHORES
#include <semaphore.h>
#endif

#define PROG_NAME "rfcmd"
#define PROG_VERSION "2.1.1"
/* #define RFCMD_DEBUG */ 

/* Local function declarations */
int createNexaString(const char * pHouseStr, const char * pChannelStr,
                     const char * pOn_offStr, char * pTxStr, int waveman);
int createSartanoString(const char * pChannelStr, const char * pOn_offStr,
                        char * pTxStr);
int createIkeaString(const char * pSystemStr, const char * pChannelStr,
                     const char * pLevelStr, const char *pDimStyle,
                     char * pStrReturn);
int createRisingSunString(const char * pCodeStr, const char* pUnitStr, const char * pOn_offStr,
                        char * pTxStr);

void printUsage(void);
void printVersion(void);

#ifdef LIBFTDI
int usbWriteFtdi(char *cmdstr);
#endif

int main( int argc, char **argv )
{
	struct termios tio;
	int fd = -1;
#ifndef NO_SEMAPHORES
	sem_t * portMutex;
	char SEM_NAME[]= "RFCMD_SEM"; /* Semaphore for multiple access ctrl */
#endif


	char txStr[100];

	if( (argc == 6) && (strcmp(*(argv+2), "NEXA") == 0)) {
		if (createNexaString(*(argv+3), *(argv+4), *(argv+5), txStr, 0) == 0) {
			printUsage();
			exit(1);
		}
		/* else - a send cmd string was created */
	} else if( (argc == 6) && (strcmp(*(argv+2), "WAVEMAN") == 0)) {
		if (createNexaString(*(argv+3),*(argv+4), *(argv+5), txStr, 1) == 0) {
			printUsage();
			exit(1);
		}
	} else if( (argc == 5) && (strcmp(*(argv+2), "SARTANO") == 0)) {
		if (createSartanoString(*(argv+3), *(argv+4), txStr) == 0) {
			printUsage();
			exit(1);
		}
		/* else - a send cmd string was created */
	} else if ( (argc == 7) && (strcmp(*(argv+2),"IKEA")==0) ) {
		//                    System,   Channel,    Level,    DimStyle, TXString
		if ( createIkeaString(*(argv+3), *(argv+4), *(argv+5), *(argv+6),txStr) == 0 ) {
			printUsage();
			exit(1);
		}
		/* else - a send cmd string was created */
	} else if ( (argc == 6) && (strcmp(*(argv+2),"RISINGSUN")==0) ) {
	//                    Code,   Unit,    Power
		if ( createRisingSunString(*(argv+3), *(argv+4), *(argv+5), txStr) == 0 ) {
			printUsage();
			exit(1);
			}
			/* else - a send cmd string was created */
	} else if ( (argc >= 2) && (strcmp(*(argv+1),"--version")==0) ) {
		printVersion();
		exit(1);
	} else { /* protocol or parameters not recognized */
		printUsage();
		exit(1);
	}

#ifdef RFCMD_DEBUG
	printf("txStr: %s\n", txStr);

#endif

	if(strlen(txStr) > 0) {
#ifndef NO_SEMAPHORES
		/* create the semaphore - will reuse an existing one if it exists */
		portMutex = sem_open(SEM_NAME,O_CREAT,0644,1);
		if( portMutex == SEM_FAILED) {
			fprintf(stderr,  "%s - Error creating port semaphore\n", PROG_NAME);
			perror("Semaphore open error");
			sem_unlink(SEM_NAME);
			exit(1);
		}

		/* lock semaphore to protect port from multiple access */
		if(sem_wait(portMutex) != 0) {
			fprintf(stderr,  "%s - Error aquiring port semaphore\n", PROG_NAME);
			sem_unlink(SEM_NAME);
			sem_close(portMutex);
			exit(1); 
		}
#endif


		if (strcmp(*(argv+1), "LIBUSB") != 0) {
			if( 0 > ( fd = open( *(argv+1), O_RDWR ) ) ) {
#ifdef __FreeBSD__
				fprintf(stderr,  "%s - Error opening %s; You're on a FreeBSD system, you should probably use LIBUSB.\n", PROG_NAME, *(argv+1));
#else
				fprintf(stderr,  "%s - Error opening %s\n", PROG_NAME, *(argv+1));
#endif
#ifndef NO_SEMAPHORES
				if(sem_post(portMutex) != 0) {
					fprintf(stderr,  "%s - Error releasing port semaphore\n", PROG_NAME);
				}
				sem_unlink(SEM_NAME);
				sem_close(portMutex);
#endif
				exit(1);
			}

			/* adjust serial port parameters */
			bzero(&tio, sizeof(tio)); /* clear struct for new port settings */
			tio.c_cflag = B4800 | CS8 | CLOCAL | CREAD; /* CREAD not used yet */
			tio.c_iflag = IGNPAR;
			tio.c_oflag = 0;
			tio.c_ispeed = 4800;
			tio.c_ospeed = 4800;
			tcflush(fd, TCIFLUSH);
			tcsetattr(fd,TCSANOW,&tio);

			write(fd, txStr, strlen(txStr));

			sleep(1); /* one second sleep to avoid		device 'choking' */
			close(fd); /* Modified : Close fd to make a clean exit */
		} else {
#ifdef LIBFTDI
			usbWriteFtdi( txStr );
#else
			fprintf(stderr,  "%s - Support for libftdi is not compiled in, please recompile rfcmd with support for libftdi\n", PROG_NAME);
#endif
		}
#ifndef NO_SEMAPHORES
		/* Unlock semaphore */
		if (sem_post(portMutex) != 0) {
			fprintf(stderr,  "%s - Error releasing port semaphore\n", PROG_NAME);
			sem_unlink(SEM_NAME);
			sem_close(portMutex);
			exit(1);
		} else {
			sem_unlink(SEM_NAME);
			sem_close(portMutex);	
		}
#endif
	}
  exit(0);
}


int createNexaString(const char * pHouseStr, const char * pChannelStr,
                     const char * pOn_offStr, char * pTxStr, int waveman)
{
	* pTxStr = '\0'; /* Make sure tx string is empty */
	int houseCode;
	int channelCode;
	int on_offCode;
	int txCode = 0;
	const int unknownCode =  0x6;
	int bit;
	int bitmask = 0x0001;

	houseCode = (int)((* pHouseStr) - 65);	/* House 'A'..'P' */
	channelCode = atoi(pChannelStr) - 1;	/* Channel 1..16 */
	on_offCode =  atoi(pOn_offStr);	        /* ON/OFF 0..1 */

#ifdef RFCMD_DEBUG
	printf("House: %d, channel: %d, on_off: %d\n", houseCode, channelCode, on_offCode);
#endif

	/* check converted parameters for validity */
	if((houseCode < 0) || (houseCode > 15) ||      // House 'A'..'P'
	   (channelCode < 0) || (channelCode > 15) ||
	   (on_offCode < 0) || (on_offCode > 1))
	{

	} else {
		/* b0..b11 txCode where 'X' will be represented by 1 for simplicity.
		   b0 will be sent first */
		txCode = houseCode;
		txCode |= (channelCode <<  4);
		if (waveman && on_offCode == 0) {
		} else {
			txCode |= (unknownCode <<  8);
			txCode |= (on_offCode  << 11);
		}

		/* convert to send cmd string */
		strcat(pTxStr,"S");
		for(bit=0;bit<12;bit++)
		{
			if((bitmask & txCode) == 0) {
				/* bit timing might need further refinement */
				strcat(pTxStr," ` `"); /* 320 us high, 960 us low,  320 us high, 960 us low */
				/* strcat(pTxStr,"$k$k"); *//* 360 us high, 1070 us low,  360 us high, 1070 us low */
			} else { /* add 'X' (floating bit) */
				strcat(pTxStr," `` "); /* 320 us high, 960 us low, 960 us high,  320 us low */
				/*strcat(pTxStr,"$kk$"); *//* 360 us high, 1070 us low, 1070 us high,  360 us low */
			}
			bitmask = bitmask<<1;
		}
		/* add stop/sync bit and command termination char '+'*/
		strcat(pTxStr," }+");
		/* strcat(pTxStr,"$}+"); */
	}

#ifdef RFCMD_DEBUG
	printf("txCode: %04X\n", txCode);
#endif

	return strlen(pTxStr);
}

int createSartanoString(const char * pChannelStr, const char * pOn_offStr,
                        char * pTxStr)
{
	* pTxStr = '\0'; /* Make sure tx string is empty */
	int on_offCode;
	int bit;

	on_offCode =  atoi(pOn_offStr);	        /* ON/OFF 0..1 */

#ifdef RFCMD_DEBUG
	printf("Channel: %s, on_off: %d\n", pChannelStr, on_offCode);
#endif

	/* check converted parameters for validity */
	if((strlen(pChannelStr) != 10) ||
	   (on_offCode < 0) || (on_offCode > 1)) {
	} else {
		strcat(pTxStr,"S");
		for(bit=0;bit<=9;bit++)
		{
			if(strncmp(pChannelStr+bit, "1", 1) == 0) { //If it is a "1"
				strcat(pTxStr,"$k$k");
			} else {
				strcat(pTxStr,"$kk$");
	    }
		}
		if (on_offCode >= 1)
			strcat(pTxStr,"$k$k$kk$"); //the "turn on"-code
		else
			strcat(pTxStr,"$kk$$k$k"); //the "turn off"-code

		/* add stop/sync bit and command termination char '+'*/
		strcat(pTxStr,"$k+");
	}

	return strlen(pTxStr);
}

int createIkeaString( const char * pSystemStr, const char * pChannelStr, const char * pLevelStr, const char *pDimStyle, char * pStrReturn)
{
	*pStrReturn = '\0'; /* Make sure tx string is empty */

	const char STARTCODE[] = "STTTTTTª";
	const char TT[]  = "TT";
	const char A[]   = "ª";
	int systemCode   = atoi(pSystemStr) - 1;   /* System 1..16 */
	int channelCode  = atoi(pChannelStr);  /* Channel 1..10 */
	int Level        = atoi(pLevelStr);     /* off,10,20,..,90,on */
	int DimStyle     = atoi(pDimStyle);
	int intCode      = 0;
	int checksum1    = 0;
	int checksum2    = 0;
	int intFade ;
	int i ;
	int rawChannelCode = 0;

	/* check converted parameters for validity */
	if ( (channelCode <= 0) || (channelCode > 10) ||
	     (systemCode < 0) || (systemCode > 15) ||
	     (Level < 0) || (Level > 10) ||
	     (DimStyle < 0) || (DimStyle > 1))
	{
		return 0;
	}

	if (channelCode == 10) {
		channelCode = 0;
	}
	rawChannelCode = (1<<(9-channelCode));

	strcat(pStrReturn, STARTCODE ) ; //Startcode, always like this;
	intCode = (systemCode << 10) | rawChannelCode;

	for ( i = 13; i >= 0; --i) {
		if ((intCode>>i) & 1) {
			strcat(pStrReturn, TT );
			if (i % 2 == 0) {
				checksum2++;
			} else {
				checksum1++;
			}
		} else {
			strcat(pStrReturn,A);
		}
	}

	if (checksum1 %2 == 0) {
		strcat(pStrReturn, TT );
	} else {
		strcat(pStrReturn, A) ; //1st checksum
	}

	if (checksum2 %2 == 0) {
		strcat(pStrReturn, TT );
	} else {
		strcat(pStrReturn, A ) ; //2nd checksum
	}

	if (DimStyle == 1) {
		intFade = 11 << 4; //Smooth
	} else {
		intFade = 1 << 4; //Instant
	}

	switch ( Level )
	{
		case 0 :
			intCode = (10 | intFade) ; //Concat level and fade
			break;
		case 1 :
			intCode = (1 | intFade) ; //Concat level and fade
			break;
		case 2 :
			intCode = (2 | intFade) ; //Concat level and fade
			break;
		case 3 :
			intCode = (3 | intFade) ; //Concat level and fade
			break;
		case 4 :
			intCode = (4 | intFade) ; //Concat level and fade
			break;
		case 5 :
			intCode = (5 | intFade) ; //Concat level and fade
			break;
		case 6 :
			intCode = (6 | intFade) ; //Concat level and fade
			break;
		case 7 :
			intCode = (7 | intFade) ; //Concat level and fade
			break;
		case 8 :
			intCode = (8 | intFade) ; //Concat level and fade
			break;
		case 9 :
			intCode = (9 | intFade) ; //Concat level and fade
			break;
		case 10 :
		default :
			intCode = (0 | intFade) ; //Concat level and fade
			break;
	}

	checksum1 = 0;
	checksum2 = 0;

	for (i = 0; i < 6; ++i) {
		if ((intCode>>i) & 1) {
			strcat(pStrReturn, TT);

			if (i % 2 == 0) {
				checksum1++;
			} else {
				checksum2++;
			}
		} else {
			strcat(pStrReturn, A );
		}
	}

	if (checksum1 %2 == 0) {
		strcat(pStrReturn, TT);
	} else {
		strcat(pStrReturn, A ) ; //2nd checksum
	}

	if (checksum2 %2 == 0) {
		strcat(pStrReturn, TT );
	} else {
		strcat(pStrReturn, A ) ; //2nd checksum
	}

	strcat(pStrReturn, "+");

	return strlen(pStrReturn);
}

int createRisingSunString(const char * pCodeStr, const char * pUnitStr, const char * pOn_offStr,
                        char * pTxStr)
{
	* pTxStr = '\0'; /* Make sure tx string is empty */
	int on_offCode;
	int unit;
	int code;
	int i;

	on_offCode =  atoi(pOn_offStr);	        /* ON/OFF 0..1 */
	code = atoi (pCodeStr);
	unit = atoi (pUnitStr);

#ifdef RFCMD_DEBUG
	printf("Code: %s, unit: %s, on_off: %d\n", pCodeStr, pUnitStr, on_offCode);
#endif

	/* check converted parameters for validity */
	if((code < 1) || (code > 4) ||
	   (unit < 1) || (unit > 4) ||
	   (on_offCode < 0) || (on_offCode > 1)) {
	} else {
		strcat(pTxStr,"S.e");
		for (i = 1; i <= 4; ++i) {
			if (i == code) {
				strcat (pTxStr, ".e.e");
			} else {
				strcat (pTxStr, "e..e");
			}
		}
		for (i = 1; i <= 4; ++i) {
			if (i == unit) {
				strcat (pTxStr, ".e.e");
			} else {
				strcat (pTxStr, "e..e");
			}
		}

		if (on_offCode >= 1) {
			strcat(pTxStr,"e..ee..ee..ee..e+"); //the "turn on"-code
		} else {
			strcat(pTxStr,"e..ee..ee..e.e.e+"); //the "turn off"-code
		}
	}

	return strlen(pTxStr);
}


void printUsage(void)
{
	printf("Usage: rfcmd DEVICE PROTOCOL [PROTOCOL_ARGUMENTS] \n");
	printf("\n");
#ifdef LIBFTDI
	printf("\t DEVICE: /dev/ttyUSB[0..n] | LIBUSB\n" );
#else
	printf("\t DEVICE: /dev/ttyUSB[0..n]\n" );
#endif
	printf("\t PROTOCOLS: NEXA, SARTANO, WAVEMAN, IKEA, RISINGSUN\n" );
	printf("\n");
	printf("\t PROTOCOL ARGUMENTS - NEXA, WAVEMAN:\n");
	printf("\t\tHOUSE_CODE: A..P\n\t\tCHANNEL: 1..16\n\t\tOFF_ON: 0..1\n" );
	printf("\n");
	printf("\t PROTOCOL ARGUMENTS - SARTANO:\n");
	printf("\t\tCHANNEL: 0000000000..1111111111\n\t\tOFF_ON: 0..1\n" );
	printf("\n");
	printf("\t PROTOCOL ARGUMENTS - IKEA:\n");
	printf("\t\tSYSTEM: 1..16\n\t\tDEVICE: 1..10\n");
	printf("\t\tDIM_LEVEL: 0..10\n\t\tDIM_STYLE: 0..1\n" );
	printf("\n");
	printf("\t PROTOCOL ARGUMENTS - RISINGSUN:\n");
	printf("\t\tCODE: 1..4\n\t\tDEVICE: 1..4\n");
	printf("\t\tOFF_ON: 0..1\n" );
	printf("\n");
	printf("Report bugs to <info.tech@telldus.se>\n");
}

void printVersion(void) {
	printf("%s v%s\n", PROG_NAME, PROG_VERSION);
	printf("\n");
	printf("Copyright (C) Tord Andersson 2007\n");
	printf("\n");
	printf("Written by:\n");
	printf("Tord Andersson, Micke Prag, Gudmund Berggren, Tapani Rintala\n");
	printf("and Johan Ström\n");
}

