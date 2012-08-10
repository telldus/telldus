import telldus

telldus.tdInit()

while True:
	result = telldus.tdSensor()
	if not isinstance(result, tuple):
		break
	else:
		protocol, model, sensorId, dataTypes = result
		print "Protocol: %s,\tModel: %s,\tSensorId: %i\nDataTypes: %i" % (protocol, model, sensorId, dataTypes)

		# Retrieve the values the sensor supports
		if dataTypes & telldus.TELLSTICK_TEMPERATURE:
			result = telldus.tdSensorValue(protocol, model, sensorId, telldus.TELLSTICK_TEMPERATURE)
			if isinstance(result, tuple):
				value, timestamp = result 
				print "Temperature:\t%sC\t(%s)\n" % (value, str(timestamp))

		if dataTypes & telldus.TELLSTICK_HUMIDITY:
			result = telldus.tdSensorValue(protocol, model, sensorId, telldus.TELLSTICK_HUMIDITY)
			if isinstance(result, tuple):
				value, timestamp = result 
				print "Humidity:\t%s\t(%s)\n" % (value, str(timestamp))

		print "\n"

telldus.tdClose()
