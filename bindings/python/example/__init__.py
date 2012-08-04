
import telldus
import time

telldus.tdInit()
devices = telldus.tdGetNumberOfDevices();
print "Devices: %d\n" % devices

allMethods = telldus.TELLDUS_TURNON | telldus.TELLDUS_TURNOFF | telldus.TELLDUS_BELL | telldus.TELLDUS_DIM

for i in xrange(devices):
	deviceid = telldus.tdGetDeviceId(i)
	name = tdGetName(deviceid)
	
	print "%s - %s\n" % (deviceid, name)
	
	methods = telldus.tdMethods(deviceid, allMethods)
	
	if methods & telldus.TELLDUS_TURNON:
		print " * TurnOn\n"
		telldus.tdTurnOn(deviceid)
		time.sleep(1)
		
	if methods & telldus.TELLDUS_TURNOFF:
		print " * TurnOff\n"
		telldus.tdTurnOff(deviceid)
		time.sleep(1)
		
	if methods & telldus.TELLDUS_BELL:
		echo " * Bell\n"
		telldus.tdBell(deviceid)
		time.sleep(1)
		
	if methods & telldus.TELLDUS_TOGGLE:
		print " * Toggle\n"
		
	if methods & telldus.TELLDUS_DIM:
		print " * Dim\n"
		telldus.tdDim(deviceid, 128)
		time.sleep(1)

telldus.tdClose()