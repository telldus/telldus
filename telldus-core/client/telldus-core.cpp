/**
 * @defgroup core telldus-core
 * Telldus Core is the base module used to interface a Telldus TellStick.
 * @{
 */

#include "telldus-core.h"
#include "common.h"
#include "Client.h"
#include "Message.h"
#include "Socket.h"
#include <stdlib.h>

using namespace TelldusCore;


/**
 * @def TELLSTICK_TURNON
 * Device-flag for devices supporting the tdTurnOn() call.
 *
 * @def TELLSTICK_TURNOFF
 * Device-flag for devices supporting the tdTurnOff() call.
 *
 * @def TELLSTICK_BELL
 * Device-flag for devices supporting the tdBell() call.
 *
 * @def TELLSTICK_TOGGLE
 * This method is currently unimplemented.
 *
 * @def TELLSTICK_DIM
 * Device-flag for devices supporting the tdDim() call.
 *
 * @def TELLSTICK_EXECUTE
 * Device-flag for devices (scenes) supporting the tdExecute() call.
 *
 * @def TELLSTICK_UP
 * Device-flag for devices supporting the tdUp() call.
 *
 * @def TELLSTICK_DOWN
 * Device-flag for devices supporting the tdDown() call.
 *
 * @def TELLSTICK_STOP
 * Device-flag for devices supporting the tdStop() call.
 *
 * @def TELLSTICK_LEARN
 * Device-flag for devices supporting the tdLearn() call.
 *
 * @def TELLSTICK_TYPE_DEVICE
 * Device type of a single device.
 *
 * @def TELLSTICK_TYPE_GROUP
 * Device type of a device which contains other devices.
 *
 * @def TELLSTICK_TYPE_SCENE
 * Device type of a device which contains other devices that are sent different commands when executed (i.e. one device turns on, another turns off).
 *
 * @def TELLSTICK_SUCCESS
 * Error code. Returned when the command succeeded.
 *
 * @def TELLSTICK_ERROR_BROKEN_PIPE
 * Error code. Pipe broken during communication.
 *
 * @def TELLSTICK_ERROR_NOT_FOUND
 * Error code. Returned if a TellStick was not found on the system.
 *
 * @def TELLSTICK_ERROR_PERMISSION_DENIED
 * Error code. Returned if the user doesn't have privileges to open
 * the TellStick device.
 *
 * @def TELLSTICK_ERROR_DEVICE_NOT_FOUND
 * Error code. The supplied device id was not found.
 *
 * @def TELLSTICK_ERROR_METHOD_NOT_SUPPORTED
 * Error code. The requested method is not supported by the device.
 * This should be avoided by a call to tdMethods().
 *
 * @def TELLSTICK_ERROR_COMMUNICATION
 * Error code. An error occurred when communicating with TellStick.
 *
 * @def TELLSTICK_ERROR_CONNECTING_SERVICE
 * Error code. The client library could not connect to the service.
 * Maybe it is not running?
 *
 * @def TELLSTICK_ERROR_UNKNOWN_RESPONSE
 * Error code. The client library received a response from the service
 * it did not understand.
 *
 * @def TELLSTICK_ERROR_SYNTAX
 * Error code. Input/command could not be parsed or didn't follow
 * input rules.
 *
 * @def TELLSTICK_ERROR_COMMUNICATING_SERVICE
 * Error code. Timeout waiting for response from the Telldus Service.
 *
 * @def TELLSTICK_ERROR_UNKNOWN
 * Error code. An unkown error has occurred.
 */

void WINAPI tdInit(void) {
	Client::getInstance(); //Create the manager-object
}

int WINAPI tdRegisterDeviceEvent( TDDeviceEvent eventFunction, void *context ) {
	Client *client = Client::getInstance();
	return client->registerDeviceEvent( eventFunction, context );
}

int WINAPI tdRegisterRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context ) {
	Client *client = Client::getInstance();
	return client->registerRawDeviceEvent( eventFunction, context );
}

int WINAPI tdRegisterDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context) {
	Client *client = Client::getInstance();
	return client->registerDeviceChangeEvent( eventFunction, context );
}

int WINAPI tdRegisterSensorEvent( TDSensorEvent eventFunction, void *context) {
	Client *client = Client::getInstance();
	return client->registerSensorEvent( eventFunction, context );
}

int WINAPI tdUnregisterCallback( int callbackId ) {
	Client *client = Client::getInstance();
	return client->unregisterCallback( callbackId );
}

/**
 * Close the library and clean up the cache it uses.
 * This should be called when the library is not supposed to be used anymore
 **/
void WINAPI tdClose(void) {
	Client::close();
}

/**
 * This method releases resources allocated by telldus-core.
 * It should be called on the returned value from all functions return <tt>char *</tt>
 **/
void WINAPI tdReleaseString(char *string) {
#ifdef _WINDOWS
	SysFreeString((BSTR)string);
#else
	free(string);
#endif
}


/**
 * Turns a device on.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to turn on.
 **/
int WINAPI tdTurnOn(int intDeviceId){
	Message msg(L"tdTurnOn");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Turns a device off.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to turn off.
 */
int WINAPI tdTurnOff(int intDeviceId){
	Message msg(L"tdTurnOff");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Sends bell command to devices supporting this.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to send bell to
 */
int WINAPI tdBell(int intDeviceId){
	Message msg(L"tdBell");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Dims a device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to dim
 * @param level The level the device should dim to. This value should be 0-255
 */
int WINAPI tdDim(int intDeviceId, unsigned char level){
	Message msg(L"tdDim");
	msg.addArgument(intDeviceId);
	msg.addArgument(level);
	return Client::getIntegerFromService(msg);
}

/**
 * Execute a scene action.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The id to execute
 */
int WINAPI tdExecute(int intDeviceId){
	Message msg(L"tdExecute");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Send "up" command to device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to send the command to
 */
int WINAPI tdUp(int intDeviceId){
	Message msg(L"tdUp");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Send "down" command to device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to send the command to
 */
int WINAPI tdDown(int intDeviceId){
	Message msg(L"tdDown");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Send "stop" command to device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to stop
 */
int WINAPI tdStop(int intDeviceId){
	Message msg(L"tdStop");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Sends a special learn command to some devices that need a special learn-command
 * to be used from TellStick
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to learn.
 */
int WINAPI tdLearn(int intDeviceId) {
	Message msg(L"tdLearn");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Returns the last sent command to a specific device
 * @param intDeviceId The device id to query
 * @param methodsSupported The methods supported by the client. See tdMethods() for more information.
 * @returns the last sent command as integer, example TELLSTICK_TURNON or TELLSTICK_TURNOFF
 */
int WINAPI tdLastSentCommand(int intDeviceId, int methodsSupported ) {
	Message msg(L"tdLastSentCommand");
	msg.addArgument(intDeviceId);
	msg.addArgument(methodsSupported);
	return Client::getIntegerFromService(msg);
}

/**
 * If the last sent command it TELLSTICK_DIM this returns the dimmed value.
 * @param intDeviceId The device id to query
 * @returns the the value as a human readable string, example "128" for 50%
 */
char * WINAPI tdLastSentValue( int intDeviceId ) {
	Message msg(L"tdLastSentValue");
	msg.addArgument(intDeviceId);
	std::wstring strReturn = Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * This function returns the number of devices configured
 * @returns an integer of the total number of devices configured
 */
int WINAPI tdGetNumberOfDevices(void){
	return Client::getIntegerFromService(Message(L"tdGetNumberOfDevices"));
}

/**
 * This function returns the unique id of a device with a specific index.
 * To get all the id numbers you should loop over all the devices:
 * \code
 * int intNumberOfDevices = tdGetNumberOfDevices();
 * for (int i = 0; i < intNumberOfDevices; i++) {
 *   int id = tdGetDeviceId( i );
 *   // id now contains the id number of the device with index of i
 * }
 * \endcode
 * @param intDeviceIndex The device index to query. The index starts from 0.
 * @returns the unique id for the device or -1 if the device is not found.
 */
int WINAPI tdGetDeviceId(int intDeviceIndex){
	Message msg(L"tdGetDeviceId");
	msg.addArgument(intDeviceIndex);
	return Client::getIntegerFromService(msg);
}

/**
 * Returns which type the device is. The device could be either
 * TELLSTICK_TYPE_DEVICE, TELLSTICK_TYPE_GROUP or TELLSTICK_TYPE_SCENE
 */
int WINAPI tdGetDeviceType(int intDeviceId) {
	Message msg(L"tdGetDeviceType");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Query a device for it's name.
 * @param intDeviceId The unique id of the device to query
 * @returns The name of the device or an empty string if the device is not found.
 */
char * WINAPI tdGetName(int intDeviceId){
	Message msg(L"tdGetName");
	msg.addArgument(intDeviceId);
	std::wstring strReturn =  Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * Sets a new name for a device. The devices are global for all application, changing
 * this will change the name for other applications aswell.
 * @param intDeviceId The device id to change the name for
 * @param strNewName The new name for the devices
 * @returns \c true on success, \c false otherwise.
 */
bool WINAPI tdSetName(int intDeviceId, const char* strNewName){
	Message msg(L"tdSetName");
	msg.addArgument(intDeviceId);
	msg.addArgument(strNewName);
	return Client::getBoolFromService(msg);
}

/**
 * @returns the protocol used by a specific device.
 * @param intDeviceId The device id to query.
 */
char* WINAPI tdGetProtocol(int intDeviceId){
	Message msg(L"tdGetProtocol");
	msg.addArgument(intDeviceId);
	std::wstring strReturn =  Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * This changes the current protocol used by a device. After changing the protocol,
 * setting new parameters is required.
 * @param intDeviceId The device to change.
 * @param strProtocol The new protocol to use.
 * @returns \c true on success, \c false otherwise.
 * @sa tdSetModel()
 * @sa tdSetDeviceParameter()
 */
bool WINAPI tdSetProtocol(int intDeviceId, const char* strProtocol){
	Message msg(L"tdSetProtocol");
	msg.addArgument(intDeviceId);
	msg.addArgument(strProtocol);
	return Client::getBoolFromService(msg);
}

/**
 * @returns the model for a device. Not all protocols uses this.
 * @param intDeviceId The device to query.
 */
char* WINAPI tdGetModel(int intDeviceId){
	Message msg(L"tdGetModel");
	msg.addArgument(intDeviceId);
	std::wstring strReturn = Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * Sets a new model for a device. Which model to set depends on the
 * current protocol.
 * @param intDeviceId The device to change
 * @param strModel The new model
 * @returns \c true on success, \c false otherwise.
 */
bool WINAPI tdSetModel(int intDeviceId, const char *strModel){
	Message msg(L"tdSetModel");
	msg.addArgument(intDeviceId);
	msg.addArgument(strModel);
	return Client::getBoolFromService(msg);
}

/**
 * Sets a new protocol specific parameter. Please see the documentation of the protocols
 * before setting any parameter.
 * @param intDeviceId The device to change.
 * @param strName The parameter to change.
 * @param strValue The new value for the parameter.
 * @returns \c true on success, \c false otherwise.
 */
bool WINAPI tdSetDeviceParameter(int intDeviceId, const char *strName, const char *strValue){
	Message msg(L"tdSetDeviceParameter");
	msg.addArgument(intDeviceId);
	msg.addArgument(strName);
	msg.addArgument(strValue);
	return Client::getBoolFromService(msg);
}

/**
 * @returns any protocol specific parameter specified by \c strName
 * @param intDeviceId The device to query.
 * @param strName The name of the parameter to query.
 * @param defaultValue A defaultValue to return if the current parameter hasn't previously been set.
 */
char * WINAPI tdGetDeviceParameter(int intDeviceId, const char *strName, const char *defaultValue){
	Message msg(L"tdGetDeviceParameter");
	msg.addArgument(intDeviceId);
	msg.addArgument(strName);
	msg.addArgument(defaultValue);
	std::wstring strReturn = Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * Add a new device to the global database of devices. This function must be called first before
 * any call to tdSetName(), tdSetProtocol() and similar functions.
 * @returns the new device id for the newly created device. If the creation fails it returnes a
 * negative value.
 */
int WINAPI tdAddDevice(){
	Message msg(L"tdAddDevice");
	return Client::getIntegerFromService(msg);
}

/**
 * Removes a device.
 * @returns \c true on success, \c false otherwise.
 */
bool WINAPI tdRemoveDevice(int intDeviceId){
	Message msg(L"tdRemoveDevice");
	msg.addArgument(intDeviceId);
	return Client::getBoolFromService(msg);
}

/**
 * Query a device for which methods it supports. By supplying the methods you support
 * the library could remap the methods a device support for better fit the application.
 * Example of querying a device supporting TELLSTICK_BELL:
 * \code
 * int methods = tdMethods(id, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_BELL);
 * //methods is now TELLSTICK_BELL
 * int methods = tdMethods(idm TELLSTICK_TURNON | TELLSTICK_TURNOFF);
 * //methods is now TELLSTICK_TURNON because the client application doesn't support TELLSTICK_BELL
 * \endcode
 * @param id The device id to query
 * @param methodsSupported The methods the client application supports
 * @returns The method-flags OR'ed into an integer.
 * @sa TELLSTICK_TURNON
 * @sa TELLSTICK_TURNOFF
 * @sa TELLSTICK_BELL
 * @sa TELLSTICK_TOGGLE
 * @sa TELLSTICK_DIM
 * @sa TELLSTICK_EXECUTE
 * @sa TELLSTICK_UP
 * @sa TELLSTICK_DOWN
 * @sa TELLSTICK_STOP
 */
int WINAPI tdMethods(int id, int methodsSupported){
	Message msg(L"tdMethods");
	msg.addArgument(id);
	msg.addArgument(methodsSupported);
	return Client::getIntegerFromService(msg);
}

/**
 * Get a human readable string from an error code returned
 * from a function in telldus-core.
 * @param intErrorNo The error code to translate.
 * @returns a string ready to show to the user.
 * @sa TELLSTICK_SUCCESS
 * @sa TELLSTICK_ERROR_NOT_FOUND
 * @sa TELLSTICK_ERROR_PERMISSION_DENIED
 * @sa TELLSTICK_ERROR_DEVICE_NOT_FOUND
 * @sa TELLSTICK_ERROR_METHOD_NOT_SUPPORTED
 * @sa TELLSTICK_ERROR_COMMUNICATION
 * @sa TELLSTICK_ERROR_CONNECTING_SERVICE
 * @sa TELLSTICK_ERROR_UNKNOWN_RESPONSE
 * @sa TELLSTICK_ERROR_SYNTAX
 * @sa TELLSTICK_ERROR_BROKEN_PIPE
 * @sa TELLSTICK_ERROR_COMMUNICATING_SERVICE
 * @sa TELLSTICK_ERROR_UNKNOWN
 */
char * WINAPI tdGetErrorString(int intErrorNo) {
	const int numResponses = 10;
	const char *responses[numResponses] = {
		"Success",
		"TellStick not found",
		"Permission denied",
		"Device not found",
		"The method you tried to use is not supported by the device",
		"An error occurred while communicating with TellStick",
		"Could not connect to the Telldus Service",
		"Received an unknown response",
		"Syntax error",
		"Broken pipe"
		"An error occured while communicating with the Telldus Service"
	};
	std::string strReturn;
	intErrorNo = abs(intErrorNo); //We don't use negative values here.
	if (intErrorNo >= numResponses) {
		strReturn = "Unknown error";
	} else {
		// Copy the error string to strReturn
		strReturn = responses[intErrorNo];
	}
	return wrapStdString(strReturn);
}

/**
 * Send a raw command to TellStick. Please read the TellStick protocol
 * definition on how the command should be constructed.
 * @param command The command for TellStick in its native format
 * @returns TELLSTICK_SUCCESS on success or one of the errorcodes on failure
 */
int WINAPI tdSendRawCommand(const char *command, int reserved) {
	std::wstring wcommand;
	for(int i = 0; i < strlen(command);++i) {
		wcommand.append(1, (unsigned char)command[i]);
	}
	Message msg(L"tdSendRawCommand");
	msg.addArgument(wcommand);
	msg.addArgument(reserved);
	return Client::getIntegerFromService(msg);
}


void WINAPI tdConnectTellStickController(int vid, int pid, const char *serial) {
	Message msg(L"tdConnectTellStickController");
	msg.addArgument(vid);
	msg.addArgument(pid);
	msg.addArgument(serial);
	Client::getWStringFromService(msg);
}

void WINAPI tdDisconnectTellStickController(int vid, int pid, const char *serial) {
	Message msg(L"tdDisconnectTellStickController");
	msg.addArgument(vid);
	msg.addArgument(pid);
	msg.addArgument(serial);
	Client::getWStringFromService(msg);
}

/**
 * Use this function to iterate over all sensors. Iterate until
 * TELLSTICK_SUCCESS is not returned
 * @param protocol A byref string where the protocol of the sensor will be placed
 * @param protocolLen The length of the \c protocol parameter
 * @param model A byref string where the model of the sensor will be placed
 * @param modelLen The length of the \c model parameter
 * @param id A byref int where the id of the sensor will be placed
 * @param dataTypes A byref int with flags for the supported sensor values
 * @returns TELLSTICK_SUCCESS if there is more sensors to be fetched
 */
int WINAPI tdSensor(char *protocol, int protocolLen, char *model, int modelLen, int *id, int *dataTypes) {
	Client *client = Client::getInstance();
	return client->getSensor(protocol, protocolLen, model, modelLen, id, dataTypes);
}

/**
 * Get one of the supported sensor values from a sensor. Make sure it support
 * the value type first by calling tdSensor(). The triplet \c protocol,
 * \c model, and \c id together identifies a sensor.
 * @param protocol The protocol for the sensor
 * @param model The model for the sensor
 * @param id The id of the sensor
 * @param dataType One of the datatype to retrieve
 * @param value A byref string where the value will be places
 * @param len The length of the \c value parameter
 * @param timestamp A byref int where the timestamp of the value will be placed
 * @returns TELLSTICK_SUCCESS if the value could be fetched or one of the
 * errorcodes on failure
 */
int WINAPI tdSensorValue(const char *protocol, const char *model, int id, int dataType, char *value, int len, int *timestamp) {
	Message msg(L"tdSensorValue");
	msg.addArgument(protocol);
	msg.addArgument(model);
	msg.addArgument(id);
	msg.addArgument(dataType);
	std::wstring retval = Client::getWStringFromService(msg);
	if (retval.length() == 0) {
		return TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
	}

	std::wstring v = Message::takeString(&retval);
	int t = Message::takeInt(&retval);
	if (value && len) {
		strncpy(value, TelldusCore::wideToString(v).c_str(), len);
	}
	if (timestamp) {
		(*timestamp) = t;
	}
	return TELLSTICK_SUCCESS;
}


/* @} */
