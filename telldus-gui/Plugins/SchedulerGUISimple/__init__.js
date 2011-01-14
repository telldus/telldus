/** SchedulerGUISimple **/
__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.schedulersimplegui.init );
}

com.telldus.schedulersimplegui = function() {
	
	function init() {
		print("START");
		var v = new com.telldus.qml.view({});
		v.load("main.qml");
		//var v = new com.telldus.qml.view("");
		application.addWidget("scheduler.simple", ":/images/devices.png", v);

	}
	
	return { //Public functions
		init:init
	}
	
}();
