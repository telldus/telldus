#include "EventUpdateManager.h"

class EventUpdateManager::PrivateData {
public:

};

EventUpdateManager::EventUpdateManager(){
	d = new PrivateData;
}

EventUpdateManager::~EventUpdateManager(void) {
	/*
	for (ControllerMap::iterator it = d->controllers.begin(); it != d->controllers.end(); ++it) {
		delete( it->second );
	}
	*/
	delete d;
}

void EventUpdateManager::sendUpdateMessage(int eventDeviceChanges, int eventChangeType, int eventMethod, int deviceType, int deviceId){
	/*
	for(){
		if(isalive){
			
			it++;
		}
		else{
			//ta bort
			delete *it;
			it = ngt.erase(it);
		}
	}
	*/

	//meddela alla klienter i listan 
				//eventdata - vad som har hänt
				//måste kolla så att de inte har kopplats ifrån, ta bort från listan då också
				//tömma listan vid delete, deletea respektive
}