#pragma once
#include "device.h"
#include "devicenexa.h"
#include <string>
#include <vector>

using namespace std;

class TelldusSettings
{
public:
	TelldusSettings(void);
	int getNumberOfDevices(void);
	Device* getDevice(int intDeviceId);
	Device* getDevice(int intDeviceId, int intDongleIndex);
	char* getName(int intDeviceId);
	bool setName(int intDeviceId, char* strNewName);
	char* getVendor(int intDeviceId);
	bool setVendor(int intDeviceId, char* strVendor);
	char* getModel(int intDeviceId);
	bool setModel(int intDeviceId, char* strModel);
	int* getArguments(int intDeviceId);
	bool setArguments(int intDeviceId, int* intArguments[], int intNumberOfArguments);
	bool setArguments(int intDeviceId, vector <int> vArguments);
	int addDevice();
	int getDeviceId(int intDeviceIndex);
	bool removeDevice(int intDeviceId);
	void debugLog(char* debugstring);
	void debugLog(int debugint);
	int getNumberOfArguments(int intDeviceId);

	~TelldusSettings(void);

private:
	int getNextDeviceId();
	bool storeGlobal();

	//variables
	HKEY hk;
	HKEY rootKey;
	std::string strRegPathDevice;
	std::string strRegPath;
	int intMaxRegValueLength;
};
