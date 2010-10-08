#include "ConnectionListener.h"

#include <windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>

#define BUFSIZE 512

class ConnectionListener::PrivateData {
public:
	std::wstring pipename;
	SECURITY_ATTRIBUTES sa;
	HANDLE hPipe;
	OVERLAPPED oOverlap;
};

ConnectionListener::ConnectionListener(const std::wstring &name)
{
	d = new PrivateData;

	d->pipename = L"\\\\.\\pipe\\" + name;
	

	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pACL = NULL;
	EXPLICIT_ACCESS ea;
	PSID pEveryoneSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;

	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH); 
	if (pSD == NULL) {
		return;
    } 
 
    if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) {  
        LocalFree(pSD);
		return;
    }

	if(!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID)) {
        LocalFree(pSD);
    }

	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	ea.grfAccessPermissions = STANDARD_RIGHTS_ALL;
	ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance= NO_INHERITANCE;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea.Trustee.ptstrName  = (LPTSTR) pEveryoneSID;

 
    // Add the ACL to the security descriptor. 
    if (!SetSecurityDescriptorDacl(pSD, 
            TRUE,     // bDaclPresent flag   
            pACL, 
            FALSE))   // not a default DACL 
    {  
        LocalFree(pSD);
		FreeSid(pEveryoneSID);
    }


	d->sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	d->sa.lpSecurityDescriptor = pSD;
	d->sa.bInheritHandle = false;

}

ConnectionListener::~ConnectionListener(void)
{
	delete d;
}

void ConnectionListener::listen(Event *waitEvent){
	
	d->oOverlap.hEvent = waitEvent->retrieveNative();
	
	d->hPipe = CreateNamedPipe( 
			(const wchar_t *)d->pipename.c_str(),             // pipe name 
			PIPE_ACCESS_DUPLEX |      // read/write access 
			FILE_FLAG_OVERLAPPED,	  //Overlapped mode
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			0,                        // client time-out 
			&d->sa);                    // default security attribute 

		if (d->hPipe == INVALID_HANDLE_VALUE) {
			//TelldusCore::logMessage("Could not create named pipe"); 
			return;
		}

		ConnectNamedPipe(d->hPipe, &d->oOverlap);
}

Socket *ConnectionListener::retrieveClientSocket(){
	DWORD cbBytesRead;

	bool connected = GetOverlappedResult(d->hPipe, &d->oOverlap, &cbBytesRead, false);
	if (!connected) {
		CloseHandle(d->hPipe);
		return 0;
	}
	Socket *s = new Socket(d->hPipe);
	return s;
	
}
