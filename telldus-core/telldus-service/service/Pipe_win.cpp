#include "Pipe.h"
#include "Socket.h"
#include "TelldusCore.h"

#include <windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>

//using namespace TelldusService;

class PipePrivate {
public:
	QString name;
	HANDLE event;
	bool closing;
};

Pipe::Pipe(QObject *parent)
	:QThread(parent)
{
	d = new PipePrivate;
	d->event = INVALID_HANDLE_VALUE;
	d->closing = false;
}

Pipe::~Pipe(void) {
	TelldusCore::logMessage(QString("Stopping pipe %1").arg(d->name));
	d->closing = true;
	if (d->event != INVALID_HANDLE_VALUE) {
		SetEvent(d->event);
	}
	TelldusCore::logMessage("Waiting for thread to close");
	this->wait();
	delete d;
}

void Pipe::listen(const QString &name) {
	d->name = QString("\\\\.\\pipe\\%1").arg(name);
	this->start();	
}

void Pipe::run() {
	SECURITY_ATTRIBUTES sa;
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pACL = NULL;
	EXPLICIT_ACCESS ea;
	PSID pEveryoneSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	DWORD cbBytesRead;

	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH); 
	if (pSD == NULL) {
		TelldusCore::logMessage("LocalAlloc Error");
		return;
    } 
 
    if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) {  
		TelldusCore::logMessage("InitializeSecurityDescriptor Error");
        LocalFree(pSD);
		return;
    }

	if(!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID)) {
		TelldusCore::logMessage("AllocateAndInitializeSid Error");
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
		TelldusCore::logMessage("SetSecurityDescriptorDacl Error");
        LocalFree(pSD);
		FreeSid(pEveryoneSID);
    }


	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = false;

	OVERLAPPED oOverlap;
	oOverlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	d->event = oOverlap.hEvent;

	while(!d->closing) {
		TelldusCore::logMessage(QString("Starting listening in pipe %1").arg(d->name));
		HANDLE hPipe = CreateNamedPipe( 
			(const wchar_t *)d->name.utf16(),             // pipe name 
			PIPE_ACCESS_DUPLEX |      // read/write access 
			FILE_FLAG_OVERLAPPED,	  //Overlapped mode
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			0,                        // client time-out 
			&sa);                    // default security attribute 

		if (hPipe == INVALID_HANDLE_VALUE) {
			TelldusCore::logMessage("Could not create named pipe"); 
			return;
		}

		ConnectNamedPipe(hPipe, &oOverlap);
		int result = WaitForSingleObject(oOverlap.hEvent, INFINITE);
		if (result == WAIT_TIMEOUT) {
			continue;
		}
		bool connected = GetOverlappedResult(hPipe, &oOverlap, &cbBytesRead, false);
		if (d->closing || !connected) {
			CloseHandle(hPipe);
			TelldusCore::logMessage("Closing or no connection, restart");
			continue;
		}
		TelldusCore::logMessage("Connected");
		emit newConnection(new Socket(hPipe));
	}

	CloseHandle(oOverlap.hEvent);
	d->event = INVALID_HANDLE_VALUE;

	LocalFree(pSD);
	FreeSid(pEveryoneSID);
	LocalFree(pACL);
}