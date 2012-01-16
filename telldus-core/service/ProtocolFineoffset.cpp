#include "ProtocolFineoffset.h"
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#ifdef _MSC_VER
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
#else
#include <stdint.h>
#endif

std::string ProtocolFineoffset::decodeData(ControllerMessage &dataMsg)
{
	std::string data = dataMsg.getParameter("data");
	if (data.length() < 8) {
		return "";
	}

	uint8_t checksum = strtol(data.substr(data.length()-2).c_str(), NULL, 16);
	data = data.substr(0, data.length()-2);

	uint8_t humidity = strtol(data.substr(data.length()-2).c_str(), NULL, 16);
	data = data.substr(0, data.length()-2);

	uint16_t value = strtol(data.substr(data.length()-3).c_str(), NULL, 16);
	double temperature = (value & 0x7FF)/10.0;

	value >>= 11;
	if (value & 1) {
		temperature = -temperature;
	}
	data = data.substr(0, data.length()-3);

	uint16_t id = strtol(data.c_str(), NULL, 16) & 0xFF;

	std::stringstream retString;
	retString << "class:sensor;protocol:fineoffset;id:" << id << ";model:";

	if (humidity <= 100) {
		retString << "temperaturehumidity;humidity:" << (int)humidity << ";";
	} else if (humidity == 0xFF) {
		retString << "temperature;";
	} else {
		return "";
	}

	retString << "temp:" << std::fixed << std::setprecision(1) << temperature << ";";

	return retString.str();
}
