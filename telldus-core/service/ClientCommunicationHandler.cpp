#include "ClientCommunicationHandler.h"

class ClientCommunicationHandler::PrivateData {
public:
	std::wstring clientMessage;
};

ClientCommunicationHandler::ClientCommunicationHandler(const std::wstring &clientMessage)
	:Thread()
{
	d = new PrivateData;
	d->clientMessage = clientMessage;
	
}

ClientCommunicationHandler::~ClientCommunicationHandler(void)
{
	delete d;
}

void ClientCommunicationHandler::run(){
	//run thread

//	std::wstring clientMessage = s->read();

	if(d->clientMessage == L"tdGetNumberOfDevices"){
		//starta ny tråd (ny klass, ärv från Thread)
		//skicka in meddelandet i denna tråd
		//kolla där vad det är för meddelande
		//do stuff
		//TODO
	}

//	delete s;	//TODO: Cleanup
}


