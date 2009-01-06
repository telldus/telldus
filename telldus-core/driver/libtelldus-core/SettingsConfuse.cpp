//
// C++ Implementation: telldussettingsconfuse
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "Settings.h"
#include <confuse.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

class privateVars {
public:
	cfg_t *cfg;
};

bool readConfig(cfg_t **cfg);

const char* CONFIG_FILE = "/etc/tellstick.conf";

/*
* Constructor
*/
Settings::Settings(void)
{
	d = new privateVars();
	readConfig(&d->cfg);
}

/*
* Destructor
*/
Settings::~Settings(void)
{
	if (d->cfg > 0)
		cfg_free(d->cfg);
	delete d;
}

/*
* Return a setting
*/
const char *Settings::getSetting(const char *strName) {
	if (d->cfg > 0) {
		return cfg_getstr(d->cfg, strName);
	}
	return "";
}

/*
* Return the number of stored devices
*/
int Settings::getNumberOfDevices(void){
	if (d->cfg > 0) {
		return cfg_size(d->cfg, "device");
	}
	return 0;
}

int Settings::getDeviceId(int intDeviceIndex){
	if (intDeviceIndex >= getNumberOfDevices()) { //Out of bounds
		return -1;
	}
	cfg_t *cfg_device = cfg_getnsec(d->cfg, "device", intDeviceIndex);
	int id = cfg_getint(cfg_device, "id");
	return id;
}

/*
* Add a new device
*/
int Settings::addDevice(){
	int intDeviceId = getNextDeviceId();

	FILE *fp = fopen(CONFIG_FILE, "w");
	cfg_print(d->cfg, fp); //Print the config-file
	fprintf(fp, "device {\n  id=%d\n}\n", intDeviceId); //Print the new device
	fclose(fp);
	
	//Re-read config-file
	cfg_free(d->cfg);
	readConfig(&d->cfg);
	return intDeviceId;
}

/*
* Get next available device id
*/
int Settings::getNextDeviceId(){
	int intDeviceId = 0;
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") >= intDeviceId)  {
			intDeviceId = cfg_getint(cfg_device, "id");
		}
	}
	intDeviceId++;
	return intDeviceId;
}

/*
* Remove a device
*/
bool Settings::removeDevice(int intDeviceId){
	bool blnSuccess = true;
	FILE *fp = fopen(CONFIG_FILE, "w");
	
	// Print all opts
	for(int i = 0; d->cfg->opts[i].name; i++) {
		
		// Check if it isn't a device section
		if (strcmp(d->cfg->opts[i].name, "device") != 0) {
			cfg_opt_print(&d->cfg->opts[i], fp);
		} else {
			// Print all sections except the one to remove
			cfg_t *cfg_device;
			for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
				cfg_device = cfg_getnsec(d->cfg, "device", i);
				if (cfg_getint(cfg_device, "id") != intDeviceId) { //This isn't the one to skip
					fprintf(fp, "device {\n");
					cfg_print_indent(cfg_device, fp, 1);
					fprintf(fp, "}\n");
				}
			}
		}
	}
	fclose(fp);
	
	//Re-read config-file
	cfg_free(d->cfg);
	readConfig(&d->cfg);
	
	return blnSuccess;
}

char *Settings::getStringSetting(int intDeviceId, const char* name, bool parameter) {
	if (d->cfg == 0) {
		return NULL;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			if (parameter) {
				cfg_device = cfg_getsec(cfg_device, "parameters");
			}
			const char *strSetting = cfg_getstr(cfg_device, name);
			if (strSetting == NULL) {
				return NULL;
			}
			char *strReturn = (char *)malloc((strlen(strSetting)+1) * sizeof(char));
			strcpy(strReturn, strSetting);
			return strReturn;
		}
	}
	return NULL;
}

bool Settings::setStringSetting(int intDeviceId, const char* name, const char *value, bool parameter) {
	if (d->cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			if (parameter) {
				cfg_t *cfg_parameters = cfg_getsec(cfg_device, "parameters");
				cfg_setstr(cfg_parameters, name, value);
			} else {
				cfg_setstr(cfg_device, name, value);
			}
			FILE *fp = fopen(CONFIG_FILE, "w");
			cfg_print(d->cfg, fp);
			fclose(fp);
			return true;
		}
	}
	return false;
}

int Settings::getIntSetting(int intDeviceId, const char* name, bool parameter) {
	if (d->cfg == 0) {
		return 0;
	}
	cfg_t *cfg_device;
	for(int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId) {
			if (parameter) {
				cfg_device = cfg_getsec(cfg_device, "parameters");
			}
			return cfg_getint(cfg_device, name);
		}
	}
	return 0;
}

bool Settings::setIntSetting(int intDeviceId, const char* name, int value, bool parameter) {
	if (d->cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			if (parameter) {
				cfg_t *cfg_parameters = cfg_getsec(cfg_device, "parameters");
				cfg_setint(cfg_parameters, name, value);
			} else {
				cfg_setint(cfg_device, name, value);
			}
			FILE *fp = fopen(CONFIG_FILE, "w");
			cfg_print(d->cfg, fp);
			fclose(fp);
			return true;
		}
	}
	return false;
}


bool readConfig(cfg_t **cfg) {
	cfg_opt_t controller_opts[] = {
		CFG_INT("id", -1, CFGF_NONE),
	};
	
	cfg_opt_t device_parameter_opts[] = {
		//Nexa
		CFG_STR("nexa_house", 0, CFGF_NONE),
		CFG_STR("nexa_unit", 0, CFGF_NONE),
		
		//Sartano
		CFG_STR("sartano_code", 0, CFGF_NONE),
		
		//Ikea
		CFG_STR("ikea_system", 0, CFGF_NONE),
		CFG_STR("ikea_units", 0, CFGF_NONE),
		CFG_STR("ikea_fade", 0, CFGF_NONE),
		
		CFG_END()
	};
	
	cfg_opt_t device_opts[] = {
		CFG_INT("id", -1, CFGF_NONE),
		CFG_STR("name", "Unnamed", CFGF_NONE),
		CFG_INT("controller", 0, CFGF_NONE),
		CFG_STR("protocol", "arctech", CFGF_NONE),
		CFG_INT("model", 1, CFGF_NONE),
		CFG_SEC("parameters", device_parameter_opts, CFGF_NONE),
		CFG_END()
	};
	
	cfg_opt_t opts[] = {
		CFG_STR("deviceNode", "/dev/tellstick", CFGF_NONE),
		CFG_SEC("device", device_opts, CFGF_MULTI),
		CFG_END()
	};
	
	(*cfg) = cfg_init(opts, CFGF_NOCASE);
	if (cfg_parse((*cfg), CONFIG_FILE) == CFG_PARSE_ERROR) {
		(*cfg) = 0;
		return false;
	}

	return true;
}
