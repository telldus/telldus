#include "Settings.h"

/*
* Get the name of the device
*/
std::string Settings::getName(int intDeviceId) const {
	return getStringSetting(intDeviceId, "name", false);
}

/*
* Set the name of the device
*/
bool Settings::setName(int intDeviceId, const std::string &strNewName){
	return setStringSetting(intDeviceId, "name", strNewName, false);
}

/*
* Get the device vendor
*/
std::string Settings::getProtocol(int intDeviceId) const {
	return getStringSetting(intDeviceId, "protocol", false);
}

/*
* Set the device vendor
*/
bool Settings::setProtocol(int intDeviceId, const std::string &strVendor){
	return setStringSetting(intDeviceId, "protocol", strVendor, false);
}

/*
* Get the device model
*/
std::string Settings::getModel(int intDeviceId) const {
	return getStringSetting(intDeviceId, "model", false);
}

/*
* Set the device model
*/
bool Settings::setModel(int intDeviceId, const std::string &strModel){
	return setStringSetting(intDeviceId, "model", strModel, false);
}

/*
* Set device argument
*/
bool Settings::setDeviceParameter(int intDeviceId, const std::string &strName, const std::string &strValue){
	return setStringSetting(intDeviceId, strName, strValue, true);
}

/*
* Get device argument
*/
std::string Settings::getDeviceParameter(int intDeviceId, const std::string &strName) const {
	return getStringSetting(intDeviceId, strName, true);
}

#ifndef _CONFUSE

bool Settings::setDeviceState( int intDeviceId, int intDeviceState, const std::string &strDeviceStateValue ) {
	bool retval = setIntSetting( intDeviceId, "state", intDeviceState, true );
	setStringSetting( intDeviceId, "stateValue", strDeviceStateValue, true );
	return retval;
}

int Settings::getDeviceState( int intDeviceId ) const {
	return getIntSetting( intDeviceId, "state", true );
}

std::string Settings::getDeviceStateValue( int intDeviceId ) const {
	return getStringSetting( intDeviceId, "stateValue", true );
}

#endif
