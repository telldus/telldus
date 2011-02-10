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
			addJobsToSchedule: addJobsToSchedule,
			getJob: getJob,
			getMethodFromState: getMethodFromState,
			getStateFromMethod: getStateFromMethod, 
			getSunRiseTime: getSunRiseTime,
			getSunSetTime: getSunSetTime,
			getSunData: getSunData,
			getTypeFromTriggerstate: getTypeFromTriggerstate,
			getTriggerstateFromType: getTriggerstateFromType
		});

		//devices:
		deviceList = new com.telldus.qml.array();
		var list = com.telldus.core.deviceList.getList();
		for(var i=0; i < list.length; ++i) {
			var item = list[i];
			item.isEnabled = "enabled";
			item.hasChanged = "false";
			deviceList.push(item);
		}
		view.setProperty('deviceModel', deviceList);
		//Listen for device-change
		com.telldus.core.deviceChange.connect(deviceChange);

		var storedPoints = loadJobs();
		view.setProperty('storedPoints', storedPoints);

		//set images:
		view.setProperty("imageTriggerSunrise", "sunrise.png");
		view.setProperty("imageTriggerSunset", "sunset.png");
		view.setProperty("imageTriggerAbsolute", "absolute.png");
		view.setProperty("imageActionOn", "on.png");
		view.setProperty("imageActionOff", "off.png");
		view.setProperty("imageActionDim", "dim.png");
		view.setProperty("imageActionBell", "bell.png");
		view.setProperty("imageInfo", "info.png");

		//height/width constants
		view.setProperty("constBarHeight", 10);
		view.setProperty("constDeviceRowHeight", 50);
		view.setProperty("constDeviceRowWidth", 600);
		view.setProperty("constPointWidth", 30);

		view.load("main.qml");
		application.addWidget("scheduler.simple", "icon.png", view);

	}

	function addDevice() {
		deviceList.push({name:'Stallet istallet'});
	}

	function deviceChange( deviceId, eventType ) {
		if (eventType == com.telldus.core.TELLSTICK_DEVICE_ADDED) {
			var item = com.telldus.core.deviceList.getDevice(deviceId);
			item.isEnabled = "enabled";
			deviceList.push(item);
		}
	}
	
	function addJobsToSchedule(deviceId, points, deviceTimerKeys){
		//delete all current schedules for this device:
		if(deviceTimerKeys != undefined){
			for(var i=0;i<deviceTimerKeys.length;i++){
				var waitForMore = (i == deviceTimerKeys.length-1) ? "true" : undefined;
				com.telldus.scheduler.removeJob(deviceTimerKeys[i]);
			}
		}
		
		//add new schedules:
		var jobs = new Array();
		for(var i=0;i<points.length;i++){
			var jobtemp = getJob(points[i]);
			jobs.push(jobtemp);
		}
		print("Adding some jobs " + jobs.length);
		saveJobs(deviceId, jobs);
		//return [];
		return com.telldus.scheduler.addJobs(jobs);
	}
	
	function saveJobs(deviceId, devicejobs){
		var settings = new com.telldus.settings();
		var jobs = settings.value("jobs", "");
			
		if(!jobs || jobs == undefined){
			jobs = {}; //initialize new setting
		}
		jobs[deviceId] = devicejobs;
		settings.setValue("jobs", jobs);
	}
	
	function loadJobs(){
		var settings = new com.telldus.settings();
		var temp = settings.value("jobs", "");
		return settings.value("jobs", "");
		
		/*
		//from storage...
		var weekPointList = new com.telldus.qml.array();
		var dummypoint = {};
		dummypoint["day"] = 0;
		dummypoint["deviceId"] = 1;
		weekPointList.push(dummypoint);
		dummypoint = {};
		dummypoint["day"] = 1;
		dummypoint["deviceId"] = 2;
		weekPointList.push(dummypoint);
		*/
	}
	
	function getJob(pointArray){ //deviceId, pointName, startdate, lastrun, pointMethod, pointDimValue, pointTime, pointType, pointFuzzinessBefore, pointFuzzinessAfter, pointOffset, pointDays
		//var execFunc = function(job){ print("Custom execute function running"); print("Job: " + job.v.name); return 42; }; //TODO default later
		//TODO dimValue 0-100 ok? Or other number expected?
		var job = new com.telldus.scheduler.JobRecurringWeek({id: pointArray[0], executeFunc: null, name: pointArray[1], type: com.telldus.scheduler.JOBTYPE_RECURRING_WEEK, startdate: pointArray[2], lastRun: pointArray[3], device: pointArray[0], method: pointArray[4], value: pointArray[5], absoluteTime: pointArray[12]});
		var event = {};
		var pointFuzzinessBefore = (pointArray[8]*60);
		var pointFuzzinessAfter = (pointArray[9]*60);
		var pointOffset = (pointArray[10]*60);
		event.d = {id: (pointArray[0] + "_0"), value: pointArray[11][0], fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointArray[7], offset: pointOffset, time: pointArray[6]};
		job.addEvent(event);
		
		for(var i=1;i<pointArray[11].length;i++){
			event = {};
			pointDay = pointArray[11][i];
			event.d = {id: (pointArray[0] + "_" + i), value: pointDay, fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointArray[7], offset: pointOffset, time: pointArray[6]};
			job.addEvent(event);
		}
		
		return job;
	}
	
	function getMethodFromState(state){
		var pointMethod = "";
		if(state == "on"){
			pointMethod = com.telldus.core.TELLSTICK_TURNON;
		}
		else if(state == "off"){
			pointMethod = com.telldus.core.TELLSTICK_TURNOFF;
		}
		else if(state == "dim"){
			pointMethod = com.telldus.core.TELLSTICK_DIM;
		}
		else if(state == "bell"){
			pointMethod = com.telldus.core.TELLSTICK_BELL;
		}
		return pointMethod;
	}
	
	function getStateFromMethod(pointMethod){
		var state = "";
		if(pointMethod == com.telldus.core.TELLSTICK_TURNON){
			state = "on";
		}
		else if(pointMethod == com.telldus.core.TELLSTICK_TURNOFF){
			state = "off";
		}
		else if(pointMethod == com.telldus.core.TELLSTICK_DIM){
			state = "dim";
		}
		else if(pointMethod == com.telldus.core.TELLSTICK_BELL){
			state = "bell";
		}
		return state;
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

		return getSun("rise", rowWidth, pointWidth);
	}

	function getSunSetTime(rowWidth, pointWidth){
		return getSun("set", rowWidth, pointWidth);
	}
	
	function getTypeFromTriggerstate(triggerstate){
		var type = "";
		if(triggerstate == "sunrise"){
			type = com.telldus.scheduler.EVENTTYPE_SUNRISE;
		}
		else if(triggerstate == "sunset"){
			type = com.telldus.scheduler.EVENTTYPE_SUNSET;
		}
		else{
			type = com.telldus.scheduler.EVENTTYPE_ABSOLUTE;
		}
		return type;
	}
	
	function getTriggerstateFromType(type){
		var triggerstate = "";
		if(type == com.telldus.scheduler.EVENTTYPE_SUNRISE){
			triggerstate = "sunrise";
		}
		else if(type == com.telldus.scheduler.EVENTTYPE_SUNSET){
			triggerstate = "sunset";
		}
		else{
			triggerstate = "absolute";
		}
		return triggerstate;
	}

	return { //Public functions
		init:init
	}

}();
