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
	ConnectionListener clientListener(L"TelldusClient");
	EVENT clientEvent = EventHandler::createEvent();
	//TODO: eventlistener

	clientListener.listen(clientEvent);
	//TODO: listen on eventListener

	EventHandler eventHandler;
	eventHandler.addEvent(clientEvent);
	while(running) {


		EVENT signaledEvent = eventHandler.waitForAny();
		if (signaledEvent == clientEvent) {
			//New client connection
			Socket *s = clientListener.retrieveClientSocket();
			std::wstring clientMessage = s->read();

			delete s;	//TODO: Cleanup
			clientListener.listen(clientEvent);
		}

		Sleep(1000);
	}
}

void TelldusMain::stop(void){
	running = false;
}
