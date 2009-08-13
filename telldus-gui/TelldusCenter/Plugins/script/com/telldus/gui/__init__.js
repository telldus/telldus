/** TELLDUS GUI **/

var devices = new Array();

__postInit__ = function() {
	com.telldus.gui.deviceChange.connect(com.telldus.core.deviceChange);
	com.telldus.core.deviceChange.connect(deviceChange);
	application.allDoneLoading.connect( allDone );
	application.addWidget("devices.default", ":/images/devices.png", com.telldus.gui);
}

function allDone() {
	if (com.telldus.systray) {
		var count = com.telldus.core.getNumberOfDevices();
		for (var i = 0; i < count; ++i) {
			var id = com.telldus.core.getDeviceId(i);
			var name = com.telldus.core.getName(id);
			var menuId = com.telldus.systray.addMenuItem( name );

			devices.push({
				'id': id,
				'menuId': menuId
			});
		}

		com.telldus.systray.addSeparator();
	}
}

function deviceChange( deviceId, eventType ) {
	if (eventType != 2) {
		return;
	}
	for (var i=0; i < devices.length; ++i) {
		if (devices[i].id == deviceId) {
			var name = com.telldus.core.getName(deviceId);
			var menuItem = com.telldus.systray.menuItem( devices[i].menuId );
			if (menuItem) {
				menuItem.text = name;
			}
			break;
		}
	}
}

