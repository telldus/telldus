/** SchedulerGUISimple **/
__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.schedulersimplegui.init );
}

com.telldus.schedulersimplegui = function() {
	var deviceList;
	var view;

	function init() {
		view = new com.telldus.qml.view({
			addDevice: addDevice
		});
		deviceList = new com.telldus.qml.array();
		var list = com.telldus.core.deviceList.getList();
		for(var i=0; i < list.length; ++i) {
			deviceList.push(list[i]);
		}
		view.setProperty('deviceModel', deviceList);
		view.load("main.qml");
		application.addWidget("scheduler.simple", "icon.png", view);

	}

	function addDevice() {
		deviceList.push({name:'Stallet'});
	}

	return { //Public functions
		init:init
	}

}();
