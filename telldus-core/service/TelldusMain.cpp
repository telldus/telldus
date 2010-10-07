#include "TelldusMain.h"
#include "ConnectionListener.h"

TelldusMain::TelldusMain(void)
{
	running = true;
}

TelldusMain::~TelldusMain(void)
{
}

void TelldusMain::start(void){
	ConnectionListener clientListener(L"TelldusClient");
	EVENT clientEvent = CreateEvent(NULL, true, true, NULL);//EventHandler::createEvent();
	//TODO: eventlistener

	clientListener.listen(clientEvent);
	//TODO: listen on eventListener

	//EventHandler eventHandler;
	while(running) {


//		int eventId = eventHandler.waitForAny();
		int result = WaitForSingleObject(clientEvent, 3000);
		if (result != WAIT_TIMEOUT) {
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
