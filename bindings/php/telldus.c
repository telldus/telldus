#ifdef HAVE_CONFIG_H
 #include "config.h"
#endif

#include "php.h"
#include "php_telldus.h"
#include <telldus-core.h>


static function_entry telldus_functions[] = {
	PHP_FE(tdTurnOn, NULL)
	PHP_FE(tdTurnOff, NULL)
	PHP_FE(tdBell, NULL)
	PHP_FE(tdDim, NULL)
	PHP_FE(tdMethods, NULL)

	PHP_FE(tdGetNumberOfDevices, NULL)
	PHP_FE(tdGetDeviceId, NULL)
	PHP_FE(tdGetName, NULL)
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
	REGISTER_LONG_CONSTANT("TELLDUS_TURNON", TELLSTICK_TURNON, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLDUS_TURNOFF", TELLSTICK_TURNOFF, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLDUS_BELL", TELLSTICK_BELL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLDUS_TOGGLE", TELLSTICK_TOGGLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("TELLDUS_DIM", TELLSTICK_DIM, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(telldus)
{
	return SUCCESS;
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

PHP_FUNCTION(tdGetName)
{
	long id;
	char *name;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	name = tdGetName( id );
	RETURN_STRING( name, 1 );
}

