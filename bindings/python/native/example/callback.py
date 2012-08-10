import threading
import telldus
import time


telldus.tdInit()


def turnOn():
	print "turning on"
	telldus.tdTurnOn(1)

def turnOff():
	print "turning off"
	telldus.tdTurnOff(1)
	
def callback(deviceId, method, value, callbackId):
	print "callback"	
	return True

#function to be called when device event occurs, even for unregistered devices
def rawcallback(data, controllerId, callbackId):
	print "raw callback"	
	return True

#callbackid = telldus.tdRegisterDeviceEvent(callback)
rawcallbackid = telldus.tdRegisterRawDeviceEvent(rawcallback)

try:
	while(1):
		time.sleep(0.5) #don't exit
except KeyboardInterrupt:
	print "Exiting"
	#telldus.tdUnregisterCallback(callbackid)
	telldus.tdUnregisterCallback(rawcallbackid)
	telldus.tdClose()
