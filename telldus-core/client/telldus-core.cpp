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
 * @name Device method flags
 * Flags for the different methods/commands a device can support. Can be used
 * as bitflags in e.g. tdMethods().
 * @{
 *
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
 **//* @} */

/**
 * @name Device types
 * The device type as returned from tdGetDeviceType().
 * @{
 *
 * @def TELLSTICK_TYPE_DEVICE
 * Device type of a single device.
 *
 * @def TELLSTICK_TYPE_GROUP
 * Device type of a device which contains other devices.
 *
 * @def TELLSTICK_TYPE_SCENE
 * Device type of a device which contains other devices that are sent different
 * commands when executed (i.e. one device turns on, another turns off).
 *
 **//* @} */

/**
 * @name Sensor value types
 * The supported sensor value types are returned from tdSensor() and used when
 * querying a sensor for a specific value in tdSensorValue().
 * @{
 *
 * @def TELLSTICK_TEMPERATURE
 * The sensor can report the temperature.
 *
 * @def TELLSTICK_HUMIDITY
 * The sensor can report the humidity.
 *
 **//* @} */

/**
 * @name Controller type
 * The controller type as returned from tdController().
 * @{
 *
 * @def TELLSTICK_CONTROLLER_TELLSTICK
 * The controller is a TellStick.
 *
 * @def TELLSTICK_CONTROLLER_TELLSTICK_DUO
 * The controller is a TellStick Duo.
 *
 * @def TELLSTICK_CONTROLLER_TELLSTICK_NET
 * The controller is a TellStick Net.
 *
 **//* @} */

/**
 * @name Device changes
 * Flags used in event callbacks.
 * @{
 *
 * @def TELLSTICK_DEVICE_ADDED
 * A new device was added.
 *
 * @def TELLSTICK_DEVICE_CHANGED
 * A device was changed.
 *
 * @def TELLSTICK_DEVICE_REMOVED
 * A device was removed.
 *
 * @def TELLSTICK_DEVICE_STATE_CHANGED
 * The state changed for a device.
 *
 **//* @} */

/**
 * @name Change types
 * Flags used in event callbacks.
 * @{
 *
 * @def TELLSTICK_CHANGE_NAME
 * The name has changed.
 *
 * @def TELLSTICK_CHANGE_PROTOCOL
 * The protocol has changed.
 *
 * @def TELLSTICK_CHANGE_MODEL
 * The model has changed.
 *
 * @def TELLSTICK_CHANGE_METHOD
 * The method has changed.
 *
 * @def TELLSTICK_CHANGE_AVAILABLE
 * The controller is now available or unavailable.
 *
 * @def TELLSTICK_CHANGE_FIRMWARE
 * The firmware has changed.
 *
 **//* @} */

/**
 * @name Error codes
 * The error codes returned from all API functions returning int.
 * @{
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
 * @def TELLSTICK_ERROR_SYNTAX
 * Error code. Input/command could not be parsed or didn't follow
 * input rules.
 *
 * @def TELLSTICK_ERROR_BROKEN_PIPE
 * Error code. Pipe broken during communication.
 *
 * @def TELLSTICK_ERROR_COMMUNICATING_SERVICE
 * Error code. Timeout waiting for response from the Telldus Service.
 *
 * @def TELLSTICK_ERROR_UNKNOWN
 * Error code. An unkown error has occurred.
 **//* @} */

/**
 * @name Callback typedefs
 * @{
 *
 * @typedef TDDeviceEvent
 * The callback type for device events.
 *
 * @attention The callback will be called by another thread than the thread
 * used by the application and some measures must be taken to synchronize it
 * with the main thread.
 *
 * @param deviceId The id of the device that changed.
 * @param method The new device state. Can be @ref TELLSTICK_TURNON, @ref
 * TELLSTICK_TURNOFF, etc.
 * @param data If @a method is @ref TELLSTICK_DIM this holds the current value
 * as a human readable string, example "128" for 50%.
 * @param callbackId The id of the callback.
 * @param context The pointer passed when registering for the event.
 *
 * @sa tdRegisterDeviceEvent
 *
 *
 * @typedef TDDeviceChangeEvent
 * The callback type for device change events.
 *
 * @attention The callback will be called by another thread than the thread
 * used by the application and some measures must be taken to synchronize it
 * with the main thread.
 *
 * @param deviceId The id of the device that was added, changed or removed.
 * @param changeEvent One of the constants @ref TELLSTICK_DEVICE_ADDED, @ref
 * TELLSTICK_DEVICE_CHANGED or @ref TELLSTICK_DEVICE_REMOVED.
 * @param changeType If @a changeEvent is @ref TELLSTICK_DEVICE_CHANGED, this
 * parameter indicates what has changed (e.g @ref TELLSTICK_CHANGE_NAME, @ref
 * TELLSTICK_CHANGE_PROTOCOL, @ref TELLSTICK_CHANGE_MODEL or @ref
 * TELLSTICK_CHANGE_METHOD).
 * @param callbackId The id of the callback.
 * @param context The pointer passed when registering for the event.
 *
 * @sa tdRegisterDeviceChangeEvent
 *
 *
 * @typedef TDRawDeviceEvent
 * The callback type for raw events.
 *
 * @attention The callback will be called by another thread than the thread
 * used by the application and some measures must be taken to synchronize it
 * with the main thread.
 *
 * @param data Raw device data.
 * @param controllerId Id of receiving controller.
 * @param callbackId The id of the callback.
 * @param context The pointer passed when registering for the event.
 *
 * @sa tdRegisterRawDeviceEvent
 *
 *
 * @typedef TDSensorEvent
 * The callback type for sensor events.
 *
 * @attention The callback will be called by another thread than the thread
 * used by the application and some measures must be taken to synchronize it
 * with the main thread.
 *
 * @param protocol The sensor's protocol.
 * @param model The model of the sensor.
 * @param id The unique id for the sensor.
 * @param dataType The type that @a value is. Can be one of @ref
 * TELLSTICK_TEMPERATURE or @ref TELLSTICK_HUMIDITY.
 * @param value A human readable string of the data.
 * @param timestamp The time (as returned by the time() system call) when the
 * value was received.
 * @param callbackId The id of the callback.
 * @param context The pointer passed when registering for the event.
 *
 * @sa tdRegisterSensorEvent
 *
 *
 * @typedef TDControllerEvent
 * The callback type for controller events.
 *
 * @attention The callback will be called by another thread than the thread
 * used by the application and some measures must be taken to synchronize it
 * with the main thread.
 *
 * @param controllerId The id of the controller that was added, changed or
 * removed.
 * @param changeEvent One of the constants @ref TELLSTICK_DEVICE_ADDED, @ref
 * TELLSTICK_DEVICE_CHANGED, @ref TELLSTICK_DEVICE_STATE_CHANGED or @ref
 * TELLSTICK_DEVICE_REMOVED.

 * @param changeType If @a changeEvent is:
 *  @arg @ref TELLSTICK_DEVICE_ADDED this is the controller's type (e.g. @ref
 *  TELLSTICK_CONTROLLER_TELLSTICK or @ref TELLSTICK_CONTROLLER_TELLSTICK_DUO),
 *  @arg @ref TELLSTICK_DEVICE_CHANGED this indicates what has changed
 *  (e.g. @ref TELLSTICK_CHANGE_NAME or @ref TELLSTICK_CHANGE_FIRMWARE),
 *  @arg @ref TELLSTICK_DEVICE_STATE_CHANGED this indicates which state that
 *  has changed (e.g. @ref TELLSTICK_CHANGE_AVAILABLE),
 *  @arg @ref TELLSTICK_DEVICE_REMOVED this is unused.
 * @param newValue If @a changeEvent is:
 *  @arg @ref TELLSTICK_DEVICE_CHANGED this is the property's new value.
 *  @arg @ref TELLSTICK_DEVICE_STATE_CHANGED this is the new state. For @ref
 *  TELLSTICK_CHANGE_AVAILABLE this is either 0 or 1.
 * @param callbackId The id of the callback.
 * @param context The pointer passed when registering for the event.
 *
 * @sa tdRegisterControllerEvent
 *
 **//* @} */

/**
 * This function initiates the library. Call this function before any other
 * call to a function in telldus-core.
 *
 * @since Version 2.0.0
 **/
void WINAPI tdInit(void) {
	Client::getInstance(); //Create the manager-object
}

/**
 * Register a callback that will receive device events.
 *
 * @param eventFunction Callback function
 * @param context Pointer that will be passed back in the callback.
 *
 * @returns An id identifying the callback. Pass this id to
 * tdUnregisterCallback() to stop receiving callbacks.
 *
 * @sa @ref sec_events_registering
 * @since Version 2.0.0
 **/
int WINAPI tdRegisterDeviceEvent( TDDeviceEvent eventFunction, void *context ) {
	eventFunction;	Client *client = Client::getInstance();
	return client->registerEvent( CallbackStruct::DeviceEvent, (void *)eventFunction, context );
}

/**
 * Register a callback that will receive raw (i.e. all) events.
 *
 * @param eventFunction Callback function
 * @param context Pointer that will be passed back in the callback.
 *
 * @returns An id identifying the callback. Pass this id to
 * tdUnregisterCallback() to stop receiving callbacks.
 *
 * @sa @ref sec_events_registering
 * @since Version 2.0.0
 **/
int WINAPI tdRegisterRawDeviceEvent( TDRawDeviceEvent eventFunction, void *context ) {
	Client *client = Client::getInstance();
	return client->registerEvent( CallbackStruct::RawDeviceEvent, (void *)eventFunction, context );
}

/**
 * Register a callback that will receive device change events. This includes
 * addition of new devices, changes of device properties (e.g. name) and
 * removal of existing devices.
 *
 * @param eventFunction Callback function
 * @param context Pointer that will be passed back in the callback.
 *
 * @returns An id identifying the callback. Pass this id to
 * tdUnregisterCallback() to stop receiving callbacks.
 *
 * @sa @ref sec_events_registering
 * @since Version 2.1.0
 **/
int WINAPI tdRegisterDeviceChangeEvent( TDDeviceChangeEvent eventFunction, void *context) {
	Client *client = Client::getInstance();
	return client->registerEvent( CallbackStruct::DeviceChangeEvent, (void *)eventFunction, context );
}

/**
 * Register a callback that will receive sensor events.
 *
 * @param eventFunction Callback function
 * @param context Pointer that will be passed back in the callback.
 *
 * @returns An id identifying the callback. Pass this id to
 * tdUnregisterCallback() to stop receiving callbacks.
 *
 * @sa @ref sec_events_registering
 * @since Version 2.1.0
 **/
int WINAPI tdRegisterSensorEvent( TDSensorEvent eventFunction, void *context) {
	Client *client = Client::getInstance();
	return client->registerEvent( CallbackStruct::SensorEvent, (void *)eventFunction, context );
}

/**
 * Register a callback that will receive controller events.
 *
 * @param eventFunction Callback function
 * @param context Pointer that will be passed back in the callback.
 *
 * @returns An id identifying the callback. Pass this id to
 * tdUnregisterCallback() to stop receiving callbacks.
 *
 * @sa @ref sec_events_registering
 * @since Version 2.1.2
 **/
int WINAPI tdRegisterControllerEvent( TDControllerEvent eventFunction, void *context) {
	Client *client = Client::getInstance();
	return client->registerEvent( CallbackStruct::ControllerEvent, (void *)eventFunction, context );
}

/**
 * Unregister a callback.
 *
 * @param callbackId The returned id from one of the tdRegister* functions.
 *
 * @returns 1 on success, or 0 on failure.
 *
 * @sa @ref sec_events_registering
 * @since Version 2.1.0
 **/
int WINAPI tdUnregisterCallback( int callbackId ) {
	Client *client = Client::getInstance();
	return client->unregisterCallback( callbackId );
}

/**
 * Close the library and clean up the cache it uses.
 * This should be called when the library is not supposed to be used anymore
 *
 * @since Version 2.0.0
 **/
void WINAPI tdClose(void) {
	Client::close();
}

/**
 * This method releases resources allocated by telldus-core.
 * It should be called on the returned value from all functions returning
 * <tt>char *</tt>.
 *
 * @param string A string returned from a td* function.
 *
 * @since Version 2.0.1
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
 *
 * @param intDeviceId The device id to turn on.
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.0.0
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
 *
 * @param intDeviceId The device id to turn off.
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.0.0
 **/
int WINAPI tdTurnOff(int intDeviceId){
	Message msg(L"tdTurnOff");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Sends bell command to devices supporting this.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 *
 * @param intDeviceId The device id to send bell to
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.0.0
 **/
int WINAPI tdBell(int intDeviceId){
	Message msg(L"tdBell");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Dims a device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 *
 * @param intDeviceId The device id to dim
 * @param level The level the device should dim to. This value should be 0-255
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.0.0
 **/
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
 *
 * @param intDeviceId The id to execute
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.1.0
 **/
int WINAPI tdExecute(int intDeviceId){
	Message msg(L"tdExecute");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Send "up" command to device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 *
 * @param intDeviceId The device id to send the command to
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.1.0
 **/
int WINAPI tdUp(int intDeviceId){
	Message msg(L"tdUp");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Send "down" command to device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 *
 * @param intDeviceId The device id to send the command to
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.1.0
 **/
int WINAPI tdDown(int intDeviceId){
	Message msg(L"tdDown");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Send "stop" command to device.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 *
 * @param intDeviceId The device id to stop
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.1.0
 */
int WINAPI tdStop(int intDeviceId){
	Message msg(L"tdStop");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Sends a special learn command to some devices that need a special
 * learn-command to be used from TellStick.
 * Make sure the device supports this by calling tdMethods() before any
 * call to this function.
 *
 * @param intDeviceId The device id to learn.
 *
 * @returns @ref TELLSTICK_SUCCESS on success or appropriate error code on
 * failure.
 *
 * @since Version 2.0.0
 **/
int WINAPI tdLearn(int intDeviceId) {
	Message msg(L"tdLearn");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Returns the last sent command to a specific device
 *
 * @param intDeviceId The device id to query
 * @param methodsSupported The methods supported by the client. See tdMethods()
 * for more information.
 *
 * @returns The last sent command as integer, example @ref TELLSTICK_TURNON or
 * @ref TELLSTICK_TURNOFF
 *
 * @since Version 2.0.0
 **/
int WINAPI tdLastSentCommand(int intDeviceId, int methodsSupported ) {
	Message msg(L"tdLastSentCommand");
	msg.addArgument(intDeviceId);
	msg.addArgument(methodsSupported);
	return Client::getIntegerFromService(msg);
}

/**
 * If the last sent command it @ref TELLSTICK_DIM this returns the dimmed
 * value.
 *
 * @param intDeviceId The device id to query
 *
 * @returns The the value as a human readable string, example "128" for
 * 50%. The returned string must be freed by calling tdReleaseString().
 *
 * @since Version 2.0.0
 **/
char * WINAPI tdLastSentValue( int intDeviceId ) {
	Message msg(L"tdLastSentValue");
	msg.addArgument(intDeviceId);
	std::wstring strReturn = Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * This function returns the number of devices configured.
 *
 * @returns An integer of the total number of devices configured
 *
 * @since Version 2.0.0
 **/
int WINAPI tdGetNumberOfDevices(void){
	return Client::getIntegerFromService(Message(L"tdGetNumberOfDevices"));
}

/**
 * This function returns the unique id of a device with a specific index.
 * To get all the id numbers you should loop over all the devices:
 * @code
 * int intNumberOfDevices = tdGetNumberOfDevices();
 * for (int i = 0; i < intNumberOfDevices; i++) {
 *   int id = tdGetDeviceId( i );
 *   // id now contains the id number of the device with index of i
 * }
 * @endcode
 *
 * @param intDeviceIndex The device index to query. The index starts from 0.
 *
 * @returns The unique id for the device or -1 if the device is not found.
 *
 * @since Version 2.0.0
 **/
int WINAPI tdGetDeviceId(int intDeviceIndex){
	Message msg(L"tdGetDeviceId");
	msg.addArgument(intDeviceIndex);
	return Client::getIntegerFromService(msg);
}

/**
 * Get the device type.
 *
 * @param intDeviceId The unique id of the device to query
 *
 * @returns Which type the device is. The device could be either @ref
 * TELLSTICK_TYPE_DEVICE, @ref TELLSTICK_TYPE_GROUP or @ref
 * TELLSTICK_TYPE_SCENE
 *
 * @since Version 2.0.0
 **/
int WINAPI tdGetDeviceType(int intDeviceId) {
	Message msg(L"tdGetDeviceType");
	msg.addArgument(intDeviceId);
	return Client::getIntegerFromService(msg);
}

/**
 * Query a device for it's name.
 *
 * @param intDeviceId The unique id of the device to query
 *
 * @returns The name of the device or an empty string if the device is not
 * found. The returned string must be freed by calling tdReleaseString().
 *
 * @since Version 2.0.0
 **/
char * WINAPI tdGetName(int intDeviceId){
	Message msg(L"tdGetName");
	msg.addArgument(intDeviceId);
	std::wstring strReturn =  Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * Sets a new name for a device. The devices are global for all application,
 * changing this will change the name for other applications aswell.
 *
 * @param intDeviceId The device id to change the name for
 * @param strNewName The new name for the devices
 *
 * @returns @c true on success, @c false otherwise.
 *
 * @since Version 2.0.0
 **/
bool WINAPI tdSetName(int intDeviceId, const char* strNewName){
	Message msg(L"tdSetName");
	msg.addArgument(intDeviceId);
	msg.addArgument(strNewName);
	return Client::getBoolFromService(msg);
}

/**
 * Get the device protocol.
 * @param intDeviceId The device id to query.
 *
 * @returns The protocol used by a specific device. The returned string must be
 * freed by calling tdReleaseString().
 *
 * @since Version 2.0.0
 **/
char* WINAPI tdGetProtocol(int intDeviceId){
	Message msg(L"tdGetProtocol");
	msg.addArgument(intDeviceId);
	std::wstring strReturn =  Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * This changes the current protocol used by a device. After changing the
 * protocol, setting new parameters is required.
 *
 * @param intDeviceId The device to change.
 * @param strProtocol The new protocol to use.
 *
 * @returns @c true on success, @c false otherwise.
 *
 * @sa tdSetModel()
 * @sa tdSetDeviceParameter()
 *
 * @since Version 2.0.0
 **/
bool WINAPI tdSetProtocol(int intDeviceId, const char* strProtocol){
	Message msg(L"tdSetProtocol");
	msg.addArgument(intDeviceId);
	msg.addArgument(strProtocol);
	return Client::getBoolFromService(msg);
}

/**
 * Get the device model.
 *
 * @param intDeviceId The device to query.
 *
 * @returns The model for a device. Not all protocols uses this. The returned
 * string must be freed by calling tdReleaseString().
 *
 * @since Version 2.0.0
 **/
char* WINAPI tdGetModel(int intDeviceId){
	Message msg(L"tdGetModel");
	msg.addArgument(intDeviceId);
	std::wstring strReturn = Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * Sets a new model for a device. Which model to set depends on the
 * current protocol.
 *
 * @param intDeviceId The device to change
 * @param strModel The new model
 *
 * @returns @c true on success, @c false otherwise.
 *
 * @since Version 2.0.0
 **/
bool WINAPI tdSetModel(int intDeviceId, const char *strModel){
	Message msg(L"tdSetModel");
	msg.addArgument(intDeviceId);
	msg.addArgument(strModel);
	return Client::getBoolFromService(msg);
}

/**
 * Sets a new protocol specific parameter. Please see the documentation of the
 * protocols before setting any parameter.
 *
 * @param intDeviceId The device to change.
 * @param strName The parameter to change.
 * @param strValue The new value for the parameter.
 *
 * @returns @c true on success, @c false otherwise.
 *
 * @since Version 2.0.0
 **/
bool WINAPI tdSetDeviceParameter(int intDeviceId, const char *strName, const char *strValue){
	Message msg(L"tdSetDeviceParameter");
	msg.addArgument(intDeviceId);
	msg.addArgument(strName);
	msg.addArgument(strValue);
	return Client::getBoolFromService(msg);
}

/**
 * Get a protocol specific parameter for a device.
 *
 * @param intDeviceId The device to query.
 * @param strName The name of the parameter to query.
 * @param defaultValue A default value to return if the current parameter
 * hasn't previously been set.
 *
 * @returns The protocol specific parameter specified by @a strName. The
 * returned string must be freed by calling tdReleaseString().
 *
 * @since Version 2.0.0
 **/
char * WINAPI tdGetDeviceParameter(int intDeviceId, const char *strName, const char *defaultValue){
	Message msg(L"tdGetDeviceParameter");
	msg.addArgument(intDeviceId);
	msg.addArgument(strName);
	msg.addArgument(defaultValue);
	std::wstring strReturn = Client::getWStringFromService(msg);
	return wrapStdWstring(strReturn);
}

/**
 * Add a new device to the global database of devices. This function must be
 * called first before any call to tdSetName(), tdSetProtocol() and similar
 * functions.
 *
 * @returns The new device id for the newly created device. If the creation
 * fails it returnes an error (i.e. negative) value.
 *
 * @since Version 2.0.0
 **/
int WINAPI tdAddDevice(){
	Message msg(L"tdAddDevice");
	return Client::getIntegerFromService(msg);
}

/**
 * Removes a device.
 *
 * @param intDeviceId The device to query.
 *
 * @returns @c true on success, @c false otherwise.
 *
 * @since Version 2.0.0
 **/
bool WINAPI tdRemoveDevice(int intDeviceId){
	Message msg(L"tdRemoveDevice");
	msg.addArgument(intDeviceId);
	return Client::getBoolFromService(msg);
}

/**
 * Query a device for which methods it supports. By supplying the methods you
 * support the library could remap the methods a device support for better fit
 * the application.
 *
 * Example of querying a device supporting TELLSTICK_BELL:
 * @code
 * int methods = tdMethods(
 *   id, TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_BELL);
 * // methods is now TELLSTICK_BELL
 * int methods = tdMethods(idm TELLSTICK_TURNON | TELLSTICK_TURNOFF);
 * // methods is now TELLSTICK_TURNON because the client application doesn't
 * // support TELLSTICK_BELL
 * @endcode
 *
 * @param id The device id to query
 * @param methodsSupported The methods the client application supports
 *
 * @returns The method-flags OR'ed into an integer.
 *
 * @sa TELLSTICK_TURNON
 * @sa TELLSTICK_TURNOFF
 * @sa TELLSTICK_BELL
 * @sa TELLSTICK_TOGGLE
 * @sa TELLSTICK_DIM
 * @sa TELLSTICK_EXECUTE
 * @sa TELLSTICK_UP
 * @sa TELLSTICK_DOWN
 * @sa TELLSTICK_STOP
 *
 * @since Version 2.0.0
 **/
int WINAPI tdMethods(int id, int methodsSupported){
	Message msg(L"tdMethods");
	msg.addArgument(id);
	msg.addArgument(methodsSupported);
	return Client::getIntegerFromService(msg);
}

/**
 * Get a human readable string from an error code returned from a function in
 * telldus-core.
 *
 * @param intErrorNo The error code to translate.
 *
 * @returns A string ready to show to the user. The returned string must be
 * freed by calling tdReleaseString().
 *
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
 *
 * @since Version 2.0.0
 **/
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
 *
 * @param command The command for TellStick in its native format
 * @param reserved Reserved for future use
 *
 * @returns @ref TELLSTICK_SUCCESS on success or one of the errorc odes on
 * failure
 *
 * @since Version 2.0.0
 **/
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

/**
 * Connect a TellStick controller.
 *
 * @param vid The vendor id
 * @param pid The product id
 * @param serial The serial
 *
 * @since Version 2.1.0
 **/
void WINAPI tdConnectTellStickController(int vid, int pid, const char *serial) {
	Message msg(L"tdConnectTellStickController");
	msg.addArgument(vid);
	msg.addArgument(pid);
	msg.addArgument(serial);
	Client::getWStringFromService(msg);
}

/**
 * Disconnect a TellStick controller.
 *
 * @param vid The vendor id
 * @param pid The product id
 * @param serial The serial
 *
 * @since Version 2.1.0
 **/
void WINAPI tdDisconnectTellStickController(int vid, int pid, const char *serial) {
	Message msg(L"tdDisconnectTellStickController");
	msg.addArgument(vid);
	msg.addArgument(pid);
	msg.addArgument(serial);
	Client::getWStringFromService(msg);
}

/**
 * Use this function to iterate over all sensors. Iterate until @ref
 * TELLSTICK_SUCCESS is not returned
 *
 * @param protocol A byref string where the protocol of the sensor will be
 * placed
 * @param protocolLen The length of the @a protocol parameter
 * @param model A byref string where the model of the sensor will be placed
 * @param modelLen The length of the @a model parameter
 * @param id A byref int where the id of the sensor will be placed
 * @param dataTypes A byref int with flags for the supported sensor values
 *
 * @returns @ref TELLSTICK_SUCCESS if there is more sensors to be fetched
 *
 * @since Version 2.1.0
 */
int WINAPI tdSensor(char *protocol, int protocolLen, char *model, int modelLen, int *id, int *dataTypes) {
	Client *client = Client::getInstance();
	return client->getSensor(protocol, protocolLen, model, modelLen, id, dataTypes);
}

/**
 * Get one of the supported sensor values from a sensor. Make sure it support
 * the value type first by calling tdSensor(). The triplet @a protocol,
 * @a model and @a id together identifies a sensor.
 *
 * @param protocol The protocol for the sensor
 * @param model The model for the sensor
 * @param id The id of the sensor
 * @param dataType One of the datatype to retrieve
 * @param value A byref string where the value will be places
 * @param len The length of the @a value parameter
 * @param timestamp A byref int where the timestamp of the value will be placed
 *
 * @returns @ref TELLSTICK_SUCCESS if the value could be fetched or one of the
 * error codes on failure
 *
 * @since Version 2.1.0
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

/**
 * Use this function to iterate over all controllers. Iterate until
 * @ref TELLSTICK_SUCCESS is not returned
 *
 * @param controllerId A byref int where the id of the controller will be placed
 * @param controllerType A byref int where the type of the controller will be placed
 * @param name A byref string where the name of the controller will be placed
 * @param nameLen The length of the @a name parameter
 * @param available A byref int if the controller is currently available or maybe disconnected
 * @returns TELLSTICK_SUCCESS if there is more sensors to be fetched
 * @sa TELLSTICK_CONTROLLER_TELLSTICK
 * @sa TELLSTICK_CONTROLLER_TELLSTICK_DUO
 * @sa TELLSTICK_CONTROLLER_TELLSTICK_NET
 *
 * @since Version 2.1.2
 **/
int WINAPI tdController(int *controllerId, int *controllerType, char *name, int nameLen, int *available) {
	Client *client = Client::getInstance();
	return client->getController(controllerId, controllerType, name, nameLen, available);
}

/**
 * This function gets a parameter on a controller.
 * Valid parameters are: @c serial and @c firmware
 *
 * @param controllerId The controller to change
 * @param name The parameter to get.
 * @param value A byref string where the value of the parameter will be placed
 * @param valueLen The length of the @a value parameter
 *
 * @returns @ref TELLSTICK_SUCCESS on success, or an error code on failure.
 *
 * @since Version 2.1.2
 **/
int WINAPI tdControllerValue(int controllerId, const char *name, char *value, int valueLen) {
	Message msg(L"tdControllerValue");
	msg.addArgument(controllerId);
	msg.addArgument(name);
	std::wstring retval = Client::getWStringFromService(msg);
	if (retval.length() == 0) {
		return TELLSTICK_ERROR_METHOD_NOT_SUPPORTED;
	}

	if (value && valueLen) {
		strncpy(value, TelldusCore::wideToString(retval).c_str(), valueLen);
	}
	return TELLSTICK_SUCCESS;
}

/**
 * This function sets a parameter on a controller.
 * Valid parameters are: @c name
 *
 * @param controllerId The controller to change
 * @param name The parameter to change.
 * @param value The new value for the parameter.
 *
 * @returns @ref TELLSTICK_SUCCESS on success, or an error code on failure.
 *
 * @since Version 2.1.2
 **/
int WINAPI tdSetControllerValue(int controllerId, const char *name, const char *value) {
	Message msg(L"tdSetControllerValue");
	msg.addArgument(controllerId);
	msg.addArgument(name);
	msg.addArgument(value);
	return Client::getIntegerFromService(msg);
}

/**
 * This function removes a controller from the list of controllers. The
 * controller must not be available (disconnected) for this to work.
 *
 * @param controllerId The controller to remove
 *
 * @returns @ref TELLSTICK_SUCCESS if the controller was removed, @ref
 * TELLSTICK_ERROR_NOT_FOUND if the controller was not found, and @ref
 * TELLSTICK_ERROR_PERMISSION_DENIED if the controller is still connected.
 *
 * @since Version 2.1.2
 **/
int WINAPI tdRemoveController(int controllerId) {
	Message msg(L"tdRemoveController");
	msg.addArgument(controllerId);
	return Client::getIntegerFromService(msg);
}

/* @} */
