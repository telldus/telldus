#include "Settings.h"
#include <Windows.h>
#include <sstream> 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "../client/telldus-core.h"

const int intMaxRegValueLength = 1000;

class Settings::PrivateData {
public:
	HKEY rootKey;
	std::wstring strRegPathDevice;
	std::wstring strRegPath;	
};

/*
* Constructor
*/
Settings::Settings(void) {
	d = new PrivateData();
	d->strRegPathDevice = L"SOFTWARE\\Telldus\\Devices\\";
	d->strRegPath = L"SOFTWARE\\Telldus\\";
	d->rootKey = HKEY_LOCAL_MACHINE;
}

/*
* Destructor
*/
Settings::~Settings(void) {
	delete d;
}

/*
* Return the number of stored devices
*/
int Settings::getNumberOfDevices(void) const {
	TelldusCore::MutexLocker locker(&mutex);

	int intNumberOfDevices = 0;
	HKEY hk;
	
	long lnExists = RegOpenKeyEx(d->rootKey, d->strRegPathDevice.c_str(), 0, KEY_QUERY_VALUE, &hk);
				
	if(lnExists == ERROR_SUCCESS){
	
		std::wstring strNumSubKeys;
		DWORD dNumSubKeys;
		RegQueryInfoKey(hk, NULL, NULL, NULL, &dNumSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		
		intNumberOfDevices = (int)dNumSubKeys;

		RegCloseKey(hk);
	}
	return intNumberOfDevices;
}


int Settings::getDeviceId(int intDeviceIndex) const {
	TelldusCore::MutexLocker locker(&mutex);

	int intReturn = -1;
	HKEY hk;
	
	long lnExists = RegOpenKeyEx(d->rootKey, d->strRegPathDevice.c_str(), 0, KEY_READ, &hk);
				
	if(lnExists == ERROR_SUCCESS){
		
		wchar_t* Buff = new wchar_t[intMaxRegValueLength];
		DWORD size = intMaxRegValueLength;
		if (RegEnumKeyEx(hk, intDeviceIndex, (LPWSTR)Buff, &size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
			intReturn = _wtoi(Buff);
		}
		
		delete Buff;
		RegCloseKey(hk);
	}
	return intReturn;
}

/*
* Add a new device
*/
int Settings::addDevice() {
	TelldusCore::MutexLocker locker(&mutex);

	int intDeviceId = -1;
	HKEY hk;

	DWORD dwDisp;
	intDeviceId = getNextDeviceId();

	std::wostringstream ssRegPath; 
	ssRegPath << d->strRegPathDevice << intDeviceId;
	std::wstring strCompleteRegPath = ssRegPath.str();
		
	if (RegCreateKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hk, &dwDisp)) {
		//fail
		intDeviceId = -1;
	}

	RegCloseKey(hk);
	return intDeviceId;
}

/*
* Get next available device id
*/
int Settings::getNextDeviceId() const {
	//Private, no locks needed
	int intReturn = -1;
	HKEY hk;
	DWORD dwDisp;

	long lnExists = RegCreateKeyEx(d->rootKey, d->strRegPathDevice.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hk, &dwDisp);	//create or open if already created
				
	if(lnExists == ERROR_SUCCESS){
			
		DWORD dwLength;
		char *Buff = new char[intMaxRegValueLength];

		long lngStatus = RegQueryValueEx(hk, L"LastUsedId", NULL, NULL, (LPBYTE)Buff, &dwLength);

		if(lngStatus == ERROR_MORE_DATA){
			//The buffer is to small, recreate it
			delete Buff;
			Buff = new char[dwLength];
			lngStatus = RegQueryValueEx(hk, L"LastUsedId", NULL, NULL, (LPBYTE)Buff, &dwLength);
		}

		if(lngStatus == ERROR_SUCCESS){

			int intLast = (int)Buff[0];
			intReturn = intLast + 1;
		} else {
			intReturn = 1;
		}
		delete Buff;

		DWORD dwVal = intReturn;
			
		RegSetValueEx (hk, L"LastUsedId", 0L, REG_DWORD, (CONST BYTE*) &dwVal, sizeof(DWORD));
			
	}
	RegCloseKey(hk);
	return intReturn;
}

/*
* Remove a device
*/
int Settings::removeDevice(int intDeviceId) {
	TelldusCore::MutexLocker locker(&mutex);
	
	std::wostringstream ssRegPath; 
	ssRegPath << d->strRegPathDevice << intDeviceId;
	std::wstring strCompleteRegPath = ssRegPath.str();

	long lngSuccess = RegDeleteKey(d->rootKey, strCompleteRegPath.c_str());

	if(lngSuccess == ERROR_SUCCESS){
		//one of the deletions succeeded
		return TELLSTICK_SUCCESS;
	}

	return TELLSTICK_ERROR_UNKNOWN;
}

std::wstring Settings::getStringSetting(int intDeviceId, const std::wstring &name, bool parameter) const {
	std::wstring strReturn;
	HKEY hk;

	std::wostringstream ssRegPath; 
	ssRegPath << d->strRegPathDevice << intDeviceId;
	std::wstring strCompleteRegPath = ssRegPath.str();
	long lnExists = RegOpenKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, KEY_QUERY_VALUE, &hk);
			
	if(lnExists == ERROR_SUCCESS){
		wchar_t* Buff = new wchar_t[intMaxRegValueLength];
		DWORD dwLength = sizeof(wchar_t)*intMaxRegValueLength;
		long lngStatus = RegQueryValueEx(hk, name.c_str(), NULL, NULL, (LPBYTE)Buff, &dwLength);

		if(lngStatus == ERROR_MORE_DATA){
			//The buffer is to small, recreate it
			delete Buff;
			Buff = new wchar_t[dwLength];
			lngStatus = RegQueryValueEx(hk, name.c_str(), NULL, NULL, (LPBYTE)Buff, &dwLength);
		}
		if (lngStatus == ERROR_SUCCESS) {
			strReturn = Buff;
		}
		delete Buff;
	}
	RegCloseKey(hk);
	return strReturn;
}

int Settings::setStringSetting(int intDeviceId, const std::wstring &name, const std::wstring &value, bool parameter) {

	HKEY hk;
	int ret = TELLSTICK_SUCCESS;
		
	std::wostringstream ssRegPath; 
	ssRegPath << d->strRegPathDevice << intDeviceId;
	std::wstring strCompleteRegPath = ssRegPath.str();
	long lnExists = RegOpenKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, KEY_WRITE, &hk);
				
	if (lnExists == ERROR_SUCCESS){
		int length = (int)value.length() * sizeof(wchar_t);
		RegSetValueEx(hk, name.c_str(), 0, REG_SZ, (LPBYTE)value.c_str(), length+1);
	} else {
		ret = TELLSTICK_ERROR_UNKNOWN;
	}
	RegCloseKey(hk);

	return ret;

}

int Settings::getIntSetting(int intDeviceId, const std::wstring &name, bool parameter) const {
	int intReturn = 0;

	std::wstring strSetting = getStringSetting(intDeviceId, name, parameter);
	if (strSetting.length()) {
		intReturn = (int)strSetting[0];
	}

	return intReturn;
}

int Settings::setIntSetting(int intDeviceId, const std::wstring &name, int value, bool parameter) {
	int intReturn = TELLSTICK_ERROR_UNKNOWN;
	HKEY hk;

	std::wostringstream ssRegPath; 
	ssRegPath << d->strRegPathDevice << intDeviceId;
	std::wstring strCompleteRegPath = ssRegPath.str();
	long lnExists = RegOpenKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, KEY_WRITE, &hk);
	if (lnExists == ERROR_SUCCESS) {
		DWORD dwVal = value;
		lnExists = RegSetValueEx (hk, name.c_str(), 0L, REG_DWORD, (CONST BYTE*) &dwVal, sizeof(DWORD));
		if (lnExists == ERROR_SUCCESS) {
			intReturn = TELLSTICK_SUCCESS;
		}
	}
	RegCloseKey(hk);
	return intReturn;
}
