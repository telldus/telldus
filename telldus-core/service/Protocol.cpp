#include "Protocol.h"
#include "../client/telldus-core.h"

#include "ControllerMessage.h"
#include "ProtocolBrateck.h"
#include "ProtocolComen.h"
#include "ProtocolEverflourish.h"
#include "ProtocolFineoffset.h"
#include "ProtocolFuhaote.h"
#include "ProtocolGroup.h"
#include "ProtocolHasta.h"
#include "ProtocolIkea.h"
#include "ProtocolMandolyn.h"
#include "ProtocolNexa.h"
#include "ProtocolOregon.h"
#include "ProtocolRisingSun.h"
#include "ProtocolSartano.h"
#include "ProtocolScene.h"
#include "ProtocolSilvanChip.h"
#include "ProtocolUpm.h"
#include "ProtocolWaveman.h"
#include "ProtocolX10.h"
#include "ProtocolYidong.h"

#include "Strings.h"
#include <sstream>

class Protocol::PrivateData {
public:
	ParameterMap parameterList;
	std::wstring model;
};

Protocol::Protocol(){

	d = new PrivateData;
}

Protocol::~Protocol(void) {
	delete d;
}

std::wstring Protocol::model() const {
	std::wstring strModel = d->model;
	//Strip anything after : if it is found
	size_t pos = strModel.find(L":");
	if (pos != std::wstring::npos) {
		strModel = strModel.substr(0, pos);
	}

	return strModel;
}

void Protocol::setModel(const std::wstring &model){
	d->model = model;
}

void Protocol::setParameters(ParameterMap &parameterList){
	d->parameterList = parameterList;
}

std::wstring Protocol::getStringParameter(const std::wstring &name, const std::wstring &defaultValue) const {
	ParameterMap::const_iterator it = d->parameterList.find(name);
	if (it == d->parameterList.end()) {
		return defaultValue;
	}
	return it->second;
}

int Protocol::getIntParameter(const std::wstring &name, int min, int max) const {
	std::wstring value = getStringParameter(name, L"");
	if (value == L"") {
		return min;
	}
	std::wstringstream st;
	st << value;
	int intValue = 0;
	st >> intValue;
	if (intValue < min) {
		return min;
	}
	if (intValue > max) {
		return max;
	}
	return intValue;
}

bool Protocol::checkBit(int data, int bitno) {
	return ((data>>bitno)&0x01);
}


Protocol *Protocol::getProtocolInstance(const std::wstring &protocolname){

	if(TelldusCore::comparei(protocolname, L"arctech")){
		return new ProtocolNexa();

	} else if (TelldusCore::comparei(protocolname, L"brateck")) {
		return new ProtocolBrateck();

	} else if (TelldusCore::comparei(protocolname, L"comen")) {
		return new ProtocolComen();

	} else if (TelldusCore::comparei(protocolname, L"everflourish")) {
		return new ProtocolEverflourish();

	} else if (TelldusCore::comparei(protocolname, L"fuhaote")) {
		return new ProtocolFuhaote();

	} else if (TelldusCore::comparei(protocolname, L"hasta")) {
		return new ProtocolHasta();

	} else if (TelldusCore::comparei(protocolname, L"ikea")) {
		return new ProtocolIkea();

	} else if (TelldusCore::comparei(protocolname, L"risingsun")) {
		return new ProtocolRisingSun();

	} else if (TelldusCore::comparei(protocolname, L"sartano")) {
		return new ProtocolSartano();

	} else if (TelldusCore::comparei(protocolname, L"silvanchip")) {
		return new ProtocolSilvanChip();

	} else if (TelldusCore::comparei(protocolname, L"upm")) {
		return new ProtocolUpm();

	} else if (TelldusCore::comparei(protocolname, L"waveman")) {
		return new ProtocolWaveman();

	} else if (TelldusCore::comparei(protocolname, L"x10")) {
		return new ProtocolX10();

	} else if (TelldusCore::comparei(protocolname, L"yidong")) {
		return new ProtocolYidong();

	} else if (TelldusCore::comparei(protocolname, L"group")) {
		return new ProtocolGroup();
	}

	else if (TelldusCore::comparei(protocolname, L"scene")) {
		return new ProtocolScene();
	}

	return 0;
}

std::list<std::string> Protocol::getParametersForProtocol(const std::wstring &protocolName) {
	std::list<std::string> parameters;
	if(TelldusCore::comparei(protocolName, L"arctech")){
		parameters.push_back("house");
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"brateck")) {
		parameters.push_back("house");

	} else if (TelldusCore::comparei(protocolName, L"comen")) {
		parameters.push_back("house");
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"everflourish")) {
		parameters.push_back("house");
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"fuhaote")) {
		parameters.push_back("code");

	} else if (TelldusCore::comparei(protocolName, L"hasta")) {
		parameters.push_back("house");
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"ikea")) {
		parameters.push_back("system");
		parameters.push_back("units");
		//parameters.push_back("fade");

	} else if (TelldusCore::comparei(protocolName, L"risingsun")) {
		parameters.push_back("house");
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"sartano")) {
		parameters.push_back("code");

	} else if (TelldusCore::comparei(protocolName, L"silvanchip")) {
		parameters.push_back("house");

	} else if (TelldusCore::comparei(protocolName, L"upm")) {
		parameters.push_back("house");
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"waveman")) {
		parameters.push_back("house");
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"x10")) {
		parameters.push_back("house");
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"yidong")) {
		parameters.push_back("unit");

	} else if (TelldusCore::comparei(protocolName, L"group")) {
		parameters.push_back("devices");

	}  else if (TelldusCore::comparei(protocolName, L"scene")) {
		parameters.push_back("devices");
	}

	return parameters;
}

std::list<std::string> Protocol::decodeData(const std::string &fullData) {
	std::list<std::string> retval;
	std::string decoded = "";

	ControllerMessage dataMsg(fullData);
	if( TelldusCore::comparei(dataMsg.protocol(), L"arctech") ) {
		decoded = ProtocolNexa::decodeData(dataMsg);
		if (decoded != "") {
			retval.push_back(decoded);
		}
		decoded = ProtocolWaveman::decodeData(dataMsg);
		if (decoded != "") {
			retval.push_back(decoded);
		}
		decoded = ProtocolSartano::decodeData(dataMsg);
		if (decoded != "") {
			retval.push_back(decoded);
		}
	}
	else if(TelldusCore::comparei(dataMsg.protocol(), L"everflourish") ) {
		decoded = ProtocolEverflourish::decodeData(dataMsg);
		if (decoded != "") {
			retval.push_back(decoded);
		}
	}
	else if(TelldusCore::comparei(dataMsg.protocol(), L"fineoffset") ) {
		decoded = ProtocolFineoffset::decodeData(dataMsg);
		if (decoded != "") {
			retval.push_back(decoded);
		}
	}
	else if(TelldusCore::comparei(dataMsg.protocol(), L"mandolyn") ) {
		decoded = ProtocolMandolyn::decodeData(dataMsg);
		if (decoded != "") {
			retval.push_back(decoded);
		}
	}
	else if(TelldusCore::comparei(dataMsg.protocol(), L"oregon") ) {
		decoded = ProtocolOregon::decodeData(dataMsg);
		if (decoded != "") {
			retval.push_back(decoded);
		}
	}
	else if(TelldusCore::comparei(dataMsg.protocol(), L"x10") ) {
		decoded = ProtocolX10::decodeData(dataMsg);
		if (decoded != "") {
			retval.push_back(decoded);
		}
	}

	return retval;
}
