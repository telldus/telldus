__setupPackage__( __extension__ );

com.telldus.live = function() {
    var socket = null;
    var menuId = 0;
    var isRegistered = false;
	
	function init() {
		socket = new LiveSocket();
		socket.notRegistered.connect(notRegistered);
		socket.registered.connect(registered);
		socket.messageReceived.connect(messageReceived);
		socket.connectToServer();
   		com.telldus.core.deviceEvent.connect(deviceEvent);
   		com.telldus.core.deviceChange.connect(sendDevicesReport);
	}
	
	function notRegistered() {
	    isRegistered = false;
   		if (com.telldus.systray) {
   		    com.telldus.systray.addSeparator();
			menuId = com.telldus.systray.addMenuItem( "Activate Telldus Live!" );
			com.telldus.systray.menuItem(menuId).triggered.connect(socket.activate);
		    com.telldus.systray.addSeparator();
		}
	}
	
	function deviceEvent(deviceId, method, data) {
	    msg = new LiveMessage("DeviceEvent");
	    msg.append(deviceId);
	    msg.append(method);
	    msg.append(data);
	    socket.sendMessage(msg);
	}
	
	function deviceChangeEvent() {
	}
	
	function messageReceived(msg) {
		if (msg.name() == "turnon") {
			com.telldus.core.turnOn( msg.argument(0).intVal() );
		} else if (msg.name() == "turnoff") {
			com.telldus.core.turnOff( msg.argument(0).intVal() );
		}
		print("Received: " + msg.name());
	}
	
	function registered() {
	    if (menuId > 0) {
	        com.telldus.systray.removeMenuItem(menuId);
	        menuId = 0;
	    }
	    isRegistered = true;
        sendDevicesReport();
	}
	
	function sendDevicesReport() {
	    if (!isRegistered) {
	        return;
	    }
	    msg = new LiveMessage("DevicesReport");
	    var deviceList = com.telldus.core.deviceList.getList();
	    list = new LiveMessageToken();
		for( i in deviceList ) {
		    device = new LiveMessageToken();
		    device.set('id', deviceList[i].id);
		    device.set('name', deviceList[i].name);
		    list.add(device);
		}
		msg.appendToken(list);
		socket.sendMessage(msg);
	}
	
	return { //Public functions
		init:init
	}
}();

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.live.init );
}


