
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
	Manager *manager = Manager::getInstance();
	Message message("tdTurnOn");
	message.addArgument(intDeviceId);
	return manager->send(message, TELLSTICK_ERROR_UNKNOWN).toInt();
}

int WINAPI tdTurnOff(int intDeviceId){
	Manager *manager = Manager::getInstance();
	Message message("tdTurnOff");
	message.addArgument(intDeviceId);
	return manager->send(message, TELLSTICK_ERROR_UNKNOWN).toInt();
}

int WINAPI tdBell(int intDeviceId){
	Manager *manager = Manager::getInstance();
	Message message("tdBell");
	message.addArgument(intDeviceId);
	return manager->send(message, TELLSTICK_ERROR_UNKNOWN).toInt();
}

int WINAPI tdDim(int intDeviceId, unsigned char level){
	Manager *manager = Manager::getInstance();
	Message message("tdDim");
	message.addArgument(intDeviceId);
	return manager->send(message, TELLSTICK_ERROR_UNKNOWN).toInt();
}

int WINAPI tdLearn(int intDeviceId) {
	Manager *manager = Manager::getInstance();
	Message message("tdLearn");
	message.addArgument(intDeviceId);
	return manager->send(message, TELLSTICK_ERROR_UNKNOWN).toInt();
}

int WINAPI tdLastSentCommand( int intDeviceId, int methodsSupported ) {
	Manager *manager = Manager::getInstance();
	Message message("tdLastSentCommand");
	message.addArgument(intDeviceId);
	message.addArgument(methodsSupported);
	return manager->send(message, TELLSTICK_TURNOFF).toInt();
}

char * WINAPI tdLastSentValue( int intDeviceId ) {
	Manager *manager = Manager::getInstance();
	Message message("tdLastSentValue");
	message.addArgument(intDeviceId);
	return wrapStdString(manager->send(message, "0").toString().toStdString());
}

int WINAPI tdGetNumberOfDevices(void){
	//We go through the Manager so we can cache the value
	Manager *manager = Manager::getInstance();
	return manager->numberOfDevices();
}

int WINAPI tdGetDeviceId(int intDeviceIndex){
	Manager *manager = Manager::getInstance();
	Message message("tdGetDeviceId");
	message.addArgument(intDeviceIndex);
	return manager->send(message, -1).toInt();
}

int WINAPI tdGetDeviceType(int intDeviceId) {
	Manager *manager = Manager::getInstance();
	Message message("tdGetDeviceType");
	message.addArgument(intDeviceId);
	return manager->send(message, TELLSTICK_TYPE_DEVICE).toInt();
}

char * WINAPI tdGetName(int intDeviceId){
	Manager *manager = Manager::getInstance();
	QString name = manager->deviceName(intDeviceId);
	return wrapStdString(name.toStdString());
}

bool WINAPI tdSetName(int intDeviceId, const char* strNewName){
	Manager *manager = Manager::getInstance();
	Message message("tdSetName");
	message.addArgument(intDeviceId);
	message.addArgument(strNewName);
	return manager->send(message, 0).toBool();
}

char* WINAPI tdGetProtocol(int intDeviceId){
	Manager *manager = Manager::getInstance();
	Message message("tdGetProtocol");
	message.addArgument(intDeviceId);
	return wrapStdString(manager->send(message, "arctech").toString().toStdString());
}

bool WINAPI tdSetProtocol(int intDeviceId, const char* strProtocol){
	Manager *manager = Manager::getInstance();
	Message message("tdSetProtocol");
	message.addArgument(intDeviceId);
	message.addArgument(strProtocol);
	return manager->send(message, 0).toBool();
}

char* WINAPI tdGetModel(int intDeviceId){
	Manager *manager = Manager::getInstance();
	Message message("tdGetModel");
	message.addArgument(intDeviceId);
	return wrapStdString(manager->send(message, "").toString().toStdString());
}

bool WINAPI tdSetModel(int intDeviceId, const char *strModel){
	Manager *manager = Manager::getInstance();
	Message message("tdSetModel");
	message.addArgument(intDeviceId);
	message.addArgument(strModel);
	return manager->send(message, 0).toBool();
}

bool WINAPI tdSetDeviceParameter(int intDeviceId, const char *strName, const char *strValue){
	Manager *manager = Manager::getInstance();
	Message message("tdSetDeviceParameter");
	message.addArgument(intDeviceId);
	message.addArgument(strName);
	message.addArgument(strValue);
	return manager->send(message, 0).toBool();
}

char * WINAPI tdGetDeviceParameter(int intDeviceId, const char *strName, const char *defaultValue){
	Manager *manager = Manager::getInstance();
	Message message("tdGetDeviceParameter");
	message.addArgument(intDeviceId);
	message.addArgument(strName);
	message.addArgument(defaultValue);
	return wrapStdString(manager->send(message, defaultValue).toString().toStdString());
}

int WINAPI tdAddDevice(){
	Manager *manager = Manager::getInstance();
	Message message("tdAddDevice");
	return manager->send(message, -1).toInt();
}

bool WINAPI tdRemoveDevice(int intDeviceId){
	Manager *manager = Manager::getInstance();
	Message message("tdRemoveDevice");
	return manager->send(message, 0).toBool();
}

int WINAPI tdMethods(int id, int methodsSupported){
	Manager *manager = Manager::getInstance();
	Message message("tdMethods");
	message.addArgument(id);
	message.addArgument(methodsSupported);
	return manager->send(message, 0).toInt();
}

char * WINAPI tdGetErrorString(int intErrorNo) {
	Manager *manager = Manager::getInstance();
	Message message("tdGetErrorString");
	message.addArgument(intErrorNo);
	return wrapStdString(manager->send(message, "Unknown error").toString().toStdString());
}

int WINAPI tdSendRawCommand(const char *command, int reserved) {
	Manager *manager = Manager::getInstance();
	Message message("tdSendRawCommand");
	message.addArgument(command);
	message.addArgument(reserved);
	return manager->send(message, TELLSTICK_ERROR_UNKNOWN).toInt();
}
