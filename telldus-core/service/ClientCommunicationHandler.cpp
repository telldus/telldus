#include "ClientCommunicationHandler.h"
#include "Message.h"

class ClientCommunicationHandler::PrivateData {
public:
	TelldusCore::Socket *clientSocket;
	Event *event;
	bool done;
	DeviceManager *deviceManager;
};

ClientCommunicationHandler::ClientCommunicationHandler(){

}

ClientCommunicationHandler::ClientCommunicationHandler(TelldusCore::Socket *clientSocket, Event *event, DeviceManager *deviceManager)
	:Thread()
{
	d = new PrivateData;
	d->clientSocket = clientSocket;
	d->event = event;
	d->done = false;
	d->deviceManager = deviceManager;
	
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

	int intReturn;
	std::wstring strReturn;
	strReturn = L"";
	parseMessage(clientMessage, &intReturn, &strReturn);
	
	TelldusCore::Message msg;
	
	if(strReturn == L""){
		msg.addArgument(intReturn);
	}
	else{
		msg.addArgument(strReturn);
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


void ClientCommunicationHandler::parseMessage(const std::wstring &clientMessage, int *intReturn, std::wstring *wstringReturn){

	(*intReturn) = 0;
	(*wstringReturn) = L"";
	std::wstring msg(clientMessage);	//Copy
	std::wstring function(TelldusCore::Message::takeString(&msg));
	
	if (function == L"tdTurnOn") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = d->deviceManager->doAction(deviceId, TELLSTICK_TURNON, 0);
	} else if (function == L"tdTurnOff") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = d->deviceManager->doAction(deviceId, TELLSTICK_TURNOFF, 0);

	} else if (function == L"tdBell") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = d->deviceManager->doAction(deviceId, TELLSTICK_BELL, 0);

	} else if (function == L"tdDim") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		int level = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = d->deviceManager->doAction(deviceId, TELLSTICK_DIM, level);

	} else if (function == L"tdLearn") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = d->deviceManager->doAction(deviceId, TELLSTICK_LEARN, 0);

	} else if (function == L"tdLastSentCommand") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		int methodsSupported = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = 0;	// tdLastSentCommand(deviceId, methodsSupported); TODO

	} else if (function == L"tdLastSentValue") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		const char *value = "";	//tdLastSentValue(deviceId);	TODO
		(*wstringReturn) =  TelldusCore::Message::charToWstring(value);

	} else if(function == L"tdGetNumberOfDevices"){

		(*intReturn) = d->deviceManager->getNumberOfDevices();

	} else if (function == L"tdGetDeviceId") {
		int deviceIndex = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = d->deviceManager->getDeviceId(deviceIndex);
	
	} else if (function == L"tdGetDeviceType") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = d->deviceManager->getDeviceType(deviceId);

	} else if (function == L"tdGetName") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		(*wstringReturn) = d->deviceManager->getDeviceName(deviceId);

	} else if (function == L"tdSetName") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		std::wstring name = TelldusCore::Message::takeString(&msg);
		(*intReturn) = d->deviceManager->setDeviceName(deviceId, name);
		//TODO, signal event

	} else if (function == L"tdGetProtocol") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		(*wstringReturn) = d->deviceManager->getDeviceProtocol(deviceId);

	} else if (function == L"tdSetProtocol") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		std::wstring protocol = TelldusCore::Message::takeString(&msg);
		(*intReturn) = d->deviceManager->setDeviceProtocol(deviceId, protocol);
		//TODO, signal event

	} else if (function == L"tdGetModel") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		(*wstringReturn) = d->deviceManager->getDeviceModel(deviceId);

	} else if (function == L"tdSetModel") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		std::wstring model = TelldusCore::Message::takeString(&msg);
		(*intReturn) = d->deviceManager->setDeviceModel(deviceId, model);
		//TODO, signal event

	} else if (function == L"tdSetDeviceParameter") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		std::wstring name = TelldusCore::Message::takeString(&msg);
		std::wstring value = TelldusCore::Message::takeString(&msg);
		(*intReturn) = 0;	//tdSetDeviceParameter(deviceId, name.c_str(), value.c_str());
		//TODO, signal event

	} else if (function == L"tdGetDeviceParameter") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		std::wstring name = TelldusCore::Message::takeString(&msg);
		std::wstring defaultValue = TelldusCore::Message::takeString(&msg);
		const char *value = "Device parameter default";	//tdGetDeviceParameter(deviceId, name.c_str(), defaultValue.c_str());
		(*wstringReturn) = TelldusCore::Message::charToWstring(value);

	} else if (function == L"tdAddDevice") {
		//TODO: Lock
		if(d->deviceManager->addDevice()){
			(*intReturn) = 1;
			//TODO: signalEvent, or do that from where this is called? Or even inside addDevice?
		}

	} else if (function == L"tdRemoveDevice") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		if(d->deviceManager->removeDevice(deviceId)){
			(*intReturn) = 1;
			//TODO: signalEvent, or do that from where this is called? Or even inside removeDevice?
		}

	} else if (function == L"tdMethods") {
		int deviceId = TelldusCore::Message::takeInt(&msg);
		int intMethodsSupported = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = 63;	// tdMethods(deviceId, intMethodsSupported);

	} else if (function == L"tdGetErrorString") {
		int intErrorNo = TelldusCore::Message::takeInt(&msg);
		const char *response = "Default error string";	//tdGetErrorString(intErrorNo);
		(*wstringReturn) = TelldusCore::Message::charToWstring(response);

	} else if (function == L"tdSendRawCommand") {
		std::wstring command = TelldusCore::Message::takeString(&msg);
		int reserved = TelldusCore::Message::takeInt(&msg);
		(*intReturn) = 0;	//tdSendRawCommand(command.c_str(), reserved);
		
	} else if (function == L"tdConnectTellStickController") {
		int vid = TelldusCore::Message::takeInt(&msg);
		int pid = TelldusCore::Message::takeInt(&msg);
		std::wstring serial = TelldusCore::Message::takeString(&msg);
		//tdConnectTellStickController(vid, pid, serial.c_str());
		(*wstringReturn) = L"";
		//TODO, signal event? Or done in other place?

	} else if (function == L"tdDisconnectTellStickController") {
		int vid = TelldusCore::Message::takeInt(&msg);
		int pid = TelldusCore::Message::takeInt(&msg);
		std::wstring serial = TelldusCore::Message::takeString(&msg);
		//tdDisconnectTellStickController(vid, pid, serial.c_str());
		(*wstringReturn) = L"";
		//TODO, signal event? Or done in other place?

	}
	else{
		(*intReturn) = TELLSTICK_ERROR_UNKNOWN;
	}
}

