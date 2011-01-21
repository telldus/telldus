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
			getSunSetTime: getSunSetTime,
			getSunData: getSunData
		});
		deviceList = new com.telldus.qml.array();
		var list = com.telldus.core.deviceList.getList();
		for(var i=0; i < list.length; ++i) {
			deviceList.push(list[i]);
		}
		view.setProperty('deviceModel', deviceList);
		//set images:
		view.setProperty("imageTriggerSunrise", "sunrise.png");
		view.setProperty("imageTriggerSunset", "sunset.png");
		view.setProperty("imageTriggerAbsolute", "absolute.png");
		view.setProperty("imageActionOn", "on.png");
		view.setProperty("imageActionOff", "off.png");
		view.setProperty("imageActionDim", "dim.png");
		view.setProperty("imageActionBell", "bell.png");
		view.setProperty("imageInfo", "info.png");
		
		view.setProperty("constBarHeight", 10);
		
		view.load("main.qml");
		application.addWidget("scheduler.simple", "icon.png", view);

	}

	function addDevice() {
		deviceList.push({name:'Stallet istallet'});
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
		var hourSize = rowWidth/24;
		return hourSize*hourminute[0] + hourSize * (hourminute[1]/60) - pointWidth/2;
	}
	
	//Raw sun data
	function getSunData(){
		var date = new Date;
		return com.telldus.suncalculator.riseset(date);
	}
	
	function getSunRiseTime(rowWidth, pointWidth){
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
