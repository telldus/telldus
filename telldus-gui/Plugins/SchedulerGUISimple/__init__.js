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
			addDevice: addDevice,
			getSunRiseTime: getSunRiseTime,
			getSunSetTime: getSunSetTime
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
	
	function getSun(riseset, rowWidth, pointWidth){
		var date = new Date();
		var timevalues = com.telldus.suncalculator.riseset(date);
		var hourminute;
		if(riseset == "rise"){
			hourminute = timevalues[0].split(':');
		}
		else{
			hourminute = timevalues[1].split(':');
		}
		print("Hourminute: " + hourminute[0]);
		var hourSize = rowWidth/24;
		print("Size: " + hourSize);
		return hourSize*hourminute[0];  //TODO Only hour for now...
	}
	
	function getSunRiseTime(rowWidth, pointWidth){
		//TODO turn into one method
		rowWidth = 700 //TODO
		pointWidth = 30 //TODO
		return getSun("rise", rowWidth, pointWidth);
	}
	
	function getSunSetTime(rowWidth, pointWidth){
		rowWidth = 700 //TODO
		pointWidth = 30 //TODO
		return getSun("set", rowWidth, pointWidth);
	}

	return { //Public functions
		init:init
	}

}();
