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
			getSunRiseTime: getSunRiseTime,
			getSunSetTime: getSunSetTime,
			getSunData: getSunData,
			getTypeFromTriggerstate: getTypeFromTriggerstate
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

		//points:
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

		var now = new Date();
		var time1 = now.getHours() * 3600 + now.getMinutes() * 60 + now.getSeconds();
		print(time1); // 48880
		time2 = time1 + 30;
		time3 = time1 - 60;
		time1 = time1 + 50;

		var startdate = now; //new Date(2011,0,5).getTime();
		//var startdate2 = new Date(2011,0,5).getTime();

		//ID = ID for storage
		//Key is position in list, returned from "addJob"
		var execFunc = function(job){ print("Custom execute function running"); print("Job: " + job.v.name); return 42; };
		var job = new com.telldus.scheduler.JobRecurringWeek({id: 4, executeFunc: execFunc, name: "testnamn14", type: com.telldus.scheduler.JOBTYPE_RECURRING_WEEK, startdate: startdate, lastRun: 0, device: 1, method: 1, value: ""});
		var event = {};
		event.d = {id: 0, value: 3, fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 0, time: 300};  //(new Date().getTime())/1000 + 20
		job.addEvent(event);
		//job.addEvent(new Event({id: 0, value: "", fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 10, time: (new Date().getTime())/1000 + 20}));
		
		var jobs = new Array();
		jobs.push(job);
		
		job = new com.telldus.scheduler.JobRecurringWeek({id: 4, executeFunc: execFunc, name: "testnamn15", type: com.telldus.scheduler.JOBTYPE_RECURRING_WEEK, startdate: startdate, lastRun: 0, device: 1, method: 1, value: ""});
		var event = {};
		event.d = {id: 0, value: 3, fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 0, time: 100};  //(new Date().getTime())/1000 + 20
		job.addEvent(event);
		jobs.push(job);
		
		//hm, timerkeys... kommer ju få tillbaka alla på en ggn, eller iaf per device... (om ett addjob/device)
		//tänkte ändå alltid uppdatera alla (dvs ta bort/lägga till) för varje device, om den alls har ändrats...
		//man kunde klart koppla en key till en point, men isf måste man på ngt sätt lagra när den tas bort...
		//och jämför när man lägger till nya (iofs bara kolla om ngn key finns på pointen, annars är den ny)
		//ett jobb/punkt (med flera events, per dag)
		//lägga till alla jobb/device (för alla dagar) på en ggn (om device hasChanged), ta bort alla tidigare...
		//alla timers kommer ju att ha försvunnit vid avstängning, så det behöver man inte bry sig om...
	//	var timerkeys = com.telldus.scheduler.addJobs(jobs);
	//	for(var i=0;i<timerkeys.length;i++){
	//		print("ENA: " + timerkeys[i]);
	//	}
		//newRecurringMonthJob.save();
		/*
		var newAbsoluteJob = getJob({id: 5, name: "testnamn15", type: com.telldus.scheduler.JOBTYPE_RECURRING_MONTH, startdate: startdate2, lastRun: 0, device: 1, method: 1, value: "", pastGracePeriod: 90});
		newAbsoluteJob.addEvent(new Event({id: 1, value: "00-05", fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 0, time: time2}));
		newAbsoluteJob.addEvent(new Event({id: 2, value: "00-05", fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 0, time: time3}));
		//newAbsoluteJob.save();
		com.telldus.scheduler.addJob(newAbsoluteJob);
		*/
		//END FROM STORAGE

		view.setProperty('weekPointList', weekPointList);

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

	
	function addJobsToSchedule(points){
		//print("Uppe: " + points["TEST"]);
		var jobs = new Array();
		for(var i=0;i<points.length;i++){
			print("EN POINT " + points[i]);
			jobs.push(getJob(points[i]));
		}
		return com.telldus.scheduler.addJobs(jobs);
	}
	
	
	function getJob(pointArray){ //deviceId, pointName, startdate, lastrun, pointMethod, pointDimValue, pointTime, pointType, pointFuzzinessBefore, pointFuzzinessAfter, pointOffset, pointDays
		var execFunc = function(job){ print("Custom execute function running"); print("Job: " + job.v.name); return 42; }; //TODO default later
		//TODO dimValue ok? Or other number expected?
		var job = new com.telldus.scheduler.JobRecurringWeek({id: pointArray[0], executeFunc: execFunc, name: pointArray[1], type: com.telldus.scheduler.JOBTYPE_RECURRING_WEEK, startdate: pointArray[2], lastRun: pointArray[3], device: pointArray[0], method: pointArray[4], value: pointArray[5]});
		var event = {};
		var pointFuzzinessBefore = (pointArray[8]*60);
		var pointFuzzinessAfter = (pointArray[9]*60);
		var pointOffset = (pointArray[10]*60);
		event.d = {id: pointArray[0], value: pointArray[11][0], fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointArray[7], offset: pointOffset, time: pointArray[6]};
		job.addEvent(event);
		
		for(var i=1;i<pointArray[11].length;i++){
			event = {};
			pointDay = pointArray[11][i];
			event.d = {id: pointArray[0], value: pointDay, fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointArray[7], offset: pointOffset, time: pointArray[6]};
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
	
	/*
	function pointToJob(point){
		print("ngt");
		
		print("4: " + point.absoluteHour);
		var execFunc = function(job){ print("Custom execute function running"); print("Job: " + job.v.name); return 42; }; //TODO default later
		print("1");
		print("5: " + point);
		var deviceId = point.deviceRow.deviceId; //not really in use yet
		print("2 " + deviceId);
		var pointName = "Job_" + deviceId;
		var lastrun = 0; //TODO
		var startdate = new Date(); //startdate, not in use, always "now"
		var pointDimValue = point.dimvalue;
		var pointMethod = getMethodFromState.callWith(point.state);
		
		var job = getJob.callWith(deviceId, execFunc, pointName, startdate, lastrun, deviceId, pointMethod, pointDimValue); 
		print("Jobtest: " + job);
		
		var pointTime = point.absoluteHour * 3600 + point.absoluteMinute * 60;
		var pointType = getTypeFromTriggerstate.callWith(point.triggerstate);
		if(point.triggerstate == "sunrise"){
			var suntime = main.sunData[0].split(':');
			pointTime = suntime[0] * 3600 + suntime[1] * 60;
		}
		else if(point.triggerstate == "sunset"){
			var suntime = main.sunData[1].split(':');
			pointTime = suntime[0] * 3600 + suntime[1] * 60;
		}
		var pointFuzzinessBefore = point.fuzzyBefore;
		var pointFuzzinessAfter = point.fuzzyAfter;
		var pointOffset = point.offset;
		
		var pointDay = point.deviceRow.parent.parent.daydate.getDay(); //different per event
		var event = {};
		
		event.d = {id: deviceId, value: pointDay, fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointType, offset: pointOffset, time: pointTime};
		print("Job: " + job.v.name);
		job.addEvent(event);
		for(var childPoint in point.childPoints){
			event = {};
			pointDay = point.childPoints[childPoint].deviceRow.parent.parent.daydate.getDay(); //different per event
			event.d = {id: deviceId, value: pointDay, fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointType, offset: pointOffset, time: pointTime};
			job.addEvent(event);
		}
		
		return job;
	}
	*/

	return { //Public functions
		init:init
	}

}();
