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


		int eventId = eventHandler.waitForAny();
		if (eventId) {
//		if (eventId == clientEvent.id()) {
			//New client connection
			Socket *s = clientListener.retrieveClientSocket();
			s->read();
			delete s;
			clientListener.listen(clientEvent);
		}

		Sleep(1000);
	}
}

void TelldusMain::stop(void){
	running = false;
}
