#ifndef PHP_TELLDUS_H
#define PHP_TELLDUS_H 1

#define PHP_TELLDUS_VERSION "2.0.0"
#define PHP_TELLDUS_EXTNAME "telldus"

PHP_MINIT_FUNCTION(telldus);
PHP_MSHUTDOWN_FUNCTION(telldus);
PHP_RINIT_FUNCTION(telldus);

PHP_FUNCTION(tdTurnOn);
PHP_FUNCTION(tdTurnOff);
PHP_FUNCTION(tdBell);
PHP_FUNCTION(tdDim);
PHP_FUNCTION(tdMethods);

PHP_FUNCTION(tdGetNumberOfDevices);
PHP_FUNCTION(tdGetDeviceId);
PHP_FUNCTION(tdGetName);

extern zend_module_entry telldus_module_entry;
#define phpext_telldus_ptr &telldus_module_entry

#endif
