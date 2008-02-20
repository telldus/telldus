/****************************************************************************
 ** rfcmd.c ***********************************************************
 ****************************************************************************
 *
 * rfcmd - utility to control NEXA and other RF remote receivers through a TellStick 
   USB interface
 *
 * Copyright (C) 2007 Tord Andersson <tord.o.andersson@gmail.com>
 *
 * License: GPL v. 2
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define PROG_NAME "rfcmd"
#define PROG_VERSION "0.2"
/* #define RFCMD_DEBUG */

/* Local function declarations */
int createNexaString(const char * pHouseStr, const char * pChannelStr, 
	const char * pOn_offStr, char * pTxStr, int waveman);
int createSartanoString(const char * pChannelStr, const char * pOn_offStr,	
	char * pTxStr);

void printUsage(void);

int main( int argc, char **argv )
{
	struct termios tio;
	int fd = -1;
	char txStr[250];



	if( (argc == 6) && (strcmp(*(argv+2), "NEXA") == 0))
	{

		if (createNexaString(*(argv+3), *(argv+4), *(argv+5), txStr, 0) == 0)
		{
			printUsage();
			exit(1);
		}
		/* else - a send cmd string was created */
	}
	else if( (argc == 6) && (strcmp(*(argv+2), "WAVEMAN") == 0))
	{
		if (createNexaString(*(argv+3),*(argv+4), *(argv+5), txStr, 1) == 0)
		{
			printUsage();
			exit(1);
		}
	}
	else if( (argc == 5) && (strcmp(*(argv+2), "SARTANO") == 0))
	{

		if (createSartanoString(*(argv+3), *(argv+4), txStr) == 0)
		{
			printUsage();
			exit(1);
		}
		/* else - a send cmd string was created */
	}
	else /* protocol or parameters not recognized */
	{
		printUsage();
		exit(1);
	}


#ifdef RFCMD_DEBUG
	printf("txStr: %s\n", txStr);
#endif

	if( 0 > ( fd = open( *(argv+1), O_RDWR ) ) )
	{
		fprintf(stderr,  "%s - Error opening %s\n", PROG_NAME, *(argv+1));
		exit(1);
	}


	if(strlen(txStr) > 0)
	{
		/* adjust serial port parameters */
		bzero(&tio, sizeof(tio)); /* clear struct for new port settings */
         	tio.c_cflag = B4800 | CS8 | CLOCAL | CREAD; /* CREAD not used yet */
		tio.c_iflag = IGNPAR;
		tio.c_oflag = 0;
		tcflush(fd, TCIFLUSH);
		tcsetattr(fd,TCSANOW,&tio);

		write(fd, txStr, strlen(txStr));
		sleep(1); /* one second sleep to avoid device 'choking' */
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

	}
	else
	{
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
			if((bitmask & txCode) == 0)
			{
				/* bit timing might need further refinement */
				strcat(pTxStr," ` `"); /* 320 us high, 960 us low,  320 us high, 960 us low */
				/* strcat(pTxStr,"$k$k"); *//* 360 us high, 1070 us low,  360 us high, 1070 us low */
			}
			else /* add 'X' (floating bit) */
			{
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
		(on_offCode < 0) || (on_offCode > 1))
	{

	}
	else
	{
		strcat(pTxStr,"S");	
		for(bit=0;bit<=9;bit++)
		{
			if(strncmp(pChannelStr+bit, "1", 1) == 0) //If it is a "1"
			{
				strcat(pTxStr,"$k$k");
			}
			else
			{
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

#ifdef RFCMD_DEBUG
	printf("txCode: %04X\n", txCode);
#endif

	return strlen(pTxStr);
	
}

void printUsage(void)
{
	printf("%s %s - Send RF remote commands\n", PROG_NAME, PROG_VERSION);
	printf("Usage: rfcmd DEVICE PROTOCOL [PROTOCOL_ARGUMENTS] \n");
	printf("\t DEVICE: /dev/ttyUSB[0..n]\n" );
	printf("\t PROTOCOLS: NEXA, SARTANO, WAVEMAN\n" );
	printf("\t PROTOCOL ARGUMENTS - NEXA, WAVEMAN: \n");
	printf("\t\t HOUSE_CODE: A..P\n\t\t CHANNEL: 1..16\n\t\t ON_OFF: 0..1\n" );
	printf("\t PROTOCOL ARGUMENTS - SARTANO: \n");
	printf("\t\t CHANNEL: 0000000000..1111111111\n\t\t ON_OFF: 0..1\n" );
	printf("Copyright(C) Tord Andersson 2007\r\n");
}
