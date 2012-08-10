
import telldus
import time

telldus.tdInit()
devices = telldus.tdGetNumberOfDevices()
print "Devices: %d\n" % devices

allMethods = telldus.TELLSTICK_TURNON | telldus.TELLSTICK_TURNOFF | telldus.TELLSTICK_BELL | telldus.TELLSTICK_DIM

for i in xrange(devices):
	deviceid = telldus.tdGetDeviceId(i)
	name = telldus.tdGetName(deviceid)
	
	print "%s - %s\n" % (deviceid, name)
	
	methods = telldus.tdMethods(deviceid, allMethods)
	
	if methods & telldus.TELLSTICK_TURNON:
		print " * TurnOn\n"
		telldus.tdTurnOn(deviceid)
		time.sleep(1)
		
	if methods & telldus.TELLSTICK_TURNOFF:
		print " * TurnOff\n"
		telldus.tdTurnOff(deviceid)
		time.sleep(1)
		
	if methods & telldus.TELLSTICK_BELL:
		echo " * Bell\n"
		telldus.tdBell(deviceid)
		time.sleep(1)
		
	if methods & telldus.TELLSTICK_TOGGLE:
		print " * Toggle\n"
		
	if methods & telldus.TELLSTICK_DIM:
		print " * Dim\n"
		telldus.tdDim(deviceid, 128)
		time.sleep(1)

telldus.tdClose()

import time
time.sleep(30)
for i in xrange(20):
	telldus.tdTurnOn(3)
	telldus.tdTurnOff(3)


