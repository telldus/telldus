//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifdef _WINDOWS
#include <windows.h>
#include <ole2.h>
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#include <unistd.h>
#endif
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>

inline void msleep( const int msec) {
#ifdef _WINDOWS
	Sleep(msec);
#else
	usleep(msec*1000);
#endif
}

inline void debuglog(const int intMessage, const std::string strMessage){
		
    static bool firstRun = true;
    std::ofstream file;
    std::string filename("C:/log_locks.txt");
    if (firstRun) {
            file.open(filename.c_str(), std::ios::out);
            firstRun = false;
    } else {
            file.open(filename.c_str(), std::ios::out | std::ios::app);
    }
   
    file << intMessage << " - " << strMessage << "\n";
    file.flush();
    file.close();

}

inline char *wrapStdString( const std::string &string) {
#ifdef _WINDOWS
	return (char *)SysAllocStringByteLen(string.c_str(), (unsigned int)string.size());
#else
	char *returnVal = (char *)malloc(sizeof(char) * (string.size()+1));
	strcpy(returnVal, string.c_str());
	return returnVal;
#endif
}

inline char *wrapStdWstring( const std::wstring &wstring) {
	return wrapStdString(std::string(wstring.begin(), wstring.end()));	//TODO
}