#include "../StdAfx.h"
#include "TelldusSettings.h"
#include <sstream> 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
bool storeGlobal(privateVars *d);

class privateVars {
public:
	HKEY hk;
	HKEY rootKey;
	std::string strRegPathDevice;
	std::string strRegPath;
	int intMaxRegValueLength;
};

/*
* Constructor
*/
TelldusSettings::TelldusSettings(void)
{
	d = new privateVars();
	d->strRegPathDevice = "SOFTWARE\\Telldus\\Devices\\";
	d->strRegPath = "SOFTWARE\\Telldus\\";
	d->intMaxRegValueLength = 1000;
	if (storeGlobal(d)) {
		d->rootKey = HKEY_LOCAL_MACHINE;
	} else {
		d->rootKey = HKEY_CURRENT_USER;
	}
}

/*
* Destructor
*/
TelldusSettings::~TelldusSettings(void)
{
	//RegCloseKey(hk);	//close all, if still open //TODO: Need some way to know if open or closed
	d->strRegPath = "";
	d->strRegPathDevice = "";
	d->intMaxRegValueLength = -1;

}

/*
* Return the number of stored devices
*/
int TelldusSettings::getNumberOfDevices(void){
	
	int intNumberOfDevices = 0;
	
	try{
		
		long lnExists = RegOpenKeyEx(d->rootKey, d->strRegPathDevice.c_str(), 0, KEY_QUERY_VALUE, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
		
			string strNumSubKeys;
			DWORD dNumSubKeys;
			RegQueryInfoKey(d->hk, NULL, NULL, NULL, &dNumSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		
			intNumberOfDevices = (int)dNumSubKeys;

			RegCloseKey(d->hk);
		}
		else{
			throw exception();	//couldn't open reg key
		}
	}
	catch(...){
		intNumberOfDevices = -1;
	}
	return intNumberOfDevices;
}


int TelldusSettings::getDeviceId(int intDeviceIndex){
	int intReturn = -1;
	
	try{
		
		long lnExists = RegOpenKeyEx(d->rootKey, d->strRegPathDevice.c_str(), 0, KEY_READ, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
		
			char* Buff = new char[d->intMaxRegValueLength];
			DWORD size;
			if (RegEnumKeyEx(d->hk, intDeviceIndex, (LPSTR)Buff, &size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
				intReturn = (int)_atoi64(Buff);
			}
			
			delete Buff;

			RegCloseKey(d->hk);
		}
		else{
			throw exception();	//couldn't open reg key
		}
	}
	catch(...){
		intReturn = -1;
	}
	return intReturn;


}

/*
* Set device arguments
*/
/* bool TelldusSettings::setArguments(int intDeviceId, vector <int> vArguments){

	bool blnSuccess = true;
	try{
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		char chConvertBuffer [20];
		char chConvertBufferValue [20];
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_WRITE, &hk);

		if(lnExists == ERROR_SUCCESS){
			int i = 0;
			while(i < vArguments.size()){
				_itoa(i, chConvertBuffer, 10);
				_itoa(vArguments.at(i), chConvertBufferValue, 10);
				intMaxRegValueLength = (int)strlen(chConvertBufferValue);
				RegSetValueEx(hk, chConvertBuffer, 0, REG_SZ, (LPBYTE)chConvertBufferValue, intMaxRegValueLength);
				i++;
			}
		}
		else{
			throw exception();	//couldn't open reg key
		}
		RegCloseKey(hk);

	}
	catch(...){
		blnSuccess = false;
	}
	return blnSuccess;
}*/


/*
* Add a new device
*/
int TelldusSettings::addDevice(){

	int intDeviceId = -1;
	try{

		DWORD dwDisp;
		intDeviceId = getNextDeviceId();

		std::ostringstream ssRegPath; 
		ssRegPath << d->strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		
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
		   throw exception("Create Key failed");
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
int TelldusSettings::getNextDeviceId(){

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
			char* Buff = new char[d->intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(d->hk, "LastUsedId", NULL, NULL, (LPBYTE)Buff, &dwLength);

			if(lngStatus == ERROR_MORE_DATA){


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
bool TelldusSettings::removeDevice(int intDeviceId){

	bool blnSuccess = true;
	try{
		std::ostringstream ssRegPath; 
		ssRegPath << d->strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();

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

char *TelldusSettings::getStringSetting(int intDeviceId, const char* name, bool parameter) {
	char* strReturn = "";

	try{
		std::ostringstream ssRegPath; 
		ssRegPath << d->strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, KEY_QUERY_VALUE, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
			DWORD dwLength;
			char* Buff = new char[d->intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(d->hk, name, NULL, NULL, (LPBYTE)Buff, &dwLength);
			if(lngStatus == ERROR_MORE_DATA){
				Buff = new char[dwLength];
				lngStatus = RegQueryValueEx(d->hk, name, NULL, NULL, (LPBYTE)Buff, &dwLength);
			}
			strReturn = Buff;
		}
		else{
			throw exception();	//couldn't open reg key
		}
		RegCloseKey(d->hk);
	}
	catch(...){
		strReturn = "";
	}
	return strReturn;
}

bool TelldusSettings::setStringSetting(int intDeviceId, const char* name, const char *value, bool parameter) {

	bool blnSuccess = true;
	try{
		
		std::ostringstream ssRegPath; 
		ssRegPath << d->strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(d->rootKey, strCompleteRegPath.c_str(), 0, KEY_WRITE, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
			d->intMaxRegValueLength = (int)strlen(value);
			RegSetValueEx(d->hk, name, 0, REG_SZ, (LPBYTE)value, d->intMaxRegValueLength);
		}
		else{
			throw exception();	//couldn't open reg key
		}
		RegCloseKey(d->hk);

	}
	catch(...){
		blnSuccess = false;
	}
	return blnSuccess;

}

//only for debug reasons
void TelldusSettings::debugLog(char* debugstring){
		ofstream debugfile("c:\\telldusdebug.txt", ios::app);
		if(debugfile){
			debugfile << debugstring << endl;
			debugfile.close();
		}
}

//only for debug reasons
void TelldusSettings::debugLog(int debugint){
		ofstream debugfile("c:\\telldusdebug.txt", ios::app);
		if(debugfile){
			debugfile << debugint << endl;
			debugfile.close();
		}
}

bool storeGlobal(privateVars *d) {
	bool blnReturn = false;
	try{
		long lnExists = RegOpenKeyEx(HKEY_LOCAL_MACHINE, d->strRegPathDevice.c_str(), 0, KEY_QUERY_VALUE, &d->hk);
				
		if(lnExists == ERROR_SUCCESS){
			
			DWORD dwLength;
			char* Buff = new char[d->intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(d->hk, "SharedDevices", NULL, NULL, (LPBYTE)Buff, &dwLength);

			if(lngStatus == ERROR_MORE_DATA){


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