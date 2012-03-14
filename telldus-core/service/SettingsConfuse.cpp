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
#include "SettingsConfusePaths.h"
#include "../client/telldus-core.h"
#include "Strings.h"
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

const char* CONFIG_FILE = CONFIG_PATH "/tellstick.conf";
const char* VAR_CONFIG_FILE = VAR_CONFIG_PATH "/telldus-core.conf";

/*
* Constructor
*/
Settings::Settings(void)
{
	TelldusCore::MutexLocker locker(&mutex);
	d = new PrivateData;
	readConfig(&d->cfg);
	readVarConfig(&d->var_cfg);
}

/*
* Destructor
*/
Settings::~Settings(void)
{
	TelldusCore::MutexLocker locker(&mutex);
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
	TelldusCore::MutexLocker locker(&mutex);
	if (d->cfg > 0) {
		std::string setting(cfg_getstr(d->cfg, TelldusCore::wideToString(strName).c_str()));
		return TelldusCore::charToWstring(setting.c_str());
	}
	return L"";
}

/*
* Return the number of stored devices
*/
int Settings::getNumberOfNodes(Node node) const {
	TelldusCore::MutexLocker locker(&mutex);
	if (d->cfg > 0) {
		if (node == Device) {
			return cfg_size(d->cfg, "device");
		} else if (node == Controller) {
			return cfg_size(d->cfg, "controller");
		}
	}
	return 0;
}

int Settings::getNodeId(Node type, int intDeviceIndex) const {
	if (intDeviceIndex >= getNumberOfNodes(type)) { //Out of bounds
		return -1;
	}
	TelldusCore::MutexLocker locker(&mutex);
	cfg_t *cfg_node;
	if (type == Device) {
		cfg_node = cfg_getnsec(d->cfg, "device", intDeviceIndex);
	} else if (type == Controller) {
		cfg_node = cfg_getnsec(d->cfg, "controller", intDeviceIndex);
	}
	int id = cfg_getint(cfg_node, "id");
	return id;
}

/*
* Add a new node
*/
int Settings::addNode(Node type){
	TelldusCore::MutexLocker locker(&mutex);
	int intNodeId = getNextNodeId(type);

	FILE *fp = fopen(CONFIG_FILE, "w");
	if (!fp) {
		return TELLSTICK_ERROR_PERMISSION_DENIED;
	}
	cfg_print(d->cfg, fp); //Print the config-file
	if (type == Device) {
		fprintf(fp, "device {\n  id=%d\n}\n", intNodeId); //Print the new device
	} else if (type == Controller) {
		fprintf(fp, "controller {\n  id=%d\n}\n", intNodeId); //Print the new controller
	}
	fclose(fp);

	//Re-read config-file
	cfg_free(d->cfg);
	readConfig(&d->cfg);
	return intNodeId;
}

/*
* Get next available node id
*/
int Settings::getNextNodeId(Node type) const {
	//Private, no locks needed
	int intNodeId = 0;
	cfg_t *cfg_node;
	std::string strType;
	if (type == Device) {
		strType = "device";
	} else if (type == Controller) {
		strType = "controller";
	}
	for (int i = 0; i < cfg_size(d->cfg, strType.c_str()); ++i) {
		cfg_node = cfg_getnsec(d->cfg, strType.c_str(), i);
		if (cfg_getint(cfg_node, "id") >= intNodeId)  {
			intNodeId = cfg_getint(cfg_node, "id");
		}
	}
	intNodeId++;
	return intNodeId;
}

/*
* Remove a device
*/
int Settings::removeNode(Node type, int intNodeId){
	TelldusCore::MutexLocker locker(&mutex);
	FILE *fp = fopen(CONFIG_FILE, "w");
	if (!fp) {
		return TELLSTICK_ERROR_PERMISSION_DENIED;
	}

	std::string strType = getNodeString(type);

	// Print all opts
	for(int i = 0; d->cfg->opts[i].name; i++) {

		// Check if it isn't a device section
		if (strcmp(d->cfg->opts[i].name, strType.c_str()) != 0) {
			cfg_opt_print(&d->cfg->opts[i], fp);
		} else {
			// Print all sections except the one to remove
			cfg_t *cfg_node;
			for (int i = 0; i < cfg_size(d->cfg, strType.c_str()); ++i) {
				cfg_node = cfg_getnsec(d->cfg, strType.c_str(), i);
				if (cfg_getint(cfg_node, "id") != intNodeId) { //This isn't the one to skip
					fprintf(fp, "%s {\n", strType.c_str());
					cfg_print_indent(cfg_node, fp, 1);
					fprintf(fp, "}\n");
				}
			}
		}
	}
	fclose(fp);

	//Re-read config-file
	cfg_free(d->cfg);
	readConfig(&d->cfg);

	return TELLSTICK_SUCCESS;
}

bool Settings::setDeviceState( int intDeviceId, int intDeviceState, const std::wstring &strDeviceStateValue ) {
	TelldusCore::MutexLocker locker(&mutex);
	if (d->var_cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->var_cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->var_cfg, "device", i);
		int deviceId = atoi(cfg_title(cfg_device));
		if (deviceId == intDeviceId)  {
			cfg_setint(cfg_device, "state", intDeviceState);
			cfg_setstr(cfg_device, "stateValue", TelldusCore::wideToString(strDeviceStateValue).c_str());

			FILE *fp = fopen(VAR_CONFIG_FILE, "w");
			if(fp == 0){
				return false;
			}
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
	TelldusCore::MutexLocker locker(&mutex);
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
	TelldusCore::MutexLocker locker(&mutex);
	if (d->var_cfg == 0) {
		return L"";
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->var_cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->var_cfg, "device", i);
		int deviceId = atoi(cfg_title(cfg_device));
		if (deviceId == intDeviceId)  {
			std::string value(cfg_getstr(cfg_device, "stateValue"));
			return TelldusCore::charToWstring(value.c_str());
		}
	}
	return L"";
}

std::wstring Settings::getStringSetting(Node type, int intNodeId, const std::wstring &name, bool parameter) const {
	//already locked
	if (d->cfg == 0) {
		return L"";
	}
	std::string strType = getNodeString(type);

	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, strType.c_str()); ++i) {
		cfg_device = cfg_getnsec(d->cfg, strType.c_str(), i);
		if (cfg_getint(cfg_device, "id") == intNodeId)  {
			if (parameter) {
				cfg_device = cfg_getsec(cfg_device, "parameters");
			}
			std::wstring setting;
			char *cSetting = cfg_getstr(cfg_device, TelldusCore::wideToString(name).c_str());
			if (cSetting) {
				setting = TelldusCore::charToWstring(cSetting);
			}
			return setting;
		}
	}
	return L"";
}

int Settings::setStringSetting(Node type, int intDeviceId, const std::wstring &name, const std::wstring &value, bool parameter) {
	//already locked
	if (d->cfg == 0) {
		return TELLSTICK_ERROR_PERMISSION_DENIED;
	}
	std::string strType = getNodeString(type);
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, strType.c_str()); ++i) {
		cfg_device = cfg_getnsec(d->cfg, strType.c_str(), i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			std::string newValue = TelldusCore::wideToString(value);
			if (parameter) {
				cfg_t *cfg_parameters = cfg_getsec(cfg_device, "parameters");
				cfg_setstr(cfg_parameters, TelldusCore::wideToString(name).c_str(), newValue.c_str());
			} else {
				cfg_setstr(cfg_device, TelldusCore::wideToString(name).c_str(), newValue.c_str());
			}
			FILE *fp = fopen(CONFIG_FILE, "w");
			if (!fp) {
				return TELLSTICK_ERROR_PERMISSION_DENIED;
			}
			cfg_print(d->cfg, fp);
			fclose(fp);
			return TELLSTICK_SUCCESS;
		}
	}
	return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
}

int Settings::getIntSetting(Node type, int intDeviceId, const std::wstring &name, bool parameter) const {
	//already locked
	if (d->cfg == 0) {
		return 0;
	}
	std::string strType = getNodeString(type);
	cfg_t *cfg_node;
	for(int i = 0; i < cfg_size(d->cfg, strType.c_str()); ++i) {
		cfg_node = cfg_getnsec(d->cfg, strType.c_str(), i);
		if (cfg_getint(cfg_node, "id") == intDeviceId) {
			if (parameter) {
				cfg_node = cfg_getsec(cfg_node, "parameters");
			}
			return cfg_getint(cfg_node, TelldusCore::wideToString(name).c_str());
		}
	}
	return 0;
}

int Settings::setIntSetting(Node type, int intDeviceId, const std::wstring &name, int value, bool parameter) {
	//already locked
	if (d->cfg == 0) {
		return TELLSTICK_ERROR_PERMISSION_DENIED;
	}
	std::string strType = getNodeString(type);
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, strType.c_str()); ++i) {
		cfg_device = cfg_getnsec(d->cfg, strType.c_str(), i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			if (parameter) {
				cfg_t *cfg_parameters = cfg_getsec(cfg_device, "parameters");
				cfg_setint(cfg_parameters, TelldusCore::wideToString(name).c_str(), value);
			} else {
				cfg_setint(cfg_device, TelldusCore::wideToString(name).c_str(), value);
			}
			FILE *fp = fopen(CONFIG_FILE, "w");
			if (!fp) {
				return TELLSTICK_ERROR_PERMISSION_DENIED;
			}
			cfg_print(d->cfg, fp);
			fclose(fp);
			return TELLSTICK_SUCCESS;
		}
	}
	return TELLSTICK_ERROR_DEVICE_NOT_FOUND;
}


bool readConfig(cfg_t **cfg) {
	//All the const_cast keywords is to remove the compiler warnings generated by the C++-compiler.
	cfg_opt_t controller_opts[] = {
		CFG_INT(const_cast<char *>("id"), -1, CFGF_NONE),
		CFG_STR(const_cast<char *>("name"), const_cast<char *>(""), CFGF_NONE),
		CFG_INT(const_cast<char *>("type"), 0, CFGF_NONE),
		CFG_STR(const_cast<char *>("serial"), const_cast<char *>(""), CFGF_NONE),

		CFG_END()
	};

	cfg_opt_t device_parameter_opts[] = {
		//Groups
		CFG_STR(const_cast<char *>("devices"), 0, CFGF_NONE),

		CFG_STR(const_cast<char *>("house"), 0, CFGF_NONE),
		CFG_STR(const_cast<char *>("unit"), 0, CFGF_NONE),
		CFG_STR(const_cast<char *>("code"), 0, CFGF_NONE),
		CFG_STR(const_cast<char *>("system"), 0, CFGF_NONE),
		CFG_STR(const_cast<char *>("units"), 0, CFGF_NONE),
		CFG_STR(const_cast<char *>("fade"), 0, CFGF_NONE),

		CFG_END()
	};

	cfg_opt_t device_opts[] = {
		CFG_INT(const_cast<char *>("id"), -1, CFGF_NONE),
		CFG_STR(const_cast<char *>("name"), const_cast<char *>("Unnamed"), CFGF_NONE),
		CFG_INT(const_cast<char *>("controller"), 0, CFGF_NONE),
		CFG_STR(const_cast<char *>("protocol"), const_cast<char *>("arctech"), CFGF_NONE),
		CFG_STR(const_cast<char *>("model"), const_cast<char *>(""), CFGF_NONE),
		CFG_SEC(const_cast<char *>("parameters"), device_parameter_opts, CFGF_NONE),
		CFG_END()
	};

	cfg_opt_t opts[] = {
		CFG_STR(const_cast<char *>("user"), const_cast<char *>("nobody"), CFGF_NONE),
		CFG_STR(const_cast<char *>("group"), const_cast<char *>("plugdev"), CFGF_NONE),
		CFG_STR(const_cast<char *>("deviceNode"), const_cast<char *>("/dev/tellstick"), CFGF_NONE),
		CFG_STR(const_cast<char *>("ignoreControllerConfirmation"), const_cast<char *>("false"), CFGF_NONE),
		CFG_SEC(const_cast<char *>("device"), device_opts, CFGF_MULTI),
		CFG_SEC(const_cast<char *>("controller"), controller_opts, CFGF_MULTI),
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
		CFG_INT(const_cast<char *>("state"), 0, CFGF_NONE),
		CFG_STR(const_cast<char *>("stateValue"), const_cast<char *>(""), CFGF_NONE),
		CFG_END()
	};

	cfg_opt_t opts[] = {
		CFG_SEC(const_cast<char *>("device"), device_opts, CFGF_MULTI | CFGF_TITLE),
		CFG_END()
	};

	(*cfg) = cfg_init(opts, CFGF_NOCASE);
	if (cfg_parse((*cfg), VAR_CONFIG_FILE) == CFG_PARSE_ERROR) {
		(*cfg) = 0;
		return false;
	}

	return true;
}
