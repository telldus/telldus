#ifdef _WINDOWS
	#include "stdafx.h"
	#include "win\ftd2xx.h"
#else
	#include "osx/WinTypes.h"
	#include "osx/ftd2xx.h"
#endif

#ifdef _LINUX
#include <ftdi.h>
#endif
