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
#include <QStringList>
#include <QDebug>
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
	int devices = 0;
	d->settings->beginGroup("devices");
	devices = d->settings->childGroups().count();
	d->settings->endGroup();
	return devices;
}

int Settings::getDeviceId(int intDeviceIndex) const {
	int id = 0; 
	d->settings->beginGroup("devices");
	QString strId = d->settings->childGroups().at(intDeviceIndex);
	d->settings->endGroup();
	
	id = strId.toInt();
	
	return id;
}

/*
* Add a new device
*/
int Settings::addDevice() {
	int id = getNextDeviceId();
	
	if (id < 0) {
		return -1;
	}
	
	setStringSetting(id, "name", "Unnamed", false);
	setStringSetting(id, "protocol", "arctech", false);
	
	return id;
}

/*
* Get next available device id
*/
int Settings::getNextDeviceId() const {
	int id = d->settings->value("lastUsedDeviceId", 0).toInt() + 1;
	d->settings->setValue("lastUsedDeviceId", id);
	return id;
}

/*
* Remove a device
*/
bool Settings::removeDevice(int intDeviceId) {
	d->settings->remove( QString("devices/%1").arg(intDeviceId) );
	return true;
}

std::string Settings::getStringSetting(int intDeviceId, const std::string &name, bool parameter) const {
	QString key = QString("devices/%1/").arg(intDeviceId);
	if (parameter) {
		key += "parameters/";
	}
	key += name.c_str();
	QString value = d->settings->value(key).toString();
	return value.toStdString();
}

bool Settings::setStringSetting(int intDeviceId, const std::string &name, const std::string &value, bool parameter) {
	QString key = QString("devices/%1/").arg(intDeviceId);
	if (parameter) {
		key += "parameters/";
	}
	key += name.c_str();
	d->settings->setValue(key, value.c_str());
	d->settings->sync();
	return true;
}

int Settings::getIntSetting(int intDeviceId, const std::string &name, bool parameter) const {
	int retval = 0;
	QString key = QString("devices/%1/").arg(intDeviceId);
	if (parameter) {
		key += "parameters/";
	}
	key += name.c_str();
	retval = d->settings->value(key).toInt();
	return retval;
}

bool Settings::setIntSetting(int intDeviceId, const std::string &name, int value, bool parameter) {
	QString key = QString("devices/%1/").arg(intDeviceId);
	if (parameter) {
		key += "parameters/";
	}
	key += name.c_str();
	d->settings->setValue(key, value);
	d->settings->sync();
	return true;	
}
