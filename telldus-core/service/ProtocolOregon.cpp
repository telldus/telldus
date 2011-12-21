#include "ProtocolOregon.h"
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#ifdef _MSC_VER
typedef unsigned __int8 uint8_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

std::string ProtocolOregon::decodeData(ControllerMessage &dataMsg)
{
	std::string data = dataMsg.getParameter("data");

	std::wstring model = dataMsg.model();
	if (model.compare(L"0xEA4C") == 0) {
		return decodeEA4C(data);
	} else if (model.compare(L"0x1A2D")) {
		return decode1A2D(data);
	}

	return "";
}

std::string ProtocolOregon::decodeEA4C(const std::string &data) {
	uint64_t value = strtol(data.c_str(), NULL, 16);

	uint8_t checksum = 0xE + 0xA + 0x4 + 0xC;
	checksum -= (value & 0xF) * 0x10;
	checksum -= 0xA;
	value >>= 8;

	uint8_t checksumw = (value >> 4) & 0xF;
	bool neg = value & (1 << 3);
	checksum += (value & 0xF);
	value >>= 8;

	uint8_t temp2 = value & 0xF;
	uint8_t temp1 = (value >> 4) & 0xF;
	checksum += temp2 + temp1;
	value >>= 8;

	uint8_t temp3 = (value >> 4) & 0xF;
	checksum += (value & 0xF) + temp3;
	value >>= 8;

	checksum += ((value >> 4) & 0xF) + (value & 0xF);
	uint8_t address = value & 0xFF;
	value >>= 8;

	checksum += ((value >> 4) & 0xF) + (value & 0xF);
	uint8_t channel = (value >> 4) & 0x7;

	if (checksum != checksumw) {
		return "";
	}

	double temperature = ((temp1 * 100) + (temp2 * 10) + temp3)/10.0;
	if (neg) {
		temperature = -temperature;
	}

	std::stringstream retString;
	retString << "class:sensor;protocol:oregon;model:EA4C;id:" << (int)address
		<< ";temp:" << std::fixed << std::setprecision(1) << temperature << ";";

	return retString.str();
}

std::string ProtocolOregon::decode1A2D(const std::string &data) {
	//TODO
	return "";
}
