#ifndef PHP_TELLDUS_H
#define PHP_TELLDUS_H 1

#define PHP_TELLDUS_VERSION "1.2.2"
#define PHP_TELLDUS_EXTNAME "telldus"

PHP_MINIT_FUNCTION(telldus);
PHP_MSHUTDOWN_FUNCTION(telldus);
PHP_RINIT_FUNCTION(telldus);

PHP_FUNCTION(telldus_dev_turn_on);
PHP_FUNCTION(telldus_dev_turn_off);
PHP_FUNCTION(telldus_dev_bell);
PHP_FUNCTION(telldus_dev_dim);
PHP_FUNCTION(telldus_dev_methods);

PHP_FUNCTION(telldus_get_number_of_devices);
PHP_FUNCTION(telldus_get_device_id);
PHP_FUNCTION(telldus_get_name);

extern zend_module_entry telldus_module_entry;
#define phpext_telldus_ptr &telldus_module_entry

#endif
