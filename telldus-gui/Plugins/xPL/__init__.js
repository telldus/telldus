__setupPackage__("com.telldus.xpl");

com.telldus.xpl.lighting = function() {
	var instance = null;
	
	function init() {
		instance = new xPLInstance("telldus", "center");
		instance.attachedToNetwork.connect(attachedToNetwork);
		instance.messageReceived.connect(messageReceived);
	}

	function attachedToNetwork() {
		var msg = new xPLMessage(xPLMessage.xpltrig);
		msg.setMessageSchemeIdentifier("lighting.gateway");
		msg.addBodyItem("report", "gateway-ready");
		instance.sendMessage(msg);
		com.telldus.core.deviceChange.connect(deviceChangeEvent);
		com.telldus.core.deviceEvent.connect(deviceEvent);
	}

	function deviceChangeEvent( deviceId, eventType ) {
		var msg = new xPLMessage(xPLMessage.xpltrig);
		msg.setMessageSchemeIdentifier("lighting.gateway");
		msg.addBodyItem("report", "gateway-changed");
		instance.sendMessage(msg);
	}

	function deviceEvent(deviceId, method, data) {
		var msg = new xPLMessage(xPLMessage.xpltrig);
		msg.setMessageSchemeIdentifier("lighting.device");
		msg.addBodyItem("network", "1");
		msg.addBodyItem("device", deviceId);
		msg.addBodyItem("channel", "1");
		var level = 0;
		if (method == com.telldus.core.TELLSTICK_TURNOFF) {
			msg.addBodyItem("state", "off");
		} else {
			//Even a dimmed device is considered on
			msg.addBodyItem("state", "on");
			level = 100;
			if (method == com.telldus.core.TELLSTICK_DIM) {
				level = parseFloat(data);
				level = Math.round(level / 255.0 * 100.0);
				if (level > 100) {
					level = 100;
				} else if (level < 0) {
					level = 0;
				}
			}
		}
		msg.addBodyItem("level", level);
		instance.sendMessage(msg);
	}

	function handleLightingRequest(msg) {
		switch(msg.bodyItem("request")) {
			case "devinfo":
				sendDevInfo(msg);
				break;
			case "devlist":
				sendDevList(msg);
				break;
			case "gateinfo":
				sendGatewayInfo(msg);
				break;
			case "netinfo":
				sendNetInfo(msg);
				break;
			case "netlist":
				sendNetList(msg);
				break;
			default:
				print(msg);
		}
	}

	function handleLightingCommand(msg) {
		if (msg.bodyItem("command") != "goto") {
			return;
		}

		if (msg.bodyItem("network") != "1") {
			return;
		}

		var deviceId = msg.bodyItem("device")
		var device = com.telldus.core.deviceList.getDevice(deviceId);
		if (device == false) {
			return;
		}
		
		var level = msg.bodyItem("level");
		if (level == "") {
			return;
		}

		level = parseInt(level);
		if (level < 0 || level > 100) {
			return;
		}
		level = Math.round(parseFloat(level) * 255.0 / 100.0);

		if (level > 0 && level < 255) {
			/* See if the device supports dim */
			if (!(com.telldus.core.methods(deviceId, com.telldus.core.TELLSTICK_DIM) & com.telldus.core.TELLSTICK_DIM)) {
				/* Non dimmable device was dimmed */
				return;
			}
			com.telldus.core.dim(deviceId, level);
		} else if (level == 255) {
			com.telldus.core.turnOn(deviceId);
		} else if (level == 0) {
			com.telldus.core.turnOff(deviceId);
		}
	}

	function messageReceived(msg) {
		if(msg.messageSchemeIdentifier() == "lighting.request") {
			handleLightingRequest(msg);
		} else if (msg.messageSchemeIdentifier() == "lighting.basic") {
			handleLightingCommand(msg);
		}
	}

	function sendDevInfo(msg) {
		var deviceId = msg.bodyItem("device")
		var device = com.telldus.core.deviceList.getDevice(deviceId);

		var message = new xPLMessage(xPLMessage.xplstat);
		message.setMessageSchemeIdentifier("lighting.devinfo");

		var network = msg.bodyItem("network");
		message.addBodyItem("network", network);
		message.addBodyItem("device", deviceId);

		if (network == "1" && device != false) {
			var lastSentCommand = com.telldus.core.lastSentCommand(deviceId, com.telldus.core.TELLSTICK_TURNON | com.telldus.core.TELLSTICK_TURNOFF | com.telldus.core.TELLSTICK_DIM);
			var level = 0;
			if (lastSentCommand == com.telldus.core.TELLSTICK_TURNON) {
				level = 100;
			} else if (lastSentCommand == com.telldus.core.TELLSTICK_DIM) {
				var level = parseFloat(com.telldus.core.lastSentValue(deviceId));
				level = Math.round(level / 255.0 * 100.0);
				if (level > 100) {
					level = 100;
				} else if (level < 0) {
					level = 0;
				}
			} else {
				level = 0;
			}
			var methods = com.telldus.core.methods(deviceId, com.telldus.core.TELLSTICK_TURNON | com.telldus.core.TELLSTICK_TURNOFF | com.telldus.core.TELLSTICK_DIM);
			var channel = "1," + (methods & com.telldus.core.TELLSTICK_DIM ? "true" : "false") + ",0," + level;
			message.addBodyItem("status", "ok");
			message.addBodyItem("name", device.name );
			message.addBodyItem("report-on-manual", "false" );
			message.addBodyItem("channel-count", "1" );
			message.addBodyItem("primary-channel", "1" );
			message.addBodyItem("channel", channel );
			message.addBodyItem("scene-count", "0" );

		} else {
			message.addBodyItem("status", "not-found");
		}

		instance.sendMessage(message);
	}

	function sendDevList(msg) {
		var message = new xPLMessage(xPLMessage.xplstat);
		message.setMessageSchemeIdentifier("lighting.devlist");

		var network = msg.bodyItem("network");
		message.addBodyItem("network", network);

		if (network == "1") {
			message.addBodyItem("status", "ok");
			var strDevices = "";
			var deviceList = com.telldus.core.deviceList.getList();
			for( i in deviceList ) {
				if (i > 0) {
					strDevices += ",";
				}
				strDevices += deviceList[i].id;
			}
			message.addBodyItem("device-count", deviceList.length);
			message.addBodyItem("device", strDevices );
		} else {
			message.addBodyItem("status", "not-found");
		}

		instance.sendMessage(message);
	}

	function sendGatewayInfo(msg) {
		var msg = new xPLMessage(xPLMessage.xplstat);
		msg.setMessageSchemeIdentifier("lighting.gateinfo");

		msg.addBodyItem("status", "ok");
		msg.addBodyItem("protocol", "TELLDUS");
		msg.addBodyItem("description", "Telldus Center xPL-interface");
		msg.addBodyItem("version", "1");
		msg.addBodyItem("author", "Telldus Technologies AB");
		msg.addBodyItem("info-url", "http://www.telldus.se");
		msg.addBodyItem("net-count", "1");
		msg.addBodyItem("preferred-net", "1");
		msg.addBodyItem("scenes-ok", "false");
		msg.addBodyItem("channels-ok", "false");
		msg.addBodyItem("fade-rate-ok", "false");

		instance.sendMessage(msg);
	}

	function sendNetInfo(msg) {
		var message = new xPLMessage(xPLMessage.xplstat);
		message.setMessageSchemeIdentifier("lighting.netinfo");

		var network = msg.bodyItem("network");
		message.addBodyItem("network", network);

		if (network == "1") {
			message.addBodyItem("status", "ok");
			message.addBodyItem("name", "TelldusCenter" ); //TODO: Find a nice name
			message.addBodyItem("device-count", com.telldus.core.deviceList.count() );
			message.addBodyItem("scene-count", "0" );
		} else {
			message.addBodyItem("status", "not-found");
		}

		instance.sendMessage(message);
	}

	function sendNetList(msg) {
		var msg = new xPLMessage(xPLMessage.xplstat);
		msg.setMessageSchemeIdentifier("lighting.netlist");

		msg.addBodyItem("status", "ok");
		msg.addBodyItem("network", "1");

		instance.sendMessage(msg);
	}

	return { //Public functions
		init:init
	}
}();


__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.xpl.lighting.init );
}



