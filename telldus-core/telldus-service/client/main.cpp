
#include <libtelldus-core/telldus-core.h>
#include <string>
#include <libtelldus-core/common.h>
#include <stdlib.h>

#include "Manager.h"

using namespace TelldusService;

void WINAPI tdInit(void) {
	Manager::getInstance(); //Create the manager-object
}

int WINAPI tdRegisterDeviceEvent( TDDeviceEvent eventFunction, void *context ) {
	Manager *manager = Manager::getInstance();
	return manager->registerDeviceEvent( eventFunction, context );
}

int WINAPI tdRegisterDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context ) {
	Manager *manager = Manager::getInstance();
	return manager->registerDeviceChangeEvent( eventFunction, context );
}

int WINAPI tdRegisterRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context ) {
	Manager *manager = Manager::getInstance();
	return manager->registerRawDeviceEvent( eventFunction, context );
}

int WINAPI tdUnregisterCallback( int callbackId ) {
	Manager *manager = Manager::getInstance();
	return manager->unregisterCallback( callbackId );
}

void WINAPI tdClose(void) {
	Manager::close();
}

void WINAPI tdReleaseString(char *string) {
#ifdef _WINDOWS
	SysFreeString((BSTR)string);
#else
	free(string);
#endif
}

int WINAPI tdTurnOn(int intDeviceId){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdTurnOn");
	message.addArgument(intDeviceId);
	return manager->sendAndReceiveInt(message, &ok);
}

int WINAPI tdTurnOff(int intDeviceId){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdTurnOff");
	message.addArgument(intDeviceId);
	return manager->sendAndReceiveInt(message, &ok);
}

int WINAPI tdBell(int intDeviceId){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdBell");
	message.addArgument(intDeviceId);
	return manager->sendAndReceiveInt(message, &ok);
}

int WINAPI tdDim(int intDeviceId, unsigned char level){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdDim");
	message.addArgument(intDeviceId);
	message.addArgument(level);
	return manager->sendAndReceiveInt(message, &ok);
}

int WINAPI tdLearn(int intDeviceId) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdLearn");
	message.addArgument(intDeviceId);
	return manager->sendAndReceiveInt(message, &ok);
}

int WINAPI tdLastSentCommand( int intDeviceId, int methodsSupported ) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdLastSentCommand");
	message.addArgument(intDeviceId);
	message.addArgument(methodsSupported);
	return manager->sendAndReceiveInt(message, &ok);
}

char * WINAPI tdLastSentValue( int intDeviceId ) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdLastSentValue");
	message.addArgument(intDeviceId);
	std::string retval = manager->sendAndReceiveString(message, &ok);
	if (!ok) {
		return wrapStdString("0");
	}
	return wrapStdString(retval);
}

int WINAPI tdGetNumberOfDevices(void){
	//We go through the Manager so we can cache the value
	Manager *manager = Manager::getInstance();
	return manager->numberOfDevices();
}

int WINAPI tdGetDeviceId(int intDeviceIndex){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetDeviceId");
	message.addArgument(intDeviceIndex);
	return manager->sendAndReceiveInt(message, &ok);
}

int WINAPI tdGetDeviceType(int intDeviceId) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetDeviceType");
	message.addArgument(intDeviceId);
	return manager->sendAndReceiveInt(message, &ok);
}

char * WINAPI tdGetName(int intDeviceId){
	Manager *manager = Manager::getInstance();
	std::string name = manager->deviceName(intDeviceId);
	return wrapStdString(name);
}

bool WINAPI tdSetName(int intDeviceId, const char* strNewName) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSetName");
	message.addArgument(intDeviceId);
	message.addArgument(strNewName);
	int retval = manager->sendAndReceiveInt(message, &ok);
	if (!ok) {
		return false;
	}
	return (retval == 1);
}

char* WINAPI tdGetProtocol(int intDeviceId){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetProtocol");
	message.addArgument(intDeviceId);
	std::string retval = manager->sendAndReceiveString(message, &ok);
	if (!ok) {
		retval = "arctech";
	}
	return wrapStdString(retval);
}

bool WINAPI tdSetProtocol(int intDeviceId, const char* strProtocol) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSetProtocol");
	message.addArgument(intDeviceId);
	message.addArgument(strProtocol);
	int retval = manager->sendAndReceiveInt(message, &ok);
	if (!ok) {
		return false;
	}
	return (retval == 1);
}

char* WINAPI tdGetModel(int intDeviceId) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetModel");
	message.addArgument(intDeviceId);
	std::string retval = manager->sendAndReceiveString(message, &ok);
	if (!ok) {
		retval = "";
	}
	return wrapStdString(retval);
}

bool WINAPI tdSetModel(int intDeviceId, const char *strModel) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSetModel");
	message.addArgument(intDeviceId);
	message.addArgument(strModel);
	int retval = manager->sendAndReceiveInt(message, &ok);
	if (!ok) {
		return false;
	}
	return (retval == 1);
}

bool WINAPI tdSetDeviceParameter(int intDeviceId, const char *strName, const char *strValue) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSetDeviceParameter");
	message.addArgument(intDeviceId);
	message.addArgument(strName);
	message.addArgument(strValue);
	int retval = manager->sendAndReceiveInt(message, &ok);
	if (!ok) {
		return false;
	}
	return (retval == 1);
}

char * WINAPI tdGetDeviceParameter(int intDeviceId, const char *strName, const char *defaultValue){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetDeviceParameter");
	message.addArgument(intDeviceId);
	message.addArgument(strName);
	message.addArgument(defaultValue);
	std::string retval = manager->sendAndReceiveString(message, &ok);
	if (!ok) {
		retval = defaultValue;
	}
	return wrapStdString(retval);
}

int WINAPI tdAddDevice(){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdAddDevice");
	return manager->sendAndReceiveInt(message, &ok);
}

bool WINAPI tdRemoveDevice(int intDeviceId) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdRemoveDevice");
	message.addArgument(intDeviceId);
	return (manager->sendAndReceiveInt(message, &ok) == 1);
}

int WINAPI tdMethods(int id, int methodsSupported) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdMethods");
	message.addArgument(id);
	message.addArgument(methodsSupported);
	return manager->sendAndReceiveInt(message, &ok);
}

char * WINAPI tdGetErrorString(int intErrorNo) {
	bool ok;
	if (intErrorNo == TELLSTICK_ERROR_CONNECTING_SERVICE) {
		//This error cannot be fetched from the service, obviously
		return wrapStdString("Could not connect to the Telldus Service");
	}
	Manager *manager = Manager::getInstance();
	Message message("tdGetErrorString");
	message.addArgument(intErrorNo);
	std::string retval = manager->sendAndReceiveString(message, &ok);
	if (!ok) {
		retval = "Unknown error";
	}
	return wrapStdString(retval);
}

int WINAPI tdSendRawCommand(const char *command, int reserved) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSendRawCommand");
	message.addArgument(command);
	message.addArgument(reserved);
	return manager->sendAndReceiveInt(message, &ok);
}

void WINAPI tdConnectTellStickController(int vid, int pid, const char *serial) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdConnectTellStickController");
	message.addArgument(vid);
	message.addArgument(pid);
	message.addArgument(serial);
	manager->send(message, &ok);
}

void WINAPI tdDisconnectTellStickController(int vid, int pid, const char *serial) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdDisconnectTellStickController");
	message.addArgument(vid);
	message.addArgument(pid);
	message.addArgument(serial);
	manager->send(message, &ok);
}
