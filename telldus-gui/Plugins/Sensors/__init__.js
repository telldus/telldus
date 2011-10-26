/** Sensors **/
__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.sensors.init );
}

com.telldus.sensors = function() {
	var sensorList = new com.telldus.qml.array();

	function init() {
		var sensorData = 0;
		while(sensorData = com.telldus.core.sensor()) {
			var p = sensorData["protocol"];
			var m = sensorData["model"];
			var id = sensorData["sensorId"];
			var types = sensorData["dataTypes"];

			var tryFetchValue = function(p, m, id, types, type) {
				if (types & type) {
					sensorValue = com.telldus.core.sensorValue(p, m, id, type);
					sensorEvent(p, m, id, type, sensorValue["value"], sensorValue["timestamp"]);
				}
			}
			tryFetchValue(p, m, id, types, com.telldus.core.TELLSTICK_TEMPERATURE);
			tryFetchValue(p, m, id, types, com.telldus.core.TELLSTICK_HUMIDITY);

		}

		//com.telldus.core.sensorEvent.connect(sensorEvent);
		view = new com.telldus.qml.view({
		});

		view.setProperty('sensorModel', sensorList);
		view.load("main.qml");
		application.addWidget("sensors.gui", "icon.png", view);
	}

	function sensorEvent(protocol, model, id, dataType, value, timestamp) {
		var sensor = 0;
		for (var i = 0; i < sensorList.length; ++i) {
			if (sensorList.get(i).protocol != protocol) {
				continue;
			}
			if (sensorList.get(i).model != model) {
				continue;
			}
			if (sensorList.get(i).id != id) {
				continue;
			}
			sensor = sensorList.get(i);
			break;
		}

		if (!sensor) {
			sensor = new com.telldus.sensors.sensor();
			sensor.protocol = protocol;
			sensor.model = model;
			sensor.id = id;
			sensorList.push(sensor);
			print("Create new");
		} else {
			print("Update");
		}

		if (dataType == com.telldus.core.TELLSTICK_TEMPERATURE) {
			sensor.temperature = value;
		} else if (dataType == com.telldus.core.TELLSTICK_HUMIDITY) {
			sensor.humidity = value;
		}

		print("Sensor event", protocol, model, id, dataType, value, timestamp);
	}

	return { //Public functions
		init:init
	}

}();
