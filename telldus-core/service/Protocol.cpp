#include "protocol.h"
#include "Settings.h"

#include "ProtocolNexa.h"
#include <algorithm>
#include <string>

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

void Protocol::setModel(const std::wstring &model){
	d->model = model;
}

void Protocol::setParameters(ParameterMap &parameterList){
	d->parameterList = parameterList;
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
	
	//TODO: tr1:shared_ptr...
	Protocol *prot;
	if(comparei(protocolname, L"arctech")){	//TODO: Test this
		prot = new ProtocolNexa();
		//((ProtocolNexa*)prot)->setHouse(settings.getDeviceParameter(deviceId, "house"));	//todo: try to do this in ProtocolNexa instead
		//((ProtocolNexa*)prot)->setUnit(settings.getDeviceParameter(deviceId, "unit"));

	} 
	/*else if (wcscasecmp(protocolname.c_str(), L"brateck") == 0) {
		prot = new ProtocolBrateck(deviceId, modelname);
		((ProtocolBrateck*)prot)->setHouse(settings.getDeviceParameter(deviceId, "house"));

	} else if (wcscasecmp(protocolname.c_str(), L"everflourish") == 0){
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

	return prot;	//TODO can be null
}

