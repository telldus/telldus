#ifndef PHP_TELLDUS_H
#define PHP_TELLDUS_H 1

#define PHP_TELLDUS_VERSION "2.0.2"
#define PHP_TELLDUS_EXTNAME "telldus"

PHP_MINIT_FUNCTION(telldus);
PHP_MSHUTDOWN_FUNCTION(telldus);
PHP_RINIT_FUNCTION(telldus);

PHP_FUNCTION(tdInit);
PHP_FUNCTION(tdClose);

PHP_FUNCTION(tdTurnOn);
PHP_FUNCTION(tdTurnOff);
PHP_FUNCTION(tdBell);
PHP_FUNCTION(tdDim);
PHP_FUNCTION(tdLearn);
PHP_FUNCTION(tdMethods);
PHP_FUNCTION(tdLastSentCommand);
PHP_FUNCTION(tdLastSentValue);

PHP_FUNCTION(tdGetNumberOfDevices);
PHP_FUNCTION(tdGetDeviceId);
PHP_FUNCTION(tdGetDeviceType);

PHP_FUNCTION(tdGetErrorString);

PHP_FUNCTION(tdGetName);
PHP_FUNCTION(tdSetName);
PHP_FUNCTION(tdGetProtocol);
PHP_FUNCTION(tdSetProtocol);
PHP_FUNCTION(tdGetModel);
PHP_FUNCTION(tdSetModel);

PHP_FUNCTION(tdGetDeviceParameter);
PHP_FUNCTION(tdSetDeviceParameter);

PHP_FUNCTION(tdAddDevice);
PHP_FUNCTION(tdRemoveDevice);

PHP_FUNCTION(tdSendRawCommand);

extern zend_module_entry telldus_module_entry;
#define phpext_telldus_ptr &telldus_module_entry

#endif
