#include "StdAfx.h"
#include "TelldusSettings.h"
#include "Device.h"
#include "DeviceNexa.h"
#include "DeviceWaveman.h"
#include "DeviceSartano.h"
#include "DeviceIkea.h"
#include <sstream> 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

/*
* Constructor
*/
TelldusSettings::TelldusSettings(void)
{
	strRegPathDevice = "SOFTWARE\\Telldus\\Devices\\";
	strRegPath = "SOFTWARE\\Telldus\\";
	
	intMaxRegValueLength = 1000;
}

/*
* Destructor
*/
TelldusSettings::~TelldusSettings(void)
{
	//RegCloseKey(hk);	//close all, if still open //TODO: Need some way to know if open or closed
	strRegPath = "";
	strRegPathDevice = "";
	intMaxRegValueLength = -1;

}

/*
* Return the number of stored devices
*/
int TelldusSettings::getNumberOfDevices(void){
	
	int intNumberOfDevices = 0;
	
	try{
		
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strRegPathDevice.c_str(), 0, KEY_QUERY_VALUE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
		
			string strNumSubKeys;
			DWORD dNumSubKeys;
			RegQueryInfoKey(hk, NULL, NULL, NULL, &dNumSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		
			intNumberOfDevices = (int)dNumSubKeys;

			RegCloseKey(hk);
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

/*
* Get the requested device
*/
Device* TelldusSettings::getDevice(int intDeviceId){

	try{
		int intDongleIndex = Device::getDongleIndex();
		if(intDongleIndex != -1){
			return getDevice(intDeviceId, intDongleIndex);
		}
		else{
			return NULL;
		}
	}
	catch(...){
		throw;
	}
}

/*
* Get the requested device, when the index of the USB dongle is known
* Note that the returned Device should be deleted when not in use anymore
*/
Device* TelldusSettings::getDevice(int intDeviceId, int intDongleIndex){

	Device* dev = 0;
	
	try{
		char* vendor = getVendor(intDeviceId);
		
		int* args = getArguments(intDeviceId);
		
		//each new brand must be added here
		if (strcmp(vendor, "Nexa") == 0){
			dev = new DeviceNexa((int)args[0], (int)args[1], intDongleIndex);
		} else if (strcmp(vendor, "Waveman") == 0) {
			dev = new DeviceWaveman((int)args[0], (int)args[1], intDongleIndex);
		} else if (strcmp(vendor, "Sartano") == 0) {
			dev = new DeviceSartano((int)args[0], (int)args[1], intDongleIndex);
		} else if (strcmp(vendor, "Ikea") == 0) {
			dev = new DeviceIkea((int)args[0], (int)args[1], (int)args[2], intDongleIndex);
		}
	}
	catch(...){
		throw;
	}
	return dev;
}

/*
* Get the name of the device
*/
char* TelldusSettings::getName(int intDeviceId){

	char* strReturn = "";

	try{
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_QUERY_VALUE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
			DWORD dwLength;
			char* Buff = new char[intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(hk, "Name", NULL, NULL, (LPBYTE)Buff, &dwLength);
			if(lngStatus == ERROR_MORE_DATA){
				Buff = new char[dwLength];
				lngStatus = RegQueryValueEx(hk, "Name", NULL, NULL, (LPBYTE)Buff, &dwLength);
			}
			strReturn = Buff;
		}
		else{
			throw exception();	//couldn't open reg key
		}
		RegCloseKey(hk);
	}
	catch(...){
		strReturn = "";
	}
	return strReturn;
}

/*
* Set the name of the device
*/
bool TelldusSettings::setName(int intDeviceId, char* strNewName){

	bool blnSuccess = true;
	try{
		
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_WRITE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
			intMaxRegValueLength = (int)strlen(strNewName);
			RegSetValueEx(hk, "Name", 0, REG_SZ, (LPBYTE)strNewName, intMaxRegValueLength);
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
}

/*
* Get the device vendor
*/
char* TelldusSettings::getVendor(int intDeviceId){

	char* strReturn = "";

	try{
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, (LPCSTR)strCompleteRegPath.c_str(), 0, KEY_QUERY_VALUE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
			DWORD dwLength;
			char* Buff = new char[intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(hk, (LPCSTR)"Vendor", NULL, NULL, (LPBYTE)Buff, &dwLength);
			if(lngStatus == ERROR_MORE_DATA){
				Buff = new char[dwLength];
				lngStatus = RegQueryValueEx(hk, (LPCSTR)"Vendor", NULL, NULL, (LPBYTE)Buff, &dwLength);
			}

			strReturn = Buff;	
		}
		else{
			throw exception();	//couldn't open reg key
		}
		RegCloseKey(hk);
	}
	catch(exception e){
		strReturn = "";
		ofstream errorfile("c:\\errorlog.txt", ios::app);
		if(errorfile){
			errorfile << e.what() << endl;
			errorfile.close();
		}
	}
	return strReturn;
}

/*
* Set the device vendor
*/
bool TelldusSettings::setVendor(int intDeviceId, char* strVendor){

	bool blnSuccess = true;
	try{
		
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_WRITE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
		intMaxRegValueLength = (int)strlen(strVendor);
			RegSetValueEx(hk, "Vendor", 0, REG_SZ, (LPBYTE)strVendor, intMaxRegValueLength);
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
}

/*
* Get the device model
*/
char* TelldusSettings::getModel(int intDeviceId){

	char* strReturn = "";

	try{
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_QUERY_VALUE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
			DWORD dwLength;
			char* Buff = new char[intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(hk, "Model", NULL, NULL, (LPBYTE)Buff, &dwLength);
			if(lngStatus == ERROR_MORE_DATA){

				Buff = new char[dwLength];
				lngStatus = RegQueryValueEx(hk, "Model", NULL, NULL, (LPBYTE)Buff, &dwLength);
			}
			strReturn = Buff;	
		}
		else{
			throw exception();	//couldn't open reg key
		}
		RegCloseKey(hk);
	}
	catch(...){
		strReturn = "";
	}
	return strReturn;
}

/*
* Set the device model
*/
bool TelldusSettings::setModel(int intDeviceId, char* strVendor){

	bool blnSuccess = true;
	try{
		
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_WRITE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
		intMaxRegValueLength = (int)strlen(strVendor);
			RegSetValueEx(hk, "Model", 0, REG_SZ, (LPBYTE)strVendor, intMaxRegValueLength);
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
}

int TelldusSettings::getDeviceId(int intDeviceIndex){
	int intReturn = -1;
	
	try{
		
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strRegPathDevice.c_str(), 0, KEY_READ, &hk);
				
		if(lnExists == ERROR_SUCCESS){
		
			char* Buff = new char[intMaxRegValueLength];
			DWORD size;
			if (RegEnumKeyEx(hk, intDeviceIndex, (LPSTR)Buff, &size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
				intReturn = (int)_atoi64(Buff);
			}
			
			delete Buff;

			RegCloseKey(hk);
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
* Get number of device arguments
*/
int TelldusSettings::getNumberOfArguments(int intDeviceId){
	int intReturn = -1;

	try{
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_QUERY_VALUE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
			DWORD dNumValues;
			RegQueryInfoKey(hk, NULL, NULL, NULL, NULL, NULL, NULL, &dNumValues, NULL, NULL, NULL, NULL);
			intReturn = (int)dNumValues - 3;	//total number of values - model, name and vendor
		}
		else{
			throw exception();	//couldn't open reg key
		}
		RegCloseKey(hk);
	}
	catch(...){
		//error management
	}
	
	return intReturn;
}

/*
* Get device arguments
*/
int* TelldusSettings::getArguments(int intDeviceId){
	vector <int> vReturn;
	int* intReturn = new int[];

	try{
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_QUERY_VALUE, &hk);
				
		if(lnExists == ERROR_SUCCESS){
			DWORD dNumValues;
			RegQueryInfoKey(hk, NULL, NULL, NULL, NULL, NULL, NULL, &dNumValues, NULL, NULL, NULL, NULL);
		
			int intNumberOfArguments = (int)dNumValues - 3;	//total number of values - model, name and vendor
			DWORD dwLength;
			char chConvertBuffer[20];
			int i = 0;

			while(i < intNumberOfArguments){
				
				char* Buff = new char[intMaxRegValueLength];
				
				_itoa(i, chConvertBuffer, 10);
				long lngStatus = RegQueryValueEx(hk, chConvertBuffer, NULL, NULL, (LPBYTE)Buff, &dwLength);
				if(lngStatus == ERROR_MORE_DATA){
					lngStatus = RegQueryValueEx(hk, chConvertBuffer, NULL, NULL, (LPBYTE)Buff, &dwLength);
				}
				int intReturn = (int)_atoi64(Buff);
				vReturn.push_back(intReturn);
				
				i++;
				delete Buff;
			}
		}
		else{
			throw exception();	//couldn't open reg key
		}
		RegCloseKey(hk);

		intReturn = new int[vReturn.size()];

		int i = 0;
		while(i < (int)vReturn.size()){
			intReturn[i] = vReturn.at(i);
			i++;
		}
	}
	catch(...){
		//error management
	}
	
	return intReturn;
}

/*
* Set device arguments
*/
bool TelldusSettings::setArguments(int intDeviceId, int* intArguments[], int intNumberOfArguments){

	bool blnSuccess = true;
	try{
		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		char chConvertBuffer [20];
		long lnExists = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteRegPath.c_str(), 0, KEY_WRITE, &hk);

		if(lnExists == ERROR_SUCCESS){
			int i = 0;
			while(i < intNumberOfArguments){
				_itoa(i, chConvertBuffer, 10);
				RegSetValueEx(hk, chConvertBuffer, 0, REG_SZ, (LPBYTE)intArguments[i], intMaxRegValueLength);
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
}

/*
* Set device arguments
*/
bool TelldusSettings::setArguments(int intDeviceId, vector <int> vArguments){

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
}


/*
* Add a new device
*/
int TelldusSettings::addDevice(){

	int intDeviceId = -1;
	try{

		DWORD dwDisp;
		intDeviceId = getNextDeviceId();

		std::ostringstream ssRegPath; 
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();
		
		if(RegCreateKeyEx(HKEY_CURRENT_USER,
				   strCompleteRegPath.c_str(),
				   0,
				   NULL,
				   REG_OPTION_NON_VOLATILE,
				   KEY_ALL_ACCESS,
				   NULL,
				   &hk,
				   &dwDisp)){
			//fail
		   throw exception("Create Key failed");
		}

		RegCloseKey(hk);

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
		long lnExists = RegCreateKeyEx(HKEY_CURRENT_USER, strRegPathDevice.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE,
				   KEY_ALL_ACCESS,
				   NULL,
				   &hk,
				   &dwDisp);	//create or open if already created
				
		if(lnExists == ERROR_SUCCESS){
			
			DWORD dwLength;
			char* Buff = new char[intMaxRegValueLength];

			long lngStatus = RegQueryValueEx(hk, "LastUsedId", NULL, NULL, (LPBYTE)Buff, &dwLength);

			if(lngStatus == ERROR_MORE_DATA){


				Buff = new char[dwLength];
				lngStatus = RegQueryValueEx(hk, "LastUsedId", NULL, NULL, (LPBYTE)Buff, &dwLength);
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
			
			RegSetValueEx (hk, "LastUsedId", 0L, REG_DWORD, (CONST BYTE*) &dwVal, sizeof(DWORD));
			
		}
		RegCloseKey(hk);
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
		ssRegPath << strRegPathDevice << intDeviceId;
		string strCompleteRegPath = ssRegPath.str();

		long lngSuccess = RegDeleteKey(HKEY_CURRENT_USER, strCompleteRegPath.c_str());
		if(lngSuccess != ERROR_SUCCESS){
			blnSuccess = false;
		}
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