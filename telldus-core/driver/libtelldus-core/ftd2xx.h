
#if defined(_LINUX ) || defined(__FreeBSD__)
	#define LIBFTDI
#else
	#define LIBFTD2XX
#endif


#ifdef LIBFTD2XX
	#ifdef _WINDOWS
		#include "stdafx.h"
		#include "win\ftd2xx.h"
	#else
		#include "osx/WinTypes.h"
		#include "osx/ftd2xx.h"
	#endif
#endif

#ifdef LIBFTDI
	#include <ftdi.h>
#endif
