#ifndef TELLUSBD101_H
#define TELLUSBD101_H

// The following ifdef block is the standard way of creating macros
// which make exporting from a DLL simpler.  All files within this DLL
// are compiled with the TellUsbD101_EXPORTS symbol defined on the command line.
// This symbol should not be defined on any project that uses this DLL.
// This way any other project whose source files include this file see
// TELLSTICK_API functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.

#ifdef _WINDOWS
	#ifdef TellUsbD101_EXPORTS
		#define TELLSTICK_API __declspec(dllexport)
	#else
		#define TELLSTICK_API __declspec(dllimport)
	#endif
	#define WINAPI __stdcall
#else
	#define WINAPI
	#define TELLSTICK_API
#endif


extern "C" {
	TELLSTICK_API bool WINAPI devTurnOn(int intDeviceId);
	TELLSTICK_API bool WINAPI devTurnOff(int intDeviceId);
	TELLSTICK_API bool WINAPI devBell(int intDeviceId);
	TELLSTICK_API bool WINAPI devDim(int intDeviceId, unsigned char level);
	TELLSTICK_API int WINAPI devGetNumberOfDevices();
	TELLSTICK_API char * WINAPI devGetName(int intDeviceId);
	TELLSTICK_API bool WINAPI devSetName(int intDeviceId, char* chNewName);
	TELLSTICK_API char* WINAPI devGetVendor(int intDeviceId);
	TELLSTICK_API bool WINAPI devSetVendor(int intDeviceId, char* chNewName);
	TELLSTICK_API char* WINAPI devGetModel(int intDeviceId);
	TELLSTICK_API bool WINAPI devSetModel(int intDeviceId, char* chNewName);
	TELLSTICK_API int WINAPI devGetArgument(int intDeviceId, int intArgumentIndex);
	TELLSTICK_API int WINAPI devGetNumberOfArguments(int intDeviceId);
	TELLSTICK_API bool WINAPI devSetArguments(int intDeviceId, char* strArguments);
	TELLSTICK_API int WINAPI devAddDevice();
	TELLSTICK_API int WINAPI devAddDeviceWithArguments(char* strVendor, int* intArguments[], int intNumberOfArguments);
	TELLSTICK_API bool WINAPI devRemoveDevice(int intDeviceId);
	TELLSTICK_API int WINAPI devGetDeviceId(int intDeviceIndex);
	TELLSTICK_API int WINAPI devMethods(int id);
}

#define TELLSTICK_TURNON	1
#define TELLSTICK_TURNOFF	2
#define TELLSTICK_BELL		4
#define TELLSTICK_TOGGLE	8
#define TELLSTICK_DIM		16

//Protocol Nexa
#define TELLSTICK_DEVICE_YCR3500	"1"
#define TELLSTICK_DEVICE_YCR300D	"2"
#define TELLSTICK_DEVICE_WSR1000	"3"
#define TELLSTICK_DEVICE_CMR1000	"4"
#define TELLSTICK_DEVICE_CMR300		"5"
#define TELLSTICK_DEVICE_PA33300	"6"
#define TELLSTICK_DEVICE_EL2000		"8"
#define TELLSTICK_DEVICE_EL2005		"9"
#define TELLSTICK_DEVICE_EL2006		"10"
#define TELLSTICK_DEVICE_SYCR3500	"12"
#define TELLSTICK_DEVICE_SYCR300	"13"
#define TELLSTICK_DEVICE_HDR105		"14"
#define TELLSTICK_DEVICE_ML7100		"15"
#define TELLSTICK_DEVICE_EL2004		"16"
#define TELLSTICK_DEVICE_EL2016		"17"
#define TELLSTICK_DEVICE_EL2010		"18"
#define TELLSTICK_DEVICE_LYCR1000	"20"
#define TELLSTICK_DEVICE_LYCR300	"21"
#define TELLSTICK_DEVICE_LCMR1000	"22"
#define TELLSTICK_DEVICE_LCMR300	"23"
#define TELLSTICK_DEVICE_EL2023		"24"
#define TELLSTICK_DEVICE_EL2024		"25"
#define TELLSTICK_DEVICE_EL2021		"26"
#define TELLSTICK_DEVICE_EL2017		"27"
#define TELLSTICK_DEVICE_EL2019		"28"

//Protocol Ikea
#define TELLSTICK_DEVICE_KOPPLA		"19"

#endif
