#include "DeviceSilvanChip.h"
#include <string>

using namespace TelldusCore;

/*
* Constructor
*/
DeviceSilvanChip::DeviceSilvanChip(int id, const std::string &model, const std::string &name)
	:Device(id, model, name)
{
}


DeviceSilvanChip::~DeviceSilvanChip(void)
{
}

bool DeviceSilvanChip::setHouse(const std::string &strNewHouse) {
	intHouse = atoi(strNewHouse.c_str());
	return true;
}

bool DeviceSilvanChip::setDeviceParameter(const std::string &strName, const std::string &strValue) {
	if (strName.compare("house") == 0) {
		return setHouse(strValue);
	}
	return false;
}

/*
* Turn on this device
*/
int DeviceSilvanChip::turnOn(Controller *controller){

	std::string strCode = getStringCode(controller, TELLSTICK_TURNON);
		
	return controller->send(strCode);
}

/*
* Turn off this device
*/
int DeviceSilvanChip::turnOff(Controller *controller){
	std::string strCode = getStringCode(controller, TELLSTICK_TURNOFF);
	
	return controller->send(strCode);
}

int DeviceSilvanChip::learn(Controller *controller) {
	std::string strCode = getStringCode(controller, TELLSTICK_LEARN);
	
	return controller->send(strCode);
}

bool DeviceSilvanChip::parameterMatches( const std::string &name, const std::string &value ) const {
	return false;
}

/*
* Has the device got the method?
*/
int DeviceSilvanChip::methods(){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF | TELLSTICK_LEARN);
}

std::string DeviceSilvanChip::getStringCode(Controller *controller, int method){
	const unsigned char S = 100;
	const unsigned char L = 255;
	const std::string LONG = "\255\1\200";

	const std::string ONE = LONG + "\100";
	const std::string ZERO = "\100" + LONG;
	std::string strReturn = "R\1S";

	strReturn.append(1, S);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, L);
	strReturn.append(1, 1);
	strReturn.append(1, S);

	for( int i = 19; i >= 0; --i ) {
		if (intHouse & (1 << i)) {
			strReturn.append(ONE);
		} else {
			strReturn.append(ZERO);
		}
	}

	if (method == TELLSTICK_TURNON) {
		strReturn.append(ZERO);
		strReturn.append(ZERO);
		strReturn.append(ONE);
		strReturn.append(ZERO);
	} else if (method == TELLSTICK_LEARN) {
		strReturn.append(ZERO);
		strReturn.append(ZERO);
		strReturn.append(ZERO);
		strReturn.append(ONE);
	} else {
		strReturn.append(ONE);
		strReturn.append(ZERO);
		strReturn.append(ZERO);
		strReturn.append(ZERO);
	}

	strReturn.append(ZERO);
	strReturn.append("+");
	return strReturn;

}

std::string DeviceSilvanChip::getProtocol() const {
	return "silvanchip";
}

