function Device(id) {
	this.id = id;
	this.name = com.telldus.core.getName(id);
	this.commandStack = new Array();
}

Device.prototype.pushCommand = function(command) {
	this.commandStack.push(command);
	command.execute(this.id);
}

Device.prototype.popCommand = function(command) {

	this.commandStack.pop();
	if (this.commandStack.length) {
		var command = this.commandStack[this.commandStack.length-1].execute(this.id);
	} else {
		com.telldus.core.turnOff(this.id);
	}
}
