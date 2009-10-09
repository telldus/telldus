#include "StdAfx.h"
#include "Settings.h"
#include <sstream> 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

bool storeGlobal(privateVars *d);

const int intMaxRegValueLength = 1000;

class privateVars {
public:
	HKEY hk;
	HKEY rootKey;
	std::string strRegPathDevice;
	std::string strRegPath;	
};

/*
* Constructor
*/
Settings::Settings(void)
{
	d = new privateVars();
	d->strRegPathDevice = "SOFTWARE\\Telldus\\Devices\\";
	d->strRegPath = "SOFTWARE\\Telldus\\";
	if (storeGlobal(d)) {
		d->rootKey = HKEY_LOCAL_MACHINE;
	} else {
		d->rootKey = HKEY_CURRENT_USER;
	}
}

/*
* Destructor
*/
Settings::~Settings(void)
{
	//RegCloseKey(hk);	//close all, if still open //TODO: Need some way to know if open or closed
	d->strRegPath = "";
	d->strRegPathDevice = "";
	delete d;
}

/*
* Return the number of stored devices
*/
int Settings::getNumberOfDevices(void) const {
	
	int intNumberOfDevices = 0;
	
	try{
		
		long lnExists = RegOpenKeyEx(d->rootKey, d->strRegPathDevice.c_str(), 0, KEY_QUERY_VALUE, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
		
			std::string strNumSubKeys;
			DWORD dNumSubKeys;
			RegQueryInfoKey(d->hk, NULL, NULL, NULL, &dNumSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		
			intNumberOfDevices = (int)dNumSubKeys;

			RegCloseKey(d->hk);
		}
		else{
			throw std::exception();	//couldn't open reg key
		}
	}
	catch(...){
		intNumberOfDevices = -1;
	}
	return intNumberOfDevices;
}


int Settings::getDeviceId(int intDeviceIndex) const {
	int intReturn = -1;
	
	try{
		
		long lnExists = RegOpenKeyEx(d->rootKey, d->strRegPathDevice.c_str(), 0, KEY_READ, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
		
			char* Buff = new char[intMaxRegValueLength];
			DWORD size = intMaxRegValueLength;
			if (RegEnumKeyEx(d->hk, intDeviceIndex, (LPSTR)Buff, &size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
				intReturn = (int)_atoi64(Buff);
			}
			
			delete Buff;

			RegCloseKey(d->hk);
		} else {
			throw std::exception();	//couldn't open reg key
		}
	}
	catch(...){
		intReturn = -1;
	}
	return intReturn;


}

/*
* Add a new device
*/
int Settings::addDevice(){

	int intDeviceId = -1;
	try{

		DWORD dwDisp;
		intDeviceId = getNextDeviceId();

		std::ostringstream ssRegPath; 
		ssRegPath << d->strRegPathDevice << intDeviceId;
		std::string strCompleteRegPath = ssRegPath.str();
		
		if(RegCreateKeyEx(d->rootKey,
				   strCompleteRegPath.c_str(),
				   0,
				   NULL,
				   REG_OPTION_NON_VOLATILE,
				   KEY_ALL_ACCESS,
				   NULL,
				   &d->hk,
				   &dwDisp)){
			//fail
		   throw std::exception("Create Key failed");
		}

		RegCloseKey(d->hk);

	}
	catch(...){
		intDeviceId = -1;
	}
	return intDeviceId;
}

/*
* Get next available device id
*/
int Settings::getNextDeviceId() const {

	int intReturn = -1;
	try{
		DWORD dwDisp;
		long lnExists = RegCreateKeyEx(d->rootKey, d->strRegPathDevice.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE,
				   KEY_ALL_ACCESS,
				   NULL,
				   &d->hk,
				   &dwDisp);	//create or open if already created
				
		if(lnExists == ERROR_SUCCESS){
			
			DWORD dwLength;
			char* Buff = new char[intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(d->hk, "LastUsedId", NULL, NULL, (LPBYTE)Buff, &dwLength);

			if(lngStatus == ERROR_MORE_DATA){
				//The buffer is to small, recreate it
				delete Buff;
				Buff = new char[dwLength];
				lngStatus = RegQueryValueEx(d->hk, "LastUsedId", NULL, NULL, (LPBYTE)Buff, &dwLength);
			}

			if(lngStatus == ERROR_SUCCESS){

				int intLast = (int)Buff[0];
				intReturn = intLast + 1;
			}
			else{
				intReturn = 1;
			}
			delete Buff;

			DWORD dwVal = intReturn;
			
			RegSetValueEx (d->hk, "LastUsedId", 0L, REG_DWORD, (CONST BYTE*) &dwVal, sizeof(DWORD));
			
		}
		RegCloseKey(d->hk);
	}
	catch(...){
		intReturn = -1;
	}
	return intReturn;
}

/*
* Remove a device
*/
bool Settings::removeDevice(int intDeviceId){

	bool blnSuccess = true;
	try{
		std::ostringstream ssRegPath; 
		ssRegPath << d->strRegPathDevice << intDeviceId;
		std::string strCompleteRegPath = ssRegPath.str();

		long lngSuccess = RegDeleteKey(d->rootKey, strCompleteRegPath.c_str());
		if(lngSuccess != ERROR_SUCCESS){
			blnSuccess = false;
		}
	}
	catch(...){
		blnSuccess = false;
	}

	return blnSuccess;
}

std::string Settings::getStringSetting(int intDeviceId, const std::string &name, bool parameter) const {
	std::string strReturn = "";

	try{
		std::ostringstream ssRegPath; 
		ssRegPath << d->strRegPathDevice << intDeviceId;
		std::string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, KEY_QUERY_VALUE, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
			DWORD dwLength;
			char* Buff = new char[intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(d->hk, name.c_str(), NULL, NULL, (LPBYTE)Buff, &dwLength);
			if(lngStatus == ERROR_MORE_DATA){
				//The buffer is to small, recreate it
				delete Buff;
				Buff = new char[dwLength];
				lngStatus = RegQueryValueEx(d->hk, name.c_str(), NULL, NULL, (LPBYTE)Buff, &dwLength);
			}
			if (lngStatus == ERROR_SUCCESS) {
				strReturn = Buff;
			}
			delete Buff;
		}
		else{
			throw std::exception();	//couldn't open reg key
		}
		RegCloseKey(d->hk);
	}
	catch(...){
		strReturn = "";
	}
	return strReturn;
}

bool Settings::setStringSetting(int intDeviceId, const std::string &name, const std::string &value, bool parameter) {

	bool blnSuccess = true;
	try{
		
		std::ostringstream ssRegPath; 
		ssRegPath << d->strRegPathDevice << intDeviceId;
		std::string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, KEY_WRITE, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
			int length = (int)value.length();
			RegSetValueEx(d->hk, name.c_str(), 0, REG_SZ, (LPBYTE)value.c_str(), length);
		}
		else{
			throw std::exception();	//couldn't open reg key
		}
		RegCloseKey(d->hk);

	}
	catch(...){
		blnSuccess = false;
	}
	return blnSuccess;

}

int Settings::getIntSetting(int intDeviceId, const std::string &name, bool parameter) const {
	int intReturn = 0;

	std::string strSetting = getStringSetting(intDeviceId, name, parameter);
	if (strSetting.length()) {
		intReturn = (int)strSetting[0];
	}

	return intReturn;
}

bool Settings::setIntSetting(int intDeviceId, const std::string &name, int value, bool parameter) {
	bool blnReturn = false;

	std::ostringstream ssRegPath; 
	ssRegPath << d->strRegPathDevice << intDeviceId;
	std::string strCompleteRegPath = ssRegPath.str();
	long lnExists = RegOpenKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, KEY_WRITE, &d->hk);
	if (lnExists == ERROR_SUCCESS) {
		DWORD dwVal = value;
		lnExists = RegSetValueEx (d->hk, name.c_str(), 0L, REG_DWORD, (CONST BYTE*) &dwVal, sizeof(DWORD));
		if (lnExists == ERROR_SUCCESS) {
			blnReturn = true;
		}
	}
	RegCloseKey(d->hk);
	return blnReturn;
}

bool storeGlobal(privateVars *d) {
	bool blnReturn = false;
	try{
		long lnExists = RegOpenKeyEx(HKEY_LOCAL_MACHINE, d->strRegPathDevice.c_str(), 0, KEY_QUERY_VALUE, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
			
			DWORD dwLength;
			char* Buff = new char[intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(d->hk, "SharedDevices", NULL, NULL, (LPBYTE)Buff, &dwLength);

			if(lngStatus == ERROR_MORE_DATA){
				//The buffer is to small, recreate it
				delete Buff;
				Buff = new char[dwLength];

				lngStatus = RegQueryValueEx(d->hk, "SharedDevices", NULL, NULL, (LPBYTE)Buff, &dwLength);
			}

			if(lngStatus == ERROR_SUCCESS){

				int intValue = (int)Buff[0];
				blnReturn = (intValue == 1);
			}
			delete Buff;
		}
		RegCloseKey(d->hk);
	}
	catch(...){
	}
	return blnReturn;
}
