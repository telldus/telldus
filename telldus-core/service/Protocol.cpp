#include "Protocol.h"
#include "../client/telldus-core.h"

#include "ProtocolBrateck.h"
#include "ProtocolNexa.h"

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

	} /*else if (wcscasecmp(protocolname.c_str(), L"everflourish") == 0){
		prot = new ProtocolEverflourish(deviceId, modelname);
		((ProtocolEverflourish*)prot)->setHouse(settings.getDeviceParameter(deviceId, "house"));
		((ProtocolEverflourish*)prot)->setUnit(settings.getDeviceParameter(deviceId, "unit"));

	} else if (wcscasecmp(protocolname.c_str(), L"fuhaote") == 0) {
		prot = new ProtocolFuhaote(deviceId, modelname);
		((ProtocolFuhaote*)prot)->setCode(settings.getDeviceParameter(deviceId, "code"));

	} else if (wcscasecmp(protocolname.c_str(), L"silvanchip") == 0) {
		prot = new ProtocolSilvanChip(deviceId, modelname);
		((ProtocolSilvanChip*)prot)->setHouse(settings.getDeviceParameter(deviceId, "house"));

	} else if (wcscasecmp(protocolname.c_str(), L"group") == 0) {
		prot = new ProtocolGroup(deviceId, modelname);
		//TODO: take a closer look
		((ProtocolGroup*)prot)->setDevices(settings.getDeviceParameter(deviceId, "devices"));
		
	} else if (wcscasecmp(protocolname.c_str(), L"risingsun") == 0) {
		prot = new ProtocolRisingSun(deviceId, modelname);
		((ProtocolRisingSun*)prot)->setHouse(settings.getDeviceParameter(deviceId, "house"));
		((ProtocolRisingSun*)prot)->setUnit(settings.getDeviceParameter(deviceId, "unit"));

	} else if (wcscasecmp(protocolname.c_str(), L"Waveman") == 0) {
		prot = new ProtocolWaveman(deviceId, modelname);
		((ProtocolWaveman*)prot)->setHouse(settings.getDeviceParameter(deviceId, "house"));
		((ProtocolWaveman*)prot)->setUnit(settings.getDeviceParameter(deviceId, "unit"));

	} else if (wcscasecmp(protocolname.c_str(), L"Sartano") == 0) {
		prot = new ProtocolSartano(deviceId, modelname);
		((ProtocolSartano*)prot)->setCode(settings.getDeviceParameter(deviceId, "code"));

	} else if (wcscasecmp(protocolname.c_str(), L"Ikea") == 0) {
		prot = new ProtocolIkea(deviceId, modelname);
		((ProtocolIkea*)prot)->setSystem(settings.getDeviceParameter(deviceId, "system"));
		((ProtocolIkea*)prot)->setUnits(settings.getDeviceParameter(deviceId, "units"));
		((ProtocolIkea*)prot)->setFade(settings.getDeviceParameter(deviceId, "fade"));

	} else if (wcscasecmp(protocolname.c_str(), L"upm") == 0) {
		prot = new ProtocolUpm(deviceId, modelname);
		((ProtocolUpm*)prot)->setHouse(settings.getDeviceParameter(deviceId, "house"));
		((ProtocolUpm*)prot)->setUnit(settings.getDeviceParameter(deviceId, "unit"));
	
	} else if (wcscasecmp(protocolname.c_str(), L"x10") == 0) {
		prot = new ProtocolX10(deviceId, modelname);
		((ProtocolX10*)prot)->setHouse(settings.getDeviceParameter(deviceId, "house"));
		((ProtocolX10*)prot)->setUnit(settings.getDeviceParameter(deviceId, "unit"));

	}
	*/
	else{
		return 0;
	}
}

