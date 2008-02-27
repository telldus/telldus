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
#include "TelldusSettings.h"
#include <confuse.h>

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
TelldusSettings::TelldusSettings(void)
{
	d = new privateVars();
	readConfig(&d->cfg);
// 	printf("Nu: %d\n", cfg_size(d->cfg, "device"));

}

/*
* Destructor
*/
TelldusSettings::~TelldusSettings(void)
{
	if (d->cfg > 0)
		cfg_free(d->cfg);
}

/*
* Return the number of stored devices
*/
int TelldusSettings::getNumberOfDevices(void){
	if (d->cfg > 0) {
		return cfg_size(d->cfg, "device");
	}
	return 0;
}

/*
* Get the requested device
*/
Device* TelldusSettings::getDevice(int intDeviceId){
	return NULL;
}

/*
* Get the name of the device
*/
char* TelldusSettings::getName(int intDeviceId){
	return getStringSetting(intDeviceId, "name");
}

/*
* Set the name of the device
*/
bool TelldusSettings::setName(int intDeviceId, char* strNewName){
	bool blnSuccess = true;
	setStringSetting(intDeviceId, "name", strNewName);
	return blnSuccess;
}

/*
* Get the device vendor
*/
char* TelldusSettings::getVendor(int intDeviceId){
	return getStringSetting(intDeviceId, "vendor");
}

/*
* Set the device vendor
*/
bool TelldusSettings::setVendor(int intDeviceId, char* strVendor){
	bool blnSuccess = true;
	setStringSetting(intDeviceId, "vendor", strVendor);
	return blnSuccess;
}

/*
* Get the device model
*/
char* TelldusSettings::getModel(int intDeviceId){
	return getStringSetting(intDeviceId, "model");
}

/*
* Set the device model
*/
bool TelldusSettings::setModel(int intDeviceId, char* strVendor){
	bool blnSuccess = true;
	setStringSetting(intDeviceId, "model", strVendor);
	return blnSuccess;
}

int TelldusSettings::getDeviceId(int intDeviceIndex){
	if (intDeviceIndex >= getNumberOfDevices()) { //Out of bounds
		return -1;
	}
	cfg_t *cfg_device = cfg_getnsec(d->cfg, "device", intDeviceIndex);
	int id = cfg_getint(cfg_device, "id");
	return id;
}

/*
* Get number of device arguments
*/
int TelldusSettings::getNumberOfArguments(int intDeviceId){
	int intReturn = -1;
	
	return intReturn;
}

/*
* Get device arguments
*/
int* TelldusSettings::getArguments(int intDeviceId){
//	vector <int> vReturn;
	int* intReturn = new int[2];
	
	return intReturn;
}

/*
* Set device arguments
*/
bool TelldusSettings::setArguments(int intDeviceId, vector <int> vArguments){

	bool blnSuccess = false;
	return blnSuccess;
}

/*
* Add a new device
*/
int TelldusSettings::addDevice(){
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
int TelldusSettings::getNextDeviceId(){
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
bool TelldusSettings::removeDevice(int intDeviceId){
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

//only for debug reasons
void TelldusSettings::debugLog(char* debugstring){
/*	ofstream debugfile("c:\\telldusdebug.txt", ios::app);
	if(debugfile){
		debugfile << debugstring << endl;
		debugfile.close();
	}*/
}

//only for debug reasons
void TelldusSettings::debugLog(int debugint){
/*	ofstream debugfile("c:\\telldusdebug.txt", ios::app);
	if(debugfile){
		debugfile << debugint << endl;
		debugfile.close();
	}*/
}

char *TelldusSettings::getStringSetting(int intDeviceId, const char* name) {
	if (d->cfg == 0) {
		return "";
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			const char *strSetting = cfg_getstr(cfg_device, name);
			char *strReturn = (char *)malloc(strlen(strSetting) * sizeof(char));
			strcpy(strReturn, strSetting);
			return strReturn;
		}
	}
	return "";
}

bool TelldusSettings::setStringSetting(int intDeviceId, const char* name, const char *value) {
	if (d->cfg == 0) {
		return false;
	}
	cfg_t *cfg_device;
	for (int i = 0; i < cfg_size(d->cfg, "device"); ++i) {
		cfg_device = cfg_getnsec(d->cfg, "device", i);
		if (cfg_getint(cfg_device, "id") == intDeviceId)  {
			cfg_setstr(cfg_device, name, value);
			FILE *fp = fopen(CONFIG_FILE, "w");
			cfg_print(d->cfg, fp);
			fclose(fp);
			return true;
		}
	}
	return false;
}

bool readConfig(cfg_t **cfg) {
	cfg_opt_t device_opts[] = {
		CFG_INT("id", -1, CFGF_NONE),
		CFG_STR("name", "Unnamed", CFGF_NONE),
		CFG_STR("vendor", "Nexa", CFGF_NONE),
		CFG_STR("model", "1", CFGF_NONE),
		
		//Nexa
		CFG_STR("nexa_house", 0, CFGF_NONE),
		CFG_STR("nexa_unit", 0, CFGF_NONE),
		
		//Ikea
		CFG_STR("ikea_system", 0, CFGF_NONE),
		CFG_STR("ikea_units", 0, CFGF_NONE),
		CFG_STR("ikea_fade", 0, CFGF_NONE),
		
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
