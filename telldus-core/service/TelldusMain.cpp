#include "TelldusMain.h"
#include "ConnectionListener.h"
#include "EventHandler.h"

TelldusMain::TelldusMain(void)
{
	running = true;
}

TelldusMain::~TelldusMain(void)
{
}

void TelldusMain::start(void){
	EventHandler eventHandler;
	Event *clientEvent = eventHandler.addEvent();
	
	ConnectionListener clientListener(L"TelldusClient");
	//TODO: eventlistener

	clientListener.listen(clientEvent);
	//TODO: listen on eventListener

	while(running) {
		if (!eventHandler.waitForAny()) {
			continue;
		}
		//if (signaledEvent == clientEvent) {
			//New client connection
			Socket *s = clientListener.retrieveClientSocket();
			std::wstring clientMessage = s->read();

			delete s;	//TODO: Cleanup
			clientListener.listen(clientEvent);
		//}
#ifdef _WINDOWS
		Sleep(1000);
#else
		sleep(1);
#endif
	}
}

void TelldusMain::stop(void){
	running = false;
}
