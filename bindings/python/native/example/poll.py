import telldus

telldus.tdInit()

	while(1):
		result = telldus.tdSensor()
		if not result:
			break
		else:
			protocol, model, sensorId = result
			print "%s,\t%s,\t%i\n" % (protocol, model, sensorId)
	
			# Retrieve the values the sensor supports
			if (dataTypes & telldus.TELLSTICK_TEMPERATURE):
				result = telldus.tdSensorValue(protocol, model, sensorId, telldus.TELLSTICK_TEMPERATURE)
				if result:
					value, timestamp = result 
				   print "Temperature:\t%sº\t(%s)\n" % (value, str(timestamp))
				   
			if (dataTypes & telldus.TELLSTICK_HUMIDITY):
				result = telldus.tdSensorValue(protocol, model, sensorId, telldus.TELLSTICK_HUMIDITY)
				if result:
					value, timestamp = result 
					print "Humidity:\t%s\t(%s)\n" % (value, str(timestamp))
	
			print "\n"

tdClose()
