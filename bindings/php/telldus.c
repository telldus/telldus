#ifdef HAVE_CONFIG_H
 #include "config.h"
#endif

#include "php.h"
#include "php_telldus.h"
#include "TellUsbD101.h"


static function_entry telldus_functions[] = {
	PHP_FE(telldus_dev_turn_on, NULL)
	PHP_FE(telldus_dev_turn_off, NULL)
	PHP_FE(telldus_dev_bell, NULL)
	PHP_FE(telldus_dev_dim, NULL)
	PHP_FE(telldus_dev_methods, NULL)

	PHP_FE(telldus_get_number_of_devices, NULL)
	PHP_FE(telldus_get_device_id, NULL)
	PHP_FE(telldus_get_name, NULL)
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

PHP_FUNCTION(telldus_dev_turn_on)
{
	long id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	RETURN_BOOL( devTurnOn( id ) );
}

PHP_FUNCTION(telldus_dev_turn_off)
{
	long id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	RETURN_BOOL( devTurnOff( id ) );
}

PHP_FUNCTION(telldus_dev_bell)
{
	long id;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	RETURN_BOOL( devBell( id ) );
}

PHP_FUNCTION(telldus_dev_dim)
{
	long id;
	long level;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &id, &level) == FAILURE) {
		RETURN_NULL();
	}
	if (level < 0 || level > 255) {
		RETURN_NULL();
	}

	RETURN_BOOL( devDim( id, level ) );
}


PHP_FUNCTION(telldus_dev_methods)
{
	long id;
	long methods;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	methods = devMethods( id );
	RETURN_LONG(methods);
}

PHP_FUNCTION(telldus_get_number_of_devices)
{
	int nbr = devGetNumberOfDevices();
	RETURN_LONG(nbr);
}

PHP_FUNCTION(telldus_get_device_id)
{
	long index;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
		RETURN_LONG(0);
	}

	RETURN_LONG( devGetDeviceId( index ) );
}

PHP_FUNCTION(telldus_get_name)
{
	long id;
	char *name;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) {
		RETURN_NULL();
	}

	name = devGetName( id );
	RETURN_STRING( name, 1 );
}
