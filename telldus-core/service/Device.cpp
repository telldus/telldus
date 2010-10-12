#include "Device.h"

class Device::PrivateData {
public:
	std::wstring name;
	Protocol* protocol;
};

Device::Device(){

	d = new PrivateData;
	//vid skapande, hämta settings från registret, vissa sätts i protokollet, vid dess skapande
	//när något uppdateras, spara också till registret
	//Denna klass har alla metoder (turnOn, turnOff etc)... 
	//Men t.ex. att om modellen är bell, då ska turnon returnera bell... eller isDimmer, ska returnera annat... hur göra? - låt vara i samma klass till att börja med
	//Men skulle egentligen vilja ha tagit ställning till modell redan i initieringen... åtminstone spara undan det i en egen variabel
}

Device::~Device(void) {
	delete d->protocol;
	delete d;
}

std::wstring Device::getName(){
	return d->name;
}

int Device::turnOn(Controller *controller) {
	Protocol *p = this->retrieveProtocol();

	//p->turnOn(controller);
	return 0;
}

Protocol *Device::retrieveProtocol() {
	if (d->protocol) {
		return d->protocol;
	}

	d->protocol = new Protocol();
	return d->protocol;
}