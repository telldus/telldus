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
			getTriggerstateFromType: getTriggerstateFromType,
			restoreJobs: restoreJobs
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

		//var storedPoints = loadJobs();
		//view.setProperty('storedPoints', storedPoints);

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
	
	function addJobsToSchedule(deviceId, points, deviceTimerKeys, callbackFunc){
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
			var jobtemp = getJob(points[i], callbackFunc);
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
	
	function loadJobs(updateLastRun){
		var settings = new com.telldus.settings();
		var storedJobs = settings.value("jobs", "");
		restoreJobsToSchedule(storedJobs, updateLastRun); //Start timers here
		return storedJobs;
	}
	
	function restoreJobsToSchedule(storedJobs, updateLastRun){
		var jobs = new Array();
		for(var devicekey in storedJobs){
			for(var i=0;i<storedJobs[devicekey].length;i++){
				if(storedJobs[devicekey][i] == undefined){
					continue;
				}
				var jobtemp = restoreJob(storedJobs[devicekey][i], updateLastRun);
				jobs.push(jobtemp);
			}
		}
		return com.telldus.scheduler.addJobs(jobs)
	}
	
	function restoreJob(storedJob, updateLastRunInGUI){
		var execFunc = function(job){ print("Custom execute function running"); print("Job: " + job.v.name); var lastRun = new Date().getTime(); updateLastRun(job, lastRun); updateLastRunInGUI(job.v.device, job.v.events[job.v.id + "_0"].d.value, job.v.id, lastRun); return job.executeDefault();};
		var job = new com.telldus.scheduler.JobRecurringWeek(storedJob.v);
		job.v.executeFunc = execFunc;
		return job;
	}
	
	function restoreJobs(updateLastRun){
		var storedPoints = loadJobs(updateLastRun);
		view.setProperty('storedPoints', storedPoints);
	}
	
	function getJob(pointArray, updateLastRunInGUI){ //deviceId, pointName, startdate, lastrun, pointMethod, pointDimValue, pointTime, pointType, pointFuzzinessBefore, pointFuzzinessAfter, pointOffset, pointDays
		//updateLastRunInGUI: deviceId, day of week, id
		var execFunc = function(job){ print("Custom execute function running"); print("Job: " + job.v.name); var lastRun = new Date().getTime(); updateLastRun(job, lastRun); updateLastRunInGUI(job.v.device, job.v.events[job.v.id + "_0"].d.value, job.v.id, lastRun); return job.executeDefault();};
		print("POINTARRAY3: " + pointArray[3]);
		var job = new com.telldus.scheduler.JobRecurringWeek({id: pointArray[13], executeFunc: execFunc, name: pointArray[1], type: com.telldus.scheduler.JOBTYPE_RECURRING_WEEK, startdate: pointArray[2], lastRun: pointArray[3], device: pointArray[0], method: pointArray[4], value: pointArray[5], absoluteTime: pointArray[12]});
		var event = {};
		var pointFuzzinessBefore = (pointArray[8]*60);
		var pointFuzzinessAfter = (pointArray[9]*60);
		var pointOffset = (pointArray[10]*60);
		event.d = {id: (pointArray[13] + "_0"), value: pointArray[11][0], fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointArray[7], offset: pointOffset, time: pointArray[6]};
		job.addEvent(event);
		
		for(var i=1;i<pointArray[11].length;i++){
			event = {};
			pointDay = pointArray[11][i];
			event.d = {id: (pointArray[13] + "_" + i), value: pointDay, fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointArray[7], offset: pointOffset, time: pointArray[6]};
			job.addEvent(event);
		}
		
		return job;
	}
	
	function updateLastRun(job, lastRun){
		//TODO uppdatera både storage och värdet i pointen...
		var settings = new com.telldus.settings();
		var storedjobs = settings.value("jobs", "");
		//settings.setValue("test", "TESTAR");
		print("UPDATING LAST RUN 3 " + storedjobs);
		for(var devicekey in storedjobs){
			print("Key: " + devicekey);
			if(devicekey == job.v.device){
				print("En bra bit");
				
				for(var i=0;i<storedjobs[devicekey].length;i++){
					print("GREJ 2: " + storedjobs[devicekey][i].v.id);
					print("JMF: " + job.v.id);
					if(storedjobs[devicekey][i].v.id == job.v.id){
						storedjobs[devicekey][i].v.lastRun = lastRun;
						break;
					}
				}
				break;
			}
		}
		
		settings.setValue("jobs", storedjobs);
		print("saveD");
		
		//för alla points denna... device och dag (parentpointens dag, dvs det som är lagrat i job)
		//gå igenom punkterna, hitta punkt med korrekt id, uppdatera last run för den...
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
