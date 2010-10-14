/**
 * @defgroup core telldus-core
 * Telldus Core is the base module used to interface a Telldus TellStick.
 * @{
 */

#include "telldus-core.h"
#include "common.h"
#include "Message.h"
#include "Socket.h"
#include <stdlib.h>


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
 * @def TELLSTICK_LEARN
 * Device-flag for devices supporting the tdLearn() call.
 * 
 * @def TELLSTICK_TYPE_DEVICE
 * Device type of a single device.
 * 
 * @def TELLSTICK_TYPE_GROUP
 * Device type of a device which contains other devices.
 * 
 * @def TELLSTICK_SUCCESS
 * Error code. Returned when the command succeeded.
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
 * @def TELLSTICK_ERROR_UNKNOWN
 * Error code. An unkown error has occurred.
 */

#define MAX_LOADSTRING 100

void WINAPI tdInit(void) {
}

int WINAPI tdRegisterDeviceEvent( TDDeviceEvent eventFunction, void *context ) {
	return 0;
}

int WINAPI tdRegisterRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context ) {
	return 0;
}

int WINAPI tdRegisterDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context) {
	return 0;
}

int WINAPI tdUnregisterCallback( int callbackId ) {
	return TELLSTICK_SUCCESS;
}


/**
 * Close the library and clean up the cache it uses.
 * This should be called when the library is not supposed to be used anymore
 **/
void WINAPI tdClose(void) {
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
	TelldusCore::Message msg(L"tdTurnOn");	
	msg.addArgument(intDeviceId);
	return msg.getClientIntegerFromSocket();
}

/**
 * Turns a device off.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to turn off.
 */
int WINAPI tdTurnOff(int intDeviceId){
	TelldusCore::Message msg(L"tdTurnOff");	
	msg.addArgument(intDeviceId);
	return msg.getClientIntegerFromSocket();
}

/**
 * Sends bell command to devices supporting this.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to send bell to
 */
int WINAPI tdBell(int intDeviceId){
	TelldusCore::Message msg(L"tdBell");	
	msg.addArgument(intDeviceId);
	return msg.getClientIntegerFromSocket();
}

/**
 * Dims a device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to dim
 * @param level The level the device should dim to. This value should be 0-255
 */
int WINAPI tdDim(int intDeviceId, unsigned char level){
	TelldusCore::Message msg(L"tdDim");	
	msg.addArgument(intDeviceId);
	msg.addArgument(level);
	return msg.getClientIntegerFromSocket();
}

/**
 * Sends a special learn command to some devices that need a special learn-command
 * to be used from TellStick
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 * @param intDeviceId The device id to learn.
 */
int WINAPI tdLearn(int intDeviceId) {
	TelldusCore::Message msg(L"tdLearn");	
	msg.addArgument(intDeviceId);
	return msg.getClientIntegerFromSocket();
}

/**
 * Returns the last sent command to a specific device
 * @param intDeviceId The device id to query
 * @param methodsSupported The methods supported by the client. See tdMethods() for more information.
 * @returns the last sent command as integer, example TELLSTICK_TURNON or TELLSTICK_TURNOFF
 */
int WINAPI tdLastSentCommand(int intDeviceId, int methodsSupported ) {
	TelldusCore::Message msg(L"tdLastSentCommand");	
	msg.addArgument(intDeviceId);
	msg.addArgument(methodsSupported);
	return msg.getClientIntegerFromSocket();
}

/**
 * If the last sent command it TELLSTICK_DIM this returns the dimmed value.
 * @param intDeviceId The device id to query
 * @returns the the value as a human readable string, example "128" for 50%
 */
char * WINAPI tdLastSentValue( int intDeviceId ) {
	TelldusCore::Message msg(L"tdLastSentValue");	
	msg.addArgument(intDeviceId);
	std::wstring strReturn = msg.getClientWStringFromSocket();
	return wrapStdWstring(strReturn);
}

/**
 * This function returns the number of devices configured
 * @returns an integer of the total number of devices configured
 */
int WINAPI tdGetNumberOfDevices(void){
	
	TelldusCore::Message msg(L"tdGetNumberOfDevices");	
	return msg.getClientIntegerFromSocket();
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
	TelldusCore::Message msg(L"tdGetDeviceId");	
	msg.addArgument(intDeviceIndex);
	return msg.getClientIntegerFromSocket();
}

/**
 * Returns which type the device is. The device could be either
 * TELLSTICK_TYPE_DEVICE or TELLSTICK_TYPE_GROUP
 */
int WINAPI tdGetDeviceType(int intDeviceId) {
	TelldusCore::Message msg(L"tdGetDeviceType");	
	msg.addArgument(intDeviceId);
	return msg.getClientIntegerFromSocket();
}

/**
 * Query a device for it's name.
 * @param intDeviceId The unique id of the device to query
 * @returns The name of the device or an empty string if the device is not found.
 */
char * WINAPI tdGetName(int intDeviceId){
	TelldusCore::Message msg(L"tdGetName");	
	msg.addArgument(intDeviceId);
	std::wstring strReturn = msg.getClientWStringFromSocket();
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
	TelldusCore::Message msg(L"tdSetName");	
	msg.addArgument(intDeviceId);
	msg.addArgument(strNewName);
	return msg.getClientBoolFromSocket();
}

/**
 * @returns the protocol used by a specific device.
 * @param intDeviceId The device id to query.
 */
char* WINAPI tdGetProtocol(int intDeviceId){
	TelldusCore::Message msg(L"tdGetProtocol");	
	msg.addArgument(intDeviceId);
	std::wstring strReturn = msg.getClientWStringFromSocket();
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
	TelldusCore::Message msg(L"tdSetProtocol");	
	msg.addArgument(intDeviceId);
	msg.addArgument(strProtocol);
	return msg.getClientBoolFromSocket();
}

/**
 * @returns the model for a device. Not all protocols uses this.
 * @param intDeviceId The device to query.
 */
char* WINAPI tdGetModel(int intDeviceId){
	TelldusCore::Message msg(L"tdGetModel");	
	msg.addArgument(intDeviceId);
	std::wstring strReturn = msg.getClientWStringFromSocket();
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
	TelldusCore::Message msg(L"tdSetModel");	
	msg.addArgument(intDeviceId);
	msg.addArgument(strModel);
	return msg.getClientBoolFromSocket();
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
	TelldusCore::Message msg(L"tdSetDeviceParameter");	
	msg.addArgument(intDeviceId);
	msg.addArgument(strName);
	msg.addArgument(strValue);
	return msg.getClientBoolFromSocket();
}

/**
 * @returns any protocol specific parameter specified by \c strName
 * @param intDeviceId The device to query.
 * @param strName The name of the parameter to query.
 * @param defaultValue A defaultValue to return if the current parameter hasn't previously been set.
 */
char * WINAPI tdGetDeviceParameter(int intDeviceId, const char *strName, const char *defaultValue){
	TelldusCore::Message msg(L"tdGetDeviceParameter");	
	msg.addArgument(intDeviceId);
	msg.addArgument(strName);
	msg.addArgument(defaultValue);
	std::wstring strReturn = msg.getClientWStringFromSocket();
	return wrapStdWstring(strReturn);
}

/**
 * Add a new device to the global database of devices. This function must be called first before
 * any call to tdSetName(), tdSetProtocol() and similar functions.
 * @returns the new device id for the newly created device. If the creation fails it returnes a 
 * negative value.
 */
int WINAPI tdAddDevice(){
	TelldusCore::Message msg(L"tdAddDevice");	
	return msg.getClientIntegerFromSocket();
}

/**
 * Removes a device.
 * @returns \c true on success, \c false otherwise.
 */
bool WINAPI tdRemoveDevice(int intDeviceId){
	TelldusCore::Message msg(L"tdRemoveDevice");	
	msg.addArgument(intDeviceId);
	return msg.getClientBoolFromSocket();
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
 */
int WINAPI tdMethods(int id, int methodsSupported){
	TelldusCore::Message msg(L"tdMethods");	
	msg.addArgument(id);
	msg.addArgument(methodsSupported);
	return msg.getClientIntegerFromSocket();
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
 * @sa TELLSTICK_ERROR_UNKNOWN
 */
char * WINAPI tdGetErrorString(int intErrorNo) {
	const int numResponses = 8;
	const char *responses[numResponses] = {
		"Success",
		"TellStick not found",
		"Permission denied",
		"Device not found",
		"The method you tried to use is not supported by the device",
		"An error occurred while communicating with TellStick",
		"Could not connect to the Telldus Service",
		"Received an unknown response"
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
	TelldusCore::Message msg(L"tdSendRawCommand");	
	msg.addArgument(command);
	msg.addArgument(reserved);
	return msg.getClientIntegerFromSocket();
}


void WINAPI tdConnectTellStickController(int vid, int pid, const char *serial) {
	TelldusCore::Message msg(L"tdConnectTellStickController");	
	msg.addArgument(vid);
	msg.addArgument(pid);
	msg.addArgument(serial);
	msg.getClientWStringFromSocket();
}

void WINAPI tdDisconnectTellStickController(int vid, int pid, const char *serial) {
	TelldusCore::Message msg(L"tdDisconnectTellStickController");	
	msg.addArgument(vid);
	msg.addArgument(pid);
	msg.addArgument(serial);
	msg.getClientWStringFromSocket();
}


/*\@}*/
