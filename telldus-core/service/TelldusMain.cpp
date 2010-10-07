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
	EVENT clientEvent = eventHandler.addEvent();
	
	ConnectionListener clientListener(L"TelldusClient");
	//TODO: eventlistener

	clientListener.listen(clientEvent);
	//TODO: listen on eventListener

	while(running) {


		EVENT signaledEvent = eventHandler.waitForAny();
		if (signaledEvent == clientEvent) {
			//New client connection
			Socket *s = clientListener.retrieveClientSocket();
			std::wstring clientMessage = s->read();

			delete s;	//TODO: Cleanup
			clientListener.listen(clientEvent);
		}
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
