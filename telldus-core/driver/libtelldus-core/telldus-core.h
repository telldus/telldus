#ifndef TELLDUSCORE_H
#define TELLDUSCORE_H

// The following ifdef block is the standard way of creating macros
// which make exporting from a DLL simpler.  All files within this DLL
// are compiled with the TELLDUSCORE_EXPORTS symbol defined on the command line.
// This symbol should not be defined on any project that uses this DLL.
// This way any other project whose source files include this file see
// TELLSTICK_API functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.

#ifdef _WINDOWS
	#ifdef TELLDUSCORE_EXPORTS
		#define TELLSTICK_API __declspec(dllexport)
	#else
		#define TELLSTICK_API __declspec(dllimport)
	#endif
	#define WINAPI __stdcall
#else
	#define WINAPI
	#define TELLSTICK_API
#endif

#ifdef __cplusplus
extern "C" {
#endif
	TELLSTICK_API int WINAPI tdTurnOn(int intDeviceId);
	TELLSTICK_API int WINAPI tdTurnOff(int intDeviceId);
	TELLSTICK_API int WINAPI tdBell(int intDeviceId);
	TELLSTICK_API int WINAPI tdDim(int intDeviceId, unsigned char level);
	TELLSTICK_API int WINAPI tdMethods(int id);
	
	TELLSTICK_API int WINAPI tdGetNumberOfDevices();
	TELLSTICK_API int WINAPI tdGetDeviceId(int intDeviceIndex);
	
	TELLSTICK_API char * WINAPI tdGetErrorString(int intErrorNo);
	
	TELLSTICK_API char * WINAPI tdGetName(int intDeviceId);
	TELLSTICK_API bool WINAPI tdSetName(int intDeviceId, const char* chNewName);
	TELLSTICK_API char * WINAPI tdGetProtocol(int intDeviceId);
	TELLSTICK_API bool WINAPI tdSetProtocol(int intDeviceId, const char* strProtocol);
	TELLSTICK_API int WINAPI tdGetModel(int intDeviceId);
	TELLSTICK_API bool WINAPI tdSetModel(int intDeviceId, int intModel);
	
	TELLSTICK_API char * WINAPI tdGetArgument(int intDeviceId, const char *strName, const char *defaultValue);
	TELLSTICK_API bool WINAPI tdSetArgument(int intDeviceId, const char *strName, const char* strValue);
	
	TELLSTICK_API int WINAPI tdAddDevice();
	TELLSTICK_API bool WINAPI tdRemoveDevice(int intDeviceId);
#ifdef __cplusplus
}
#endif

//Device methods
#define TELLSTICK_TURNON	1
#define TELLSTICK_TURNOFF	2
#define TELLSTICK_BELL		4
#define TELLSTICK_TOGGLE	8
#define TELLSTICK_DIM		16

//Error codes
#define TELLSTICK_SUCCESS 0
#define TELLSTICK_ERROR_NOT_FOUND -1
#define TELLSTICK_ERROR_PERMISSION_DENIED -2
#define TELLSTICK_ERROR_DEVICE_NOT_FOUND -3
#define TELLSTICK_ERROR_METHOD_NOT_SUPPORTED -4
#define TELLSTICK_ERROR_UNKNOWN -99

//Protocol Nexa
#define TELLSTICK_DEVICE_YCR3500	1
#define TELLSTICK_DEVICE_YCR300D	2
#define TELLSTICK_DEVICE_WSR1000	3
#define TELLSTICK_DEVICE_CMR1000	4
#define TELLSTICK_DEVICE_CMR300		5
#define TELLSTICK_DEVICE_PA33300	6
#define TELLSTICK_DEVICE_EL2000		8
#define TELLSTICK_DEVICE_EL2005		9
#define TELLSTICK_DEVICE_EL2006		10
#define TELLSTICK_DEVICE_SYCR3500	12
#define TELLSTICK_DEVICE_SYCR300	13
#define TELLSTICK_DEVICE_HDR105		14
#define TELLSTICK_DEVICE_ML7100		15
#define TELLSTICK_DEVICE_EL2004		16
#define TELLSTICK_DEVICE_EL2016		17
#define TELLSTICK_DEVICE_EL2010		18
#define TELLSTICK_DEVICE_LYCR1000	20
#define TELLSTICK_DEVICE_LYCR300	21
#define TELLSTICK_DEVICE_LCMR1000	22
#define TELLSTICK_DEVICE_LCMR300	23
#define TELLSTICK_DEVICE_EL2023		24
#define TELLSTICK_DEVICE_EL2024		25
#define TELLSTICK_DEVICE_EL2021		26
#define TELLSTICK_DEVICE_EL2017		27
#define TELLSTICK_DEVICE_EL2019		28

//Protocol Ikea
#define TELLSTICK_DEVICE_KOPPLA		19

#endif
