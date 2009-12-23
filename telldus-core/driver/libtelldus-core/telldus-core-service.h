#ifndef TELLDUSCORESERVICE_H
#define TELLDUSCORESERVICE_H

#include "telldus-core.h"

#ifdef __cplusplus
extern "C" {
#endif
	TELLSTICK_API void WINAPI tdConnectTellStickController(int vid, int pid, const char *serial);
	TELLSTICK_API void WINAPI tdDisconnectTellStickController(int vid, int pid, const char *serial);
#ifdef __cplusplus
}
#endif

#endif //TELLDUSCORESERVICE_H
