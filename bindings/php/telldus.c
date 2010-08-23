#ifdef HAVE_CONFIG_H
 #include "config.h"
#endif

#include "php.h"
#include "php_telldus.h"
#include <telldus-core.h>


static function_entry telldus_functions[] = {
	PHP_FE(tdInit, NULL)
	PHP_FE(tdClose, NULL)

	PHP_FE(tdTurnOn, NULL)
	PHP_FE(tdTurnOff, NULL)
	PHP_FE(tdBell, NULL)
	PHP_FE(tdDim, NULL)
	PHP_FE(tdLearn, NULL)
	PHP_FE(tdMethods, NULL)
	PHP_FE(tdLastSentCommand, NULL)
	PHP_FE(tdLastSentValue, NULL)

	PHP_FE(tdGetNumberOfDevices, NULL)
	PHP_FE(tdGetDeviceId, NULL)
	PHP_FE(tdGetDeviceType, NULL)

	PHP_FE(tdGetErrorString, NULL)

	PHP_FE(tdGetName, NULL)
	PHP_FE(tdSetName, NULL)
	PHP_FE(tdGetProtocol, NULL)
	PHP_FE(tdSetProtocol, NULL)
	PHP_FE(tdGetModel, NULL)
	PHP_FE(tdSetModel, NULL)

	PHP_FE(tdGetDeviceParameter, NULL)
	PHP_FE(tdSetDeviceParameter, NULL)

	PHP_FE(tdAddDevice, NULL)
	PHP_FE(tdRemoveDevice, NULL)

	PHP_FE(tdSendRawCommand, NULL)

	{NULL, NULL, NULL}
};

zend_module_entry telldus_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_TELLDUS_EXTNAME,
	telldus_functions,
	PHP_MINIT(telldus),
	PHP_MSHUTDOWN(telldus),
	PHP_RINIT(telldus),
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	PHP_TELLDUS_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_TELLDUS
	ZEND_GET_MODULE(telldus)
#endif


PHP_RINIT_FUNCTION(telldus)
{
	return SUCCESS;
}

PHP_MINIT_FUNCTION(telldus)
{
	REGISTER_LONG_CONSTANT("TELLSTICK_TURNON", TELLSTICK_TURNON, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_TURNOFF", TELLSTICK_TURNOFF, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_BELL", TELLSTICK_BELL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_TOGGLE", TELLSTICK_TOGGLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_DIM", TELLSTICK_DIM, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_LEARN", TELLSTICK_LEARN, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("TELLSTICK_SUCCESS", TELLSTICK_SUCCESS, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_ERROR_NOT_FOUND", TELLSTICK_ERROR_NOT_FOUND, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_ERROR_PERMISSION_DENIED", TELLSTICK_ERROR_PERMISSION_DENIED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_ERROR_DEVICE_NOT_FOUND", TELLSTICK_ERROR_DEVICE_NOT_FOUND, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_ERROR_METHOD_NOT_SUPPORTED", TELLSTICK_ERROR_METHOD_NOT_SUPPORTED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_ERROR_COMMUNICATION", TELLSTICK_ERROR_COMMUNICATION, CONST_CS | CONST_PERSISTENT);
	//REGISTER_LONG_CONSTANT("TELLSTICK_ERROR_CONNECTING_SERVICE", TELLSTICK_ERROR_CONNECTING_SERVICE, CONST_CS | CONST_PERSISTENT);
	//REGISTER_LONG_CONSTANT("TELLSTICK_ERROR_UNKNOWN_RESPONSE", TELLSTICK_ERROR_UNKNOWN_RESPONSE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_ERROR_UNKNOWN", TELLSTICK_ERROR_UNKNOWN, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("TELLSTICK_TYPE_DEVICE", TELLSTICK_TYPE_DEVICE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLSTICK_TYPE_GROUP", TELLSTICK_TYPE_GROUP, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(telldus)
{
	return SUCCESS;
}

PHP_FUNCTION(tdInit)
{
	tdInit();
	RETURN_NULL();
}

PHP_FUNCTION(tdClose)
{
	tdClose();
	RETURN_NULL();
}

PHP_FUNCTION(tdTurnOn)
{
	long id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	RETURN_BOOL( tdTurnOn( id ) );
}

PHP_FUNCTION(tdTurnOff)
{
	long id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	RETURN_BOOL( tdTurnOff( id ) );
}

PHP_FUNCTION(tdBell)
{
	long id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	RETURN_BOOL( tdBell( id ) );
}

PHP_FUNCTION(tdDim)
{
	long id;
	long level;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &id, &level) == FAILURE) {
		RETURN_NULL();
	}
	if (level < 0 || level > 255) {
		RETURN_NULL();
	}

	RETURN_BOOL( tdDim( id, level ) );
}

PHP_FUNCTION(tdLearn)
{
	long id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	RETURN_BOOL( tdLearn( id ) );
}

PHP_FUNCTION(tdMethods)
{
	long id;
	long supportedMethods;
	long methods;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &id, &supportedMethods) == FAILURE) {
		RETURN_NULL();
	}

	methods = tdMethods( id, supportedMethods );
	RETURN_LONG(methods);
}

PHP_FUNCTION(tdLastSentCommand)
{
	long id;
	long supportedMethods;
	long command;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &id, &supportedMethods) == FAILURE) {
		RETURN_NULL();
	}

	command = tdLastSentCommand( id, supportedMethods );
	RETURN_LONG(command);
}

PHP_FUNCTION(tdLastSentValue)
{
	long id;
	char *value, *retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	value = tdLastSentValue( id );
	retval = estrdup(value);
	tdReleaseString(value);
	RETURN_STRING(retval, 0);
}

PHP_FUNCTION(tdGetNumberOfDevices)
{
	int nbr = tdGetNumberOfDevices();
	RETURN_LONG(nbr);
}

PHP_FUNCTION(tdGetDeviceId)
{
	long index;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		RETURN_LONG(0);
	}

	RETURN_LONG( tdGetDeviceId( index ) );
}

PHP_FUNCTION(tdGetDeviceType)
{
	long id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_LONG(0);
	}

	RETURN_LONG( tdGetDeviceType( id ) );
}

PHP_FUNCTION(tdGetErrorString)
{
	long errorNo;
	char *errorString, *retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &errorNo) == FAILURE) {
		RETURN_NULL();
	}

	errorString = tdGetErrorString( errorNo );
	retval = estrdup(errorString);
	tdReleaseString(errorString);
	RETURN_STRING( retval, 0 );
}

PHP_FUNCTION(tdGetName)
{
	long id;
	char *name, *retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	name = tdGetName( id );
	retval = estrdup(name);
	tdReleaseString(name);
	RETURN_STRING( retval, 0 );
}

PHP_FUNCTION(tdSetName)
{
	long id;
	char *name;
	int nameLen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &id, &name, &nameLen) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_BOOL( tdSetName( id, name ) );
}

PHP_FUNCTION(tdGetProtocol)
{
	long id;
	char *protocol, *retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	protocol = tdGetProtocol( id );
	retval = estrdup(protocol);
	tdReleaseString(protocol);
	RETURN_STRING( retval, 0 );
}

PHP_FUNCTION(tdSetProtocol)
{
	long id;
	char *protocol;
	int protocolLen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &id, &protocol, &protocolLen) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_BOOL( tdSetProtocol( id, protocol ) );
}

PHP_FUNCTION(tdGetModel)
{
	long id;
	char *model, *retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	model = tdGetModel( id );
	retval = estrdup(model);
	tdReleaseString(model);
	RETURN_STRING( retval, 0 );
}

PHP_FUNCTION(tdSetModel)
{
	long id;
	char *model;
	int modelLen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &id, &model, &modelLen) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_BOOL( tdSetModel( id, model ) );
}

PHP_FUNCTION(tdGetDeviceParameter)
{
	long id;
	char *param, *name, *defaultValue, *retval;
	int nameLen, defaultValueLen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lss", &id, &name, &nameLen, &defaultValue, &defaultValueLen) == FAILURE) {
		RETURN_NULL();
	}

	param = tdGetDeviceParameter( id, name, defaultValue );
	retval = estrdup(param);
	tdReleaseString(param);
	RETURN_STRING( retval, 0 );
}

PHP_FUNCTION(tdSetDeviceParameter)
{
	long id;
	char *name, *value;
	int nameLen, valueLen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lss", &id, &name, &nameLen, &value, &valueLen) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_BOOL( tdSetDeviceParameter( id, name, value ) );
}

PHP_FUNCTION(tdAddDevice)
{
	RETURN_LONG(tdAddDevice());
}

PHP_FUNCTION(tdRemoveDevice)
{
	long deviceId;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &deviceId) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_BOOL(tdRemoveDevice(deviceId));
}

PHP_FUNCTION(tdSendRawCommand)
{
	char *command;
	int commandLen;
	long reserved = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &command, &commandLen, &reserved) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_LONG(tdSendRawCommand(command, reserved));
}
