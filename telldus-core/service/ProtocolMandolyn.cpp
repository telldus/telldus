#include "ProtocolMandolyn.h"
#include "Strings.h"
#include <stdlib.h>
#include <sstream>
#include <iomanip>

std::string ProtocolMandolyn::decodeData(ControllerMessage &dataMsg)
{
	std::string data = dataMsg.getParameter("data");
	uint32_t value = (uint32_t)TelldusCore::hexTo64l(data);

	bool parity = value & 0x1;
	value >>= 1;

	double temp = (double)(value & 0x7FFF) - (double)6400;
	temp = temp/128.0;
	value >>= 15;

	uint8_t humidity = (value & 0x7F);
	value >>= 7;

	bool battOk = value & 0x1;
	value >>= 3;

	uint8_t channel = (value & 0x3)+1;
	value >>= 2;

	uint8_t house = value & 0xF;

	std::stringstream retString;
	retString << "class:sensor;protocol:mandolyn;id:"
		<< house*10+channel
		<< ";model:temperaturehumidity;"
		<< "temp:" << std::fixed << std::setprecision(1) << temp
		<< ";humidity:" << (int)humidity << ";";

	return retString.str();
}
