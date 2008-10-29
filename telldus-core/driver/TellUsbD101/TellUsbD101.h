#ifndef TELLUSBD101_H
#define TELLUSBD101_H

// The following ifdef block is the standard way of creating macros
// which make exporting from a DLL simpler.  All files within this DLL
// are compiled with the TellUsbD101_EXPORTS symbol defined on the command line.
// This symbol should not be defined on any project that uses this DLL.
// This way any other project whose source files include this file see
// TELLUSBD101_API functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.

#ifdef TellUsbD101_EXPORTS
	#define TELLUSBD101_API __declspec(dllexport)
#else
	#define TELLUSBD101_API __declspec(dllimport)
#endif
#ifdef _WINDOWS
	#define WINAPI __stdcall
#else
	#define WINAPI
#endif


extern "C" {
	TELLUSBD101_API bool WINAPI devTurnOn(int intDeviceId);
	TELLUSBD101_API bool WINAPI devTurnOff(int intDeviceId);
	TELLUSBD101_API bool WINAPI devBell(int intDeviceId);
	TELLUSBD101_API bool WINAPI devDim(int intDeviceId, unsigned char level);
	TELLUSBD101_API int WINAPI devGetNumberOfDevices();
	TELLUSBD101_API char * WINAPI devGetName(int intDeviceId);
	TELLUSBD101_API bool WINAPI devSetName(int intDeviceId, const char* chNewName);
	TELLUSBD101_API char* WINAPI devGetVendor(int intDeviceId);
	TELLUSBD101_API bool WINAPI devSetVendor(int intDeviceId, const char* chNewName);
	TELLUSBD101_API char* WINAPI devGetModel(int intDeviceId);
	TELLUSBD101_API bool WINAPI devSetModel(int intDeviceId, const char* chNewName);
	TELLUSBD101_API int WINAPI devGetArgument(int intDeviceId, int intArgumentIndex);
	TELLUSBD101_API int WINAPI devGetNumberOfArguments(int intDeviceId);
	TELLUSBD101_API bool WINAPI devSetArguments(int intDeviceId, const char* strArguments);
	TELLUSBD101_API int WINAPI devAddDevice();
	TELLUSBD101_API bool WINAPI devRemoveDevice(int intDeviceId);
	TELLUSBD101_API int WINAPI devGetDeviceId(int intDeviceIndex);
	TELLUSBD101_API int WINAPI devMethods(int id);
}

#endif
