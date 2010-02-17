#include "DeviceEverflourish.h"
#include "common.h"

using namespace TelldusCore;

const unsigned char ON = 15;
const unsigned char OFF = 0;
const unsigned char LEARN = 10;

/*
* Constructor
*/
DeviceEverflourish::DeviceEverflourish(int id, const std::string &model, const std::string &name)
	:Device(id, model, name),
	intHouse(0),
	intCode(0)
{
}

/*
* Destructor
*/
DeviceEverflourish::~DeviceEverflourish(void)
{}

bool DeviceEverflourish::setHouse(const std::string &newHouse) {
	if (newHouse.length() > 0) {
		intHouse = atoi(newHouse.c_str());
	} else {
		intHouse = 0;
	}
	if (intHouse < 0) {
		intHouse = 0;
	} else if (intHouse > 16383) {
		intHouse = 16383;
	}
	return true;
}

bool DeviceEverflourish::setUnit(const std::string &newUnit) {
	if (newUnit.length() > 0) {
		intCode = atoi(newUnit.c_str()) - 1;
	} else {
		intCode = 0;
	}
	if (intCode < 0) {
		intCode = 0;
	} else if (intCode > 3) {
		intCode = 3;
	}
	return true;
}

bool DeviceEverflourish::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("house") == 0) {
		return setHouse(strValue);
	} else if (strName.compare("unit") == 0) {
		return setUnit(strValue);
	}
	return false;
}

/*
* Turn on this device
*/
int DeviceEverflourish::turnOn(void){
	try{
		std::string strCode = getCode(ON);
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

/*
* Turn off this device
*/
int DeviceEverflourish::turnOff(void){
	try{
		std::string strCode = getCode(OFF);
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}

int DeviceEverflourish::learn(void){
	try{
		std::string strCode = getCode(LEARN);
		return Device::send(strCode);
	}
	catch(...){
		throw;
	}
	return TELLSTICK_ERROR_UNKNOWN;
}


bool DeviceEverflourish::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

/*
* Has the device got the method?
*/
int DeviceEverflourish::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_LEARN);
}

std::string DeviceEverflourish::getProtocol() const {
	return "arctech";
}

std::string DeviceEverflourish::getCode(unsigned char action) {
	unsigned int deviceCode = intHouse;
	
	const char ssss = 85;
	const char sssl = 84; // 0
	const char slss = 69; // 1

	const char bits[2] = {sssl,slss};
	int i, check;

	std::string strCode;

	deviceCode = (deviceCode << 2) | intCode;
	
	check = calculateChecksum(deviceCode);

	char preamble[] = {'R', 5, 'T', 114,60,1,1,105,ssss,ssss,0};
	strCode.append(preamble);

	for(i=15;i>=0;i--) {
		strCode.append(1, bits[(deviceCode>>i)&0x01]);
	}
	for(i=3;i>=0;i--) {
		strCode.append(1, bits[(check>>i)&0x01]);
	}
	for(i=3;i>=0;i--) {
		strCode.append(1, bits[(action>>i)&0x01]);
	}
	
	strCode.append(1, ssss);
	strCode.append(1, '+');

	return strCode;
}

// The calculation used in this function is provided by Frank Stevenson
unsigned int DeviceEverflourish::calculateChecksum(unsigned int x) {
	unsigned int bits[16] = {
		0xf ,0xa ,0x7 ,0xe,
		0xf ,0xd ,0x9 ,0x1,
		0x1 ,0x2 ,0x4 ,0x8,
		0x3 ,0x6 ,0xc ,0xb
	};
	unsigned int bit = 1;
	unsigned int res = 0x5;
	int i;
	unsigned int lo,hi;

	if ((x&0x3)==3) {
		lo = x & 0x00ff;
		hi = x & 0xff00;
		lo += 4;
		if (lo>0x100) {
			lo = 0x12;
		}
		x = lo | hi;
	}

	for(i=0;i<16;i++) {
		if (x&bit) {
			res = res ^ bits[i];
		}
		bit = bit << 1;
	}

	return res; 
}
