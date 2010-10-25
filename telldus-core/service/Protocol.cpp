#include "Protocol.h"
#include "../client/telldus-core.h"

#include "ProtocolBrateck.h"
#include "ProtocolEverflourish.h"
#include "ProtocolFuhaote.h"
#include "ProtocolIkea.h"
#include "ProtocolNexa.h"
#include "ProtocolRisingSun.h"
#include "ProtocolSartano.h"
#include "ProtocolSilvanChip.h"
#include "ProtocolUpm.h"
#include "ProtocolWaveman.h"
#include "ProtocolX10.h"

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

Protocol *Protocol::getProtocolInstance(const std::wstring &protocolname){
	
	if(TelldusCore::comparei(protocolname, L"arctech")){
		return new ProtocolNexa();

	} else if (TelldusCore::comparei(protocolname, L"brateck")) {
		return new ProtocolBrateck();

	} else if (TelldusCore::comparei(protocolname, L"everflourish")) {
		return new ProtocolEverflourish();

	} else if (TelldusCore::comparei(protocolname, L"fuhaote")) {
		return new ProtocolFuhaote();

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
	}

	return 0;
}

std::list<std::wstring> Protocol::getParametersForProtocol(const std::wstring &protocolName) {
	std::list<std::wstring> parameters;

	if(TelldusCore::comparei(protocolName, L"arctech")){
		parameters.push_back(L"house");
		parameters.push_back(L"unit");

	} else if (TelldusCore::comparei(protocolName, L"brateck")) {
		parameters.push_back(L"house");

	} else if (TelldusCore::comparei(protocolName, L"everflourish")) {
		parameters.push_back(L"house");
		parameters.push_back(L"unit");

	} else if (TelldusCore::comparei(protocolName, L"fuhaote")) {
		parameters.push_back(L"code");

	} else if (TelldusCore::comparei(protocolName, L"ikea")) {
		parameters.push_back(L"system");
		parameters.push_back(L"units");
		parameters.push_back(L"fade");

	} else if (TelldusCore::comparei(protocolName, L"risingsun")) {
		parameters.push_back(L"house");
		parameters.push_back(L"unit");

	} else if (TelldusCore::comparei(protocolName, L"sartano")) {
		parameters.push_back(L"code");

	} else if (TelldusCore::comparei(protocolName, L"silvanchip")) {
		parameters.push_back(L"house");

	} else if (TelldusCore::comparei(protocolName, L"upm")) {
		parameters.push_back(L"house");
		parameters.push_back(L"unit");
		
	} else if (TelldusCore::comparei(protocolName, L"waveman")) {
		parameters.push_back(L"house");
		parameters.push_back(L"unit");
	
	} else if (TelldusCore::comparei(protocolName, L"x10")) {
		parameters.push_back(L"house");
		parameters.push_back(L"unit");
	}

	return parameters;
}
