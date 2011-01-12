/** TELLDUS CORE **/
__setupPackage__( __extension__ );

include("Device.js");

__postInit__ = function() {
	com.telldus.core.errorOccurred.connect( errorOccurred );

	com.telldus.core.deviceList = function() {
		var devices = new Array();
		//Load all the devices
		var deviceCount = com.telldus.core.getNumberOfDevices();
		for (var i = 0; i < deviceCount; ++i) {
			deviceChangeEvent(com.telldus.core.getDeviceId(i), com.telldus.core.TELLSTICK_DEVICE_ADDED);
		}
// 		com.telldus.core.deviceEvent.connect(deviceEvent);
		com.telldus.core.deviceChange.connect(deviceChangeEvent);

		function count() {
			return devices.length;
		}

		function deviceChangeEvent( deviceId, eventType ) {
			print("DeviceChangeEvent", deviceId, eventType);
			if (eventType == com.telldus.core.TELLSTICK_DEVICE_ADDED) {
				devices.push(new Device(com.telldus.core.getDeviceId(i)));
				return;
			}

			if (eventType == com.telldus.core.TELLSTICK_DEVICE_CHANGED) {
				var device = getDevice(deviceId);
				if (device == false) {
					return;
				}
				device.name = com.telldus.core.getName(deviceId);
				return;
			}

			if (eventType == com.telldus.core.TELLSTICK_DEVICE_REMOVED) {
				for(i in devices) {
					if (devices[i].id == deviceId) {
						devices.splice(i, 1);
						break;
					}
				}
				return;
			}
		}

// 		function deviceEvent(deviceId, method, data) {
// 		}

		function deviceExists(id) {
			for(i in devices) {
				if (devices[i].id == id) {
					return true;
				}
			}
			return false;
		}

		function getDevice(id) {
			for(i in devices) {
				if (devices[i].id == id) {
					return devices[i];
				}
			}
			return false;
		}

		function getList() {
			return devices;
		}

		return { //Public functions
			count: count,
			deviceExists: deviceExists,
			getDevice: getDevice,
			getList: getList
		};
	}();
}

function errorOccurred( deviceId, errorId, errorString ) {
	application.showMessage("", errorString, "");
}
