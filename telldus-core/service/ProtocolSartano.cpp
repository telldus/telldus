#include "ProtocolSartano.h"
#include <sstream>
#include <stdio.h>

int ProtocolSartano::methods() const {
	return TELLSTICK_TURNON | TELLSTICK_TURNOFF;
}

std::string ProtocolSartano::getStringForMethod(int method, unsigned char, Controller *) {
	std::wstring strCode = this->getStringParameter(L"code", L"");
	return getStringForCode(strCode, method);
}

std::string ProtocolSartano::getStringForCode(const std::wstring &strCode, int method) {

	std::string strReturn("S");

	for (size_t i = 0; i < strCode.length(); ++i) {
		if (strCode[i] == L'1') {
			strReturn.append("$k$k");
		} else {
			strReturn.append("$kk$");
		}
	}

	if (method == TELLSTICK_TURNON) {
		strReturn.append("$k$k$kk$$k+");
	} else if (method == TELLSTICK_TURNOFF) {
		strReturn.append("$kk$$k$k$k+");
	} else {
		return "";
	}

	return strReturn;

}

std::string ProtocolSartano::decodeData(ControllerMessage &dataMsg)
{
	std::string data = dataMsg.getParameter("data");
	signed int allDataIn;
	signed int allData = 0;
	unsigned int code = 0;
	unsigned int method1 = 0;
	unsigned int method2 = 0;
	unsigned int method = 0;
	
	sscanf(data.c_str(), "%X", &allDataIn);
	
	unsigned long mask = (1<<11);
	for(int i=0;i<12;++i){
		allData >>= 1;
		if((allDataIn & mask) == 0){
			allData |= (1<<11);
		}
		mask >>= 1;
	}
	
	code = allData & 0xFFC;
	code >>= 2;
	
	method1 = allData & 0x2;
	method1 >>= 1;
	
	method2 = allData & 0x1;
	
	if(method1 == 0 && method2 == 1){
		method = 0;  //off
	}
	else if(method1 == 1 && method2 == 0){
		method = 1; //on
	}
	else{
		return "";
	}
	
	if(code < 0 || code > 1023){
		//not sartano
		return "";
	}
	
	std::stringstream retString;
	retString << "class:command;protocol:sartano;model:codeswitch;code:";
	mask = (1<<9);
	for(int i=0;i<10;i++){
		if((code & mask) != 0){
			retString << 1;
		}
		else{
			retString << 0;
		}
		mask >>= 1;
	}
	retString << ";method:";
	
	if(method == 0){
		retString << "turnoff;";
	}
	else{
		retString << "turnon;";
	}
	
	return retString.str();
}
