import telldus
import time
from threading import Timer

telldus.tdInit()

timers = {} #timerlist

def turnOn():
	print "turning on"
	telldus.tdTurnOn(1)

def turnOff():
	print "turning off"
	telldus.tdTurnOff(1)
	
def callback(deviceId, method, value, callbackId):
	global timers
	
	print "callback!"

	if (deviceId == 1):
		# is turning on deviceId 1 here, so just return if events for that device are picked up
		return
		
	t = 0
	print "Received event for device %d" % (deviceId,)
	if (deviceId in timers):
		# a timer already exists for this device, it might be running so interrupt it
		# Many devices (for example motion detectors) resends their messages many times to ensure that they
		# are received correctly. In this example, we don't want to run the turnOn/turnOff methods every time, instead we
		# start a timer, and run the method when the timer is finished. For every incoming event on this device, the timer
		# is restarted.
		t = timers[deviceId] 
		t.cancel()
	if (method == 1):
		#on
		t = Timer(0.5, turnOn) #start timer with 0.5 second delay (adjust the delay to suit your needs), then turn on
	else:
		#off
		t = Timer(0.5, turnOff) #start timer with 0.5 second delay (adjust the delay to suit your needs), then turn off

	t.start()
	timers[deviceId] = t #put timer in list, to allow later cancellation

#function to be called when device event occurs, even for unregistered devices
def rawcallback(data, controllerId, callbackId):
	print str(data)

callbackid = telldus.tdRegisterDeviceEvent(callback)
rawcallbackid = telldus.tdRegisterRawDeviceEvent(rawcallback)

try:
	while(1):
		time.sleep(0.5) #don't exit
except KeyboardInterrupt:
	print "Exiting"
	telldus.tdUnregisterCallback(callbackid)
	telldus.tdUnregisterCallback(rawcallbackid)
	telldus.tdClose()
