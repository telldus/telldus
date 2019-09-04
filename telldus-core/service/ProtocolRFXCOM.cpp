//
// Copyright (C) 2012 Telldus Technologies AB. All rights reserved.
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "service/ProtocolRFXCOM.h"
#ifdef _MSC_VER
#else
#include <stdint.h>
#endif
#include <stdio.h>
#include <sstream>
#include <string>
#include "common/Strings.h"


std::string ProtocolRFXCOM::decodeData(const ControllerMessage& dataMsg) {
        std::string data = dataMsg.getParameter("data");

        // Checksum
	data = data.substr(0, data.length()-1);
	
        // Data packet type
        uint8_t type = (uint8_t)TelldusCore::hexTo64l(data.substr(data.length()-1)) & 0xF;
	data = data.substr(0, data.length()-1);

        // 2^16 part of counter
	uint8_t counts16 = (uint8_t)TelldusCore::hexTo64l(data.substr(data.length()-2)) & 0xFF;
	data = data.substr(0, data.length()-2);

        // 2^0 part of counter
	uint8_t counts   = (uint8_t)TelldusCore::hexTo64l(data.substr(data.length()-2)) & 0xFF;
	data = data.substr(0, data.length()-2);

        // 2^8 part of counter
        uint8_t counts8  = (uint8_t)TelldusCore::hexTo64l(data.substr(data.length()-2)) & 0xFF;
	data = data.substr(0, data.length()-2);
        
        // Sum the full 24-bit counter field
        uint64_t counts = counts16*65536 + counts8*256 + counts;

	data = data.substr(0, data.length()-2);

        // Sensor ID
        uint8_t id = (uint8_t)TelldusCore::hexTo64l(data.substr(data.length()-2)) & 0xFF;


        std::stringstream retString;
        retString << "class:sensor;protocol:rfxcom;id:" << static_cast<int>(id);
        retString << ";model:rfxmeter;counter:" << static_cast<int>(counts);
        retString << ";";

	return retString.str();
}
