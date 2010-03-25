#ifndef TELLDUSCORESERVICE_H
#define TELLDUSCORESERVICE_H

#define TELLDUSCORE_STATIC
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
