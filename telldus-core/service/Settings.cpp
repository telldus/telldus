#include "Settings.h"

/*
* Get the name of the device
*/
std::wstring Settings::getName(int intDeviceId) const {
	return getStringSetting(intDeviceId, L"name", false);
}

/*
* Set the name of the device
*/
bool Settings::setName(int intDeviceId, const std::wstring &strNewName){
	return setStringSetting(intDeviceId, L"name", strNewName, false);
}

/*
* Get the device vendor
*/
std::wstring Settings::getProtocol(int intDeviceId) const {
	return getStringSetting(intDeviceId, L"protocol", false);
}

/*
* Set the device vendor
*/
bool Settings::setProtocol(int intDeviceId, const std::wstring &strVendor){
	return setStringSetting(intDeviceId, L"protocol", strVendor, false);
}

/*
* Get the device model
*/
std::wstring Settings::getModel(int intDeviceId) const {
	return getStringSetting(intDeviceId, L"model", false);
}

/*
* Set the device model
*/
bool Settings::setModel(int intDeviceId, const std::wstring &strModel){
	return setStringSetting(intDeviceId, L"model", strModel, false);
}

/*
* Set device argument
*/
bool Settings::setDeviceParameter(int intDeviceId, const std::wstring &strName, const std::wstring &strValue){
	return setStringSetting(intDeviceId, strName, strValue, true);
}

/*
* Get device argument
*/
std::wstring Settings::getDeviceParameter(int intDeviceId, const std::wstring &strName) const {
	return getStringSetting(intDeviceId, strName, true);
}

/*
* Set preferred controller id
*/
bool Settings::setPreferredControllerId(int intDeviceId, int value){
	return setIntSetting(intDeviceId,  L"controllerId", value, false);
}

/*
* Get preferred controller id
*/
int Settings::getPreferredControllerId(int intDeviceId) {
	return getIntSetting(intDeviceId, L"controllerId", false);
}

#ifndef _CONFUSE

bool Settings::setDeviceState( int intDeviceId, int intDeviceState, const std::wstring &strDeviceStateValue ) {
	bool retval = setIntSetting( intDeviceId, L"state", intDeviceState, true );
	setStringSetting( intDeviceId, L"stateValue", strDeviceStateValue, true );
	return retval;
}

int Settings::getDeviceState( int intDeviceId ) const {
	return getIntSetting( intDeviceId, L"state", true );
}

std::wstring Settings::getDeviceStateValue( int intDeviceId ) const {
	return getStringSetting( intDeviceId, L"stateValue", true );
}

#endif
