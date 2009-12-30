
#include <libtelldus-core/telldus-core.h>
#include <string>
#include <libtelldus-core/common.h>
#include <stdlib.h>

#include "Manager.h"

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

void WINAPI tdClose(void) {
	Manager::close();
}

void WINAPI tdReleaseString(char *string) {
	free(string);
}

int WINAPI tdTurnOn(int intDeviceId){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdTurnOn");
	message.addArgument(intDeviceId);
	return manager->send(message, &ok).toInt();
}

int WINAPI tdTurnOff(int intDeviceId){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdTurnOff");
	message.addArgument(intDeviceId);
	return manager->send(message, &ok).toInt();
}

int WINAPI tdBell(int intDeviceId){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdBell");
	message.addArgument(intDeviceId);
	return manager->send(message, &ok).toInt();
}

int WINAPI tdDim(int intDeviceId, unsigned char level){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdDim");
	message.addArgument(intDeviceId);
	return manager->send(message, &ok).toInt();
}

int WINAPI tdLearn(int intDeviceId) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdLearn");
	message.addArgument(intDeviceId);
	return manager->send(message, &ok).toInt();
}

int WINAPI tdLastSentCommand( int intDeviceId, int methodsSupported ) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdLastSentCommand");
	message.addArgument(intDeviceId);
	message.addArgument(methodsSupported);
	return manager->send(message, &ok).toInt();
}

char * WINAPI tdLastSentValue( int intDeviceId ) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdLastSentValue");
	message.addArgument(intDeviceId);
	QVariant retval = manager->send(message, &ok);
	if (!ok) {
		return wrapStdString("0");
	}
	return wrapStdString(retval.toString().toStdString());
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
	return manager->send(message, &ok).toInt();
}

int WINAPI tdGetDeviceType(int intDeviceId) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetDeviceType");
	message.addArgument(intDeviceId);
	return manager->send(message, &ok).toInt();
}

char * WINAPI tdGetName(int intDeviceId){
	Manager *manager = Manager::getInstance();
	QString name = manager->deviceName(intDeviceId);
	return wrapStdString(name.toStdString());
}

bool WINAPI tdSetName(int intDeviceId, const char* strNewName) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSetName");
	message.addArgument(intDeviceId);
	message.addArgument(strNewName);
	bool retval = manager->send(message, &ok).toBool();
	if (!ok) {
		return false;
	}
	return retval;
}

char* WINAPI tdGetProtocol(int intDeviceId){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetProtocol");
	message.addArgument(intDeviceId);
	QString retval = manager->send(message, &ok).toString();
	if (!ok) {
		retval = "arctech";
	}
	return wrapStdString(retval.toStdString());
}

bool WINAPI tdSetProtocol(int intDeviceId, const char* strProtocol) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSetProtocol");
	message.addArgument(intDeviceId);
	message.addArgument(strProtocol);
	bool retval = manager->send(message, &ok).toBool();
	if (!ok) {
		return false;
	}
	return retval;
}

char* WINAPI tdGetModel(int intDeviceId) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetModel");
	message.addArgument(intDeviceId);
	QString retval = manager->send(message, &ok).toString();
	if (!ok) {
		retval = "";
	}
	return wrapStdString(retval.toStdString());
}

bool WINAPI tdSetModel(int intDeviceId, const char *strModel) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSetModel");
	message.addArgument(intDeviceId);
	message.addArgument(strModel);
	bool retval = manager->send(message, &ok).toBool();
	if (!ok) {
		return false;
	}
	return retval;
}

bool WINAPI tdSetDeviceParameter(int intDeviceId, const char *strName, const char *strValue) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSetDeviceParameter");
	message.addArgument(intDeviceId);
	message.addArgument(strName);
	message.addArgument(strValue);
	bool retval = manager->send(message, &ok).toBool();
	if (!ok) {
		return false;
	}
	return retval;
}

char * WINAPI tdGetDeviceParameter(int intDeviceId, const char *strName, const char *defaultValue){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdGetDeviceParameter");
	message.addArgument(intDeviceId);
	message.addArgument(strName);
	message.addArgument(defaultValue);
	QString retval = manager->send(message, &ok).toString();
	if (!ok) {
		retval = defaultValue;
	}
	return wrapStdString(retval.toStdString());
}

int WINAPI tdAddDevice(){
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdAddDevice");
	return manager->send(message, &ok).toInt();
}

bool WINAPI tdRemoveDevice(int intDeviceId) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdRemoveDevice");
	return manager->send(message, &ok).toBool();
}

int WINAPI tdMethods(int id, int methodsSupported) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdMethods");
	message.addArgument(id);
	message.addArgument(methodsSupported);
	return manager->send(message, &ok).toInt();
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
	QString retval = manager->send(message, &ok).toString();
	if (!ok) {
		retval = "Unknown error";
	}
	return wrapStdString(retval.toStdString());
}

int WINAPI tdSendRawCommand(const char *command, int reserved) {
	bool ok;
	Manager *manager = Manager::getInstance();
	Message message("tdSendRawCommand");
	message.addArgument(command);
	message.addArgument(reserved);
	return manager->send(message, &ok).toInt();
}
