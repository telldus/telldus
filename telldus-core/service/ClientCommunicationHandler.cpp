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
	delete(d->event);
	delete(d->clientSocket);
	delete d;
}

void ClientCommunicationHandler::run(){
	//run thread

	
	std::wstring clientMessage = d->clientSocket->read();

	int intTemp;
	std::wstring strReturn;
	strReturn = L"";
	parseMessage(clientMessage, intTemp, strReturn);
	
	TelldusCore::Message msg;
	/* TODO
	if (response.type() == QVariant::Int) {
		msg.addArgument(response.toInt());
	} else {
	*/
	//allt som wstring ?
	if(strReturn == L""){
		msg.addArgument(intTemp);
	}
	else{
		msg.addArgument(strReturn);		//temp.toString().toStdString());
	}
	msg.append(L"\n");
	d->clientSocket->write(msg);

	//We are done, signal for removal
	d->done = true;
	d->event->signal();
}

bool ClientCommunicationHandler::isDone(){
	return d->done;
}


void ClientCommunicationHandler::parseMessage(const std::wstring &clientMessage, int &intReturn, std::wstring &wstringReturn){

	std::wstring msg(clientMessage);	//Copy
	std::wstring function(TelldusCore::Message::takeString(&msg));
	
	if(function == L"tdGetNumberOfDevices"){
		
		//return L"12";
		intReturn = 12;
	}
	else{
		intReturn = 5;
	}
	//return L"5";
}
