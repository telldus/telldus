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
#include "../client/telldus-core.h"
#include <confuse.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

using namespace std;

class Settings::PrivateData {
public:
	cfg_t *cfg;
	cfg_t *var_cfg;
};

bool readConfig(cfg_t **cfg);
bool readVarConfig(cfg_t **cfg);

const char* CONFIG_FILE = "/etc/tellstick.conf";
#ifdef __FreeBSD__
const char* VAR_CONFIG_FILE = "/var/spool/telldus-core.conf";
#else
const char* VAR_CONFIG_FILE = "/var/state/telldus-core.conf";
#endif

/*
* Constructor
*/
Settings::Settings(void)
{
	d = new PrivateData;
	readConfig(&d->cfg);
	readVarConfig(&d->var_cfg);
}

/*
* Destructor
*/
Settings::~Settings(void)
{
	if (d->cfg > 0) {
		cfg_free(d->cfg);
	}
	if (d->var_cfg > 0) {
		cfg_free(d->var_cfg);
	}
	delete d;
}

/*
* Return a setting
*/
std::wstring Settings::getSetting(const std::wstring &strName) const {
	if (d->cfg > 0) {
		std::string setting(cfg_getstr(d->cfg, std::string(strName.begin(), strName.end()).c_str()));	//TODO, safer conversion (other places in this file as well)
		return std::wstring(setting.begin(), setting.end());
	}
	return L"";
}

/*
* Return the number of stored devices
*/
int Settings::getNumberOfDevices(void) const {
	if (d->cfg > 0) {
		return cfg_size(d->cfg, "device");
	}
	return 0;
}

int Settings::getDeviceId(int intDeviceIndex) const {
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
int Settings::getNextDeviceId() const {
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

bool Settings::setDeviceState( int intDeviceId, int intDeviceState, const std::wstring &strDeviceStateValue ) {
	if (d->var_cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->var_cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->var_cfg, "device", i);
		int deviceId = atoi(cfg_title(cfg_device));
		if (deviceId == intDeviceId)  {
			cfg_setint(cfg_device, "state", intDeviceState);
			cfg_setstr(cfg_device, "stateValue", std::string(strDeviceStateValue.begin(), strDeviceStateValue.end()).c_str());
			
			FILE *fp = fopen(VAR_CONFIG_FILE, "w");
			cfg_print(d->var_cfg, fp);
			fclose(fp);
			return true;
		}
	}
	// The device is not found in the file, we must create it manualy...
	FILE *fp = fopen(VAR_CONFIG_FILE, "w");
	if(!fp) {
		fprintf(stderr, "Failed to write state to %s: %s\n",
				VAR_CONFIG_FILE, strerror(errno));
		return false;
	}

	cfg_print(d->var_cfg, fp); //Print the config-file
	fprintf(fp, "device %d {\n}\n", intDeviceId); //Print the new device
	fclose(fp);

	//Re-read config-file
	cfg_free(d->var_cfg);
	readVarConfig(&d->var_cfg);

	return false;
}

int Settings::getDeviceState( int intDeviceId ) const {
	if (d->var_cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->var_cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->var_cfg, "device", i);
		int deviceId = atoi(cfg_title(cfg_device));
		if (deviceId == intDeviceId)  {
			return cfg_getint(cfg_device, "state");
		}
	}
	return TELLSTICK_TURNOFF;
}

std::wstring Settings::getDeviceStateValue( int intDeviceId ) const {
	if (d->var_cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->var_cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->var_cfg, "device", i);
		int deviceId = atoi(cfg_title(cfg_device));
		if (deviceId == intDeviceId)  {
			std::string value(cfg_getstr(cfg_device, "stateValue"));
			return std::wstring(value.begin(), value.end());
		}
	}
	return L"";
}

std::wstring Settings::getStringSetting(int intDeviceId, const std::wstring &name, bool parameter) const {
	if (d->cfg == 0) {
		return L"";
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			if (parameter) {
				cfg_device = cfg_getsec(cfg_device, "parameters");
			}
			std::string setting;
			char *cSetting = cfg_getstr(cfg_device, std::string(name.begin(), name.end()).c_str());
			if (cSetting) {
				setting = cSetting;
			}
			return std::wstring(setting.begin(), setting.end());
		}
	}
	return L"";
}

bool Settings::setStringSetting(int intDeviceId, const std::wstring &name, const std::wstring &value, bool parameter) {
	if (d->cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			if (parameter) {
				cfg_t *cfg_parameters = cfg_getsec(cfg_device, "parameters");
				cfg_setstr(cfg_parameters, std::string(name.begin(), name.end()).c_str(), std::string(value.begin(), value.end()).c_str());
			} else {
				cfg_setstr(cfg_device, std::string(name.begin(), name.end()).c_str(), std::string(value.begin(), value.end()).c_str());
			}
			FILE *fp = fopen(CONFIG_FILE, "w");
			cfg_print(d->cfg, fp);
			fclose(fp);
			return true;
		}
	}
	return false;
}

int Settings::getIntSetting(int intDeviceId, const std::wstring &name, bool parameter) const {
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
			return cfg_getint(cfg_device, std::string(name.begin(), name.end()).c_str());
		}
	}
	return 0;
}

bool Settings::setIntSetting(int intDeviceId, const std::wstring &name, int value, bool parameter) {
	if (d->cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			if (parameter) {
				cfg_t *cfg_parameters = cfg_getsec(cfg_device, "parameters");
				cfg_setint(cfg_parameters, std::string(name.begin(), name.end()).c_str(), value);
			} else {
				cfg_setint(cfg_device, std::string(name.begin(), name.end()).c_str(), value);
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
		//Groups
		CFG_STR("devices", 0, CFGF_NONE),

		CFG_STR("house", 0, CFGF_NONE),
		CFG_STR("unit", 0, CFGF_NONE),
		CFG_STR("code", 0, CFGF_NONE),
		CFG_STR("system", 0, CFGF_NONE),
		CFG_STR("units", 0, CFGF_NONE),
		CFG_STR("fade", 0, CFGF_NONE),

		CFG_END()
	};

	cfg_opt_t device_opts[] = {
		CFG_INT("id", -1, CFGF_NONE),
		CFG_STR("name", "Unnamed", CFGF_NONE),
		CFG_INT("controller", 0, CFGF_NONE),
		CFG_STR("protocol", "arctech", CFGF_NONE),
		CFG_STR("model", "", CFGF_NONE),
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

bool readVarConfig(cfg_t **cfg) {

	cfg_opt_t device_opts[] = {
		CFG_INT("state", 0, CFGF_NONE),
		CFG_STR("stateValue", "", CFGF_NONE),
		CFG_END()
	};

	cfg_opt_t opts[] = {
		CFG_SEC("device", device_opts, CFGF_MULTI | CFGF_TITLE),
		CFG_END()
	};

	(*cfg) = cfg_init(opts, CFGF_NOCASE);
	if (cfg_parse((*cfg), VAR_CONFIG_FILE) == CFG_PARSE_ERROR) {
		(*cfg) = 0;
		return false;
	}

	return true;
}
