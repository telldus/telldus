#include "Settings.h"

/*
* Get the name of the device
*/
std::wstring Settings::getName(int intDeviceId) const {
	TelldusCore::MutexLocker locker(&mutex);
	return getStringSetting(intDeviceId, L"name", false);
}

/*
* Set the name of the device
*/
int Settings::setName(int intDeviceId, const std::wstring &strNewName){
	TelldusCore::MutexLocker locker(&mutex);
	return setStringSetting(intDeviceId, L"name", strNewName, false);
}

/*
* Get the device vendor
*/
std::wstring Settings::getProtocol(int intDeviceId) const {
	TelldusCore::MutexLocker locker(&mutex);
	return getStringSetting(intDeviceId, L"protocol", false);
}

/*
* Set the device vendor
*/
int Settings::setProtocol(int intDeviceId, const std::wstring &strVendor){
	TelldusCore::MutexLocker locker(&mutex);
	return setStringSetting(intDeviceId, L"protocol", strVendor, false);
}

/*
* Get the device model
*/
std::wstring Settings::getModel(int intDeviceId) const {
	TelldusCore::MutexLocker locker(&mutex);
	return getStringSetting(intDeviceId, L"model", false);
}

/*
* Set the device model
*/
int Settings::setModel(int intDeviceId, const std::wstring &strModel){
	TelldusCore::MutexLocker locker(&mutex);
	return setStringSetting(intDeviceId, L"model", strModel, false);
}

/*
* Set device argument
*/
int Settings::setDeviceParameter(int intDeviceId, const std::wstring &strName, const std::wstring &strValue){
	TelldusCore::MutexLocker locker(&mutex);
	return setStringSetting(intDeviceId, strName, strValue, true);
}

/*
* Get device argument
*/
std::wstring Settings::getDeviceParameter(int intDeviceId, const std::wstring &strName) const {
	TelldusCore::MutexLocker locker(&mutex);
	return getStringSetting(intDeviceId, strName, true);
}

/*
* Set preferred controller id
*/
int Settings::setPreferredControllerId(int intDeviceId, int value){
	TelldusCore::MutexLocker locker(&mutex);
	return setIntSetting(intDeviceId,  L"controller", value, false);
}

/*
* Get preferred controller id
*/
int Settings::getPreferredControllerId(int intDeviceId) {
	TelldusCore::MutexLocker locker(&mutex);
	return getIntSetting(intDeviceId, L"controller", false);
}

#ifndef _CONFUSE

bool Settings::setDeviceState( int intDeviceId, int intDeviceState, const std::wstring &strDeviceStateValue ) {
	TelldusCore::MutexLocker locker(&mutex);
	bool retval = setIntSetting( intDeviceId, L"state", intDeviceState, true );
	setStringSetting( intDeviceId, L"stateValue", strDeviceStateValue, true );
	return retval;
}

int Settings::getDeviceState( int intDeviceId ) const {
	TelldusCore::MutexLocker locker(&mutex);
	return getIntSetting( intDeviceId, L"state", true );
}

std::wstring Settings::getDeviceStateValue( int intDeviceId ) const {
	TelldusCore::MutexLocker locker(&mutex);
	return getStringSetting( intDeviceId, L"stateValue", true );
}

#endif
