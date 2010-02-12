//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifdef _WINDOWS
#include "stdafx.h"
#include <ole2.h>
#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#endif
#include <stdlib.h>
#include <string.h>

inline void msleep( const int msec) {
#ifdef _WINDOWS
	Sleep(msec);
#else
	usleep(msec*1000);
#endif
}

inline char *wrapStdString( const std::string &string) {
	char *returnVal;
#ifdef _WINDOWS
	returnVal = (char *)SysAllocStringByteLen(string.c_str(), string.size());
#else
	returnVal = (char *)malloc(sizeof(char) * (string.size()+1));
	strcpy(returnVal, string.c_str());
#endif
	return returnVal;
}
