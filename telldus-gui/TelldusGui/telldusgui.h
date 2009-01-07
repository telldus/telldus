#ifndef TELLDUSGUI_H
#define TELLDUSGUI_H

// The following ifdef block is the standard way of creating macros
// which make exporting from a DLL simpler.  All files within this DLL
// are compiled with the TELLDUSGUI_EXPORTS symbol defined on the command line.
// This symbol should not be defined on any project that uses this DLL.
// This way any other project whose source files include this file see
// TELLSTICK_API functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.

#ifdef _WINDOWS
	#ifdef TELLDUSGUI_EXPORTS
		#define TELLSTICK_API __declspec(dllexport)
	#else
		#define TELLSTICK_API __declspec(dllimport)
	#endif
	#define WINAPI __stdcall
#else
	#define WINAPI
	#define TELLSTICK_API
#endif

class QWidget;

#ifdef __cplusplus
extern "C" {
#endif
	TELLSTICK_API QWidget WINAPI *tdDeviceWidget( QWidget *parent = 0 );
#ifdef __cplusplus
}
#endif

#endif // TELLDUSGUI_H
