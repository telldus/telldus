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

#include <algorithm>
#include <string>
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

int Protocol::methods() const {
	return TELLSTICK_TURNON | TELLSTICK_TURNOFF;
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

/**
* This method doesn't support all locales
*/
bool Protocol::comparei(std::wstring stringA , std::wstring stringB)
{
	//move?
	transform(stringA.begin(), stringA.end(), stringA.begin(), toupper);
	transform(stringB.begin(), stringB.end(), stringB.begin(), toupper);

	return stringA == stringB;
}

Protocol *Protocol::getProtocolInstance(std::wstring &protocolname){
	
	if(comparei(protocolname, L"arctech")){
		return new ProtocolNexa();

	} else if (comparei(protocolname, L"brateck")) {
		return new ProtocolBrateck();

	} else if (comparei(protocolname, L"everflourish")) {
		return new ProtocolEverflourish();

	} else if (comparei(protocolname, L"fuhaote")) {
		return new ProtocolFuhaote();

	} else if (comparei(protocolname, L"ikea")) {
		return new ProtocolIkea();

	} else if (comparei(protocolname, L"risingsun")) {
		return new ProtocolRisingSun();

	} else if (comparei(protocolname, L"sartano")) {
		return new ProtocolSartano();

	} else if (comparei(protocolname, L"silvanchip")) {
		return new ProtocolSilvanChip();

	} else if (comparei(protocolname, L"upm")) {
		return new ProtocolUpm();
		
	} else if (comparei(protocolname, L"waveman")) {
		return new ProtocolWaveman();
	
	} else if (comparei(protocolname, L"x10")) {
		return new ProtocolX10();
	}

	return 0;
}

