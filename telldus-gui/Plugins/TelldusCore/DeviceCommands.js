/** Base **/
com.telldus.core.DeviceCommand = function() {
}

com.telldus.core.DeviceCommand.prototype.execute = function() {
	print("DeviceCommand::execute is not implemented for this type!");
}

/** ON **/
com.telldus.core.DeviceOnCommand = function() {}
com.telldus.core.DeviceOnCommand.prototype = new com.telldus.core.DeviceCommand();

com.telldus.core.DeviceOnCommand.prototype.execute = function(deviceId) {
	return com.telldus.core.turnOn(deviceId);
}

/** OFF **/
com.telldus.core.DeviceOffCommand = function() {}
com.telldus.core.DeviceOffCommand.prototype = new com.telldus.core.DeviceCommand();

com.telldus.core.DeviceOffCommand.prototype.execute = function(deviceId) {
	return com.telldus.core.turnOff(deviceId);
}

/** DIM **/
com.telldus.core.DeviceDimCommand = function(value) {
	this.value = value;
}
com.telldus.core.DeviceDimCommand.prototype = new com.telldus.core.DeviceCommand();

com.telldus.core.DeviceDimCommand.prototype.execute = function(deviceId) {
	return com.telldus.core.dim(deviceId, this.value);
}


