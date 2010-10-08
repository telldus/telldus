#include "TelldusMain.h"
#include "ConnectionListener.h"
#include "EventHandler.h"
#include "ClientCommunicationHandler.h"

#include <list>

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

	std::list<ClientCommunicationHandler *> clientCommunicationHandlerList;

	while(running) {
		if (!eventHandler.waitForAny()) {
			continue;
		}
		if (clientEvent->isSignaled()) {
			clientEvent->clearSignal();
			//New client connection
			
			
			TelldusCore::Socket *s = clientListener.retrieveClientSocket();
			if(s){
				Event *handlerEvent = eventHandler.addEvent();
				ClientCommunicationHandler *clientCommunication = new ClientCommunicationHandler(s, handlerEvent);
				clientCommunication->start();
				clientCommunicationHandlerList.push_back(clientCommunication);
			}
			
			clientListener.listen(clientEvent);
		}


		for ( std::list<ClientCommunicationHandler *>::iterator it = clientCommunicationHandlerList.begin(); it != clientCommunicationHandlerList.end(); ){
			if ((*it)->isDone()){
				delete *it;
				it = clientCommunicationHandlerList.erase(it);
			}
			else{
				++it;
			}
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
