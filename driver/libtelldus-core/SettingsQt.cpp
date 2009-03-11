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
#include <QSettings>
#include <stdlib.h>
#include <string.h>

using namespace std;

class privateVars {
public:
	QSettings *settings;
};

	
/*
* Constructor
*/
Settings::Settings(void)
{
	d = new privateVars();
	d->settings = new QSettings("Telldus", "Core");
}

/*
* Destructor
*/
Settings::~Settings(void)
{
	delete d->settings;
	delete d;
}

/*
* Return a setting
*/
std::string Settings::getSetting(const std::string &strName) const {
	return "";
}

/*
* Return the number of stored devices
*/
int Settings::getNumberOfDevices(void) const {
	int devices = 1;
	return devices;
}

int Settings::getDeviceId(int intDeviceIndex) const {
	int id = 1;
	return id;
}

/*
* Add a new device
*/
int Settings::addDevice() {
	return -1;
}

/*
* Get next available device id
*/
int Settings::getNextDeviceId() const {
	int id = 0, max = 0;
	return max;
}

/*
* Remove a device
*/
bool Settings::removeDevice(int intDeviceId) {
	return false;
}

std::string Settings::getStringSetting(int intDeviceId, const std::string &name, bool parameter) const {
	return "hej";
}

bool Settings::setStringSetting(int intDeviceId, const std::string &name, const std::string &value, bool parameter) {
	return true;
}

int Settings::getIntSetting(int intDeviceId, const std::string &name, bool parameter) const {
	int retval = 0;
	return retval;
}

bool Settings::setIntSetting(int intDeviceId, const std::string &name, int value, bool parameter) {
	return true;	
}
