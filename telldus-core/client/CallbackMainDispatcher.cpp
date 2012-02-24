/*
 *  CallbackMainDispatcher.cpp
 *  telldus-core
 *
 *  Created by Stefan Persson on 2012-02-23.
 *  Copyright 2012 Telldus Technologies AB. All rights reserved.
 *
 */

#include "CallbackMainDispatcher.h"

#include <list>

using namespace TelldusCore;

class CallbackMainDispatcher::PrivateData {
public:
	EventHandler eventHandler;
	EventRef stopEvent, generalCallbackEvent, janitor;

	std::list<std::tr1::shared_ptr<TDDeviceEventDispatcher> > deviceEventThreadList;
	std::list<std::tr1::shared_ptr<TDDeviceChangeEventDispatcher> > deviceChangeEventThreadList;
	std::list<std::tr1::shared_ptr<TDRawDeviceEventDispatcher> > rawDeviceEventThreadList;
	std::list<std::tr1::shared_ptr<TDSensorEventDispatcher> > sensorEventThreadList;

	Mutex mutex;
};

CallbackMainDispatcher::CallbackMainDispatcher()
:Thread()
{
	d = new PrivateData;
	d->stopEvent = d->eventHandler.addEvent();
	d->generalCallbackEvent = d->eventHandler.addEvent();
	d->janitor = d->eventHandler.addEvent(); //Used for cleanups
}

CallbackMainDispatcher::~CallbackMainDispatcher(void){
	d->stopEvent->signal();
	wait();
	{
		MutexLocker locker(&d->mutex);
	}
	delete d;
}

EventRef CallbackMainDispatcher::retrieveCallbackEvent(){
	return d->generalCallbackEvent;
}

void CallbackMainDispatcher::run(){

	while(!d->stopEvent->isSignaled()){
		if (!d->eventHandler.waitForAny()) {
			continue;
		}

		if(d->generalCallbackEvent->isSignaled()){
			EventDataRef eventData = d->generalCallbackEvent->takeSignal();
			
			DeviceEventCallbackData *decd = dynamic_cast<DeviceEventCallbackData*>(eventData.get());
			if(decd){
				std::tr1::shared_ptr<TDDeviceEventDispatcher> ptr(new TDDeviceEventDispatcher(decd->data, decd->deviceId, decd->deviceState, decd->deviceStateValue, d->janitor));
				MutexLocker locker(&d->mutex);
				d->deviceEventThreadList.push_back(ptr);
				continue;
			}

			DeviceChangeEventCallbackData *dcecd = dynamic_cast<DeviceChangeEventCallbackData*>(eventData.get());
			if(dcecd){
				std::tr1::shared_ptr<TDDeviceChangeEventDispatcher> ptr(new TDDeviceChangeEventDispatcher(dcecd->data, dcecd->deviceId, dcecd->eventDeviceChanges, dcecd->eventChangeType, d->janitor));
				MutexLocker locker(&d->mutex);
				d->deviceChangeEventThreadList.push_back(ptr);
				continue;
			}
			
			RawDeviceEventCallbackData *rdecd = dynamic_cast<RawDeviceEventCallbackData*>(eventData.get());
			if(rdecd){
				std::tr1::shared_ptr<TDRawDeviceEventDispatcher> ptr(new TDRawDeviceEventDispatcher(rdecd->data, rdecd->command, rdecd->controllerId, d->janitor));
				MutexLocker locker(&d->mutex);
				d->rawDeviceEventThreadList.push_back(ptr);
				continue;
			}
			
			SensorEventCallbackData *secd = dynamic_cast<SensorEventCallbackData*>(eventData.get());
			if(secd){
				std::tr1::shared_ptr<TDSensorEventDispatcher> ptr(new TDSensorEventDispatcher(secd->data, secd->protocol, secd->model, secd->id, secd->dataType, secd->value, secd->timestamp, d->janitor));
				MutexLocker locker(&d->mutex);
				d->sensorEventThreadList.push_back(ptr);
				continue;
			}
		}
		if (d->janitor->isSignaled()) {
			//Clear all of them if there is more than one
			while(d->janitor->isSignaled()) {
				d->janitor->popSignal();
			}
			this->cleanupCallbacks();
		}
	}
}

void CallbackMainDispatcher::cleanupCallbacks() {
	bool again = false;

	//Device Event
	do {
		again = false;
		MutexLocker locker(&d->mutex);
		std::list<std::tr1::shared_ptr<TDDeviceEventDispatcher> >::iterator it = d->deviceEventThreadList.begin();
		for (;it != d->deviceEventThreadList.end(); ++it) {
			if ((*it)->done()) {
				d->deviceEventThreadList.erase(it);
				again = true;
				break;
			}
		}
	} while (again);

	//Device Change Event
	do {
		again = false;
		MutexLocker locker(&d->mutex);
		std::list<std::tr1::shared_ptr<TDDeviceChangeEventDispatcher> >::iterator it = d->deviceChangeEventThreadList.begin();
		for (;it != d->deviceChangeEventThreadList.end(); ++it) {
			if ((*it)->done()) {
				d->deviceChangeEventThreadList.erase(it);
				again = true;
				break;
			}
		}
	} while (again);

	//Raw Device Event
	do {
		again = false;
		MutexLocker locker(&d->mutex);
		std::list<std::tr1::shared_ptr<TDRawDeviceEventDispatcher> >::iterator it = d->rawDeviceEventThreadList.begin();
		for (;it != d->rawDeviceEventThreadList.end(); ++it) {
			if ((*it)->done()) {
				d->rawDeviceEventThreadList.erase(it);
				again = true;
				break;
			}
		}
	} while (again);

	//Sensor Event
	do {
		again = false;
		MutexLocker locker(&d->mutex);
		std::list<std::tr1::shared_ptr<TDSensorEventDispatcher> >::iterator it = d->sensorEventThreadList.begin();
		for (;it != d->sensorEventThreadList.end(); ++it) {
			if ((*it)->done()) {
				d->sensorEventThreadList.erase(it);
				again = true;
				break;
			}
		}
	} while (again);
}