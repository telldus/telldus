/** SchedulerGUISimple **/
__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.schedulersimplegui.init );
}

com.telldus.schedulersimplegui = function() {
	
	function init() {
		var v = new com.telldus.qml.view({});
		v.setProperty('deviceModel', com.telldus.core.deviceList.getList());
		v.load("main.qml");
		application.addWidget("scheduler.simple", "icon.png", v);

	}
	
	return { //Public functions
		init:init
	}
	
}();
