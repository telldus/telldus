#include "ClientCommunicationHandler.h"
#include "Message.h"

class ClientCommunicationHandler::PrivateData {
public:
	TelldusCore::Socket *clientSocket;
	Event *event;
	bool done;
};

ClientCommunicationHandler::ClientCommunicationHandler(){

}

ClientCommunicationHandler::ClientCommunicationHandler(TelldusCore::Socket *clientSocket, Event *event)
	:Thread()
{
	d = new PrivateData;
	d->clientSocket = clientSocket;
	d->event = event;
	d->done = false;
	
}

ClientCommunicationHandler::~ClientCommunicationHandler(void)
{
	wait();
	delete(d->clientSocket);
	delete d;
}

void ClientCommunicationHandler::run(){
	//run thread

	
	std::wstring clientMessage = d->clientSocket->read();

	//std::wstring temp = parseMessage(clientMessage);
	
	

	//We are done, signal for removal
	d->done = true;
	d->event->signal();
}

bool ClientCommunicationHandler::isDone(){
	return d->done;
}


std::wstring parseMessage(const std::wstring &clientMessage){

	std::wstring msg(clientMessage);	//Copy
	//std::wstring function(TelldusCore::Message::takeString(&msg));
	if(clientMessage == L"tdGetNumberOfDevices"){
		//starta ny tråd (ny klass, ärv från Thread)
		//skicka in meddelandet i denna tråd
		//kolla där vad det är för meddelande
		//do stuff
		//TODO
	}

	return L"Dummy";
}
