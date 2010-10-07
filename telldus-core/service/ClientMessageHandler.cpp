#include "ClientMessageHandler.h"

class ClientMessageHandler::PrivateData {
public:
	std::wstring clientMessage;
};

ClientMessageHandler::ClientMessageHandler(const std::wstring &clientMessage)
	:Thread()
{
	d = new PrivateData;
	d->clientMessage = clientMessage;
	
}

ClientMessageHandler::~ClientMessageHandler(void)
{
	delete d;
}

void ClientMessageHandler::run(){
	//run thread
	if(d->clientMessage == L"tdGetNumberOfDevices"){
		//starta ny tråd (ny klass, ärv från Thread)
		//skicka in meddelandet i denna tråd
		//kolla där vad det är för meddelande
		//do stuff
		//TODO
	}
}


