/** SchedulerGUISimple **/
__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.schedulersimplegui.init );
}

com.telldus.schedulersimplegui = function() {
	var deviceList;
	var view;
	var restoredJobsTimerKeys;
	var sunData;

	function init() {
		view = new com.telldus.qml.view({
			addJobsToSchedule: addJobsToSchedule,
			getJob: getJob,
			getMethodFromState: getMethodFromState,
			getStateFromMethod: getStateFromMethod, 
			getSunRiseTime: getSunRiseTime,
			getSunSetTime: getSunSetTime,
			getSunData: getSunData,
			getTypeFromTriggerstate: getTypeFromTriggerstate,
			getTriggerstateFromType: getTriggerstateFromType,
			restoreJobs: restoreJobs,
			updateSunData: updateSunData
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

		updateSunData(new Date()); //default
		
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
		view.setProperty("mainHeight", 700); //TODO get this from somewhere
		view.setProperty("mainWidth", 800); //TODO get this from somewhere

		view.load("main.qml");
		application.addWidget("scheduler.simple", "icon.png", view);

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
		if(restoredJobsTimerKeys != undefined && restoredJobsTimerKeys[deviceId] != undefined){
			for(var i=0;i<restoredJobsTimerKeys[deviceId].length;i++){
				var waitForMore = (i == restoredJobsTimerKeys[deviceId].length-1) ? "true" : undefined;
				com.telldus.scheduler.removeJob(restoredJobsTimerKeys[deviceId][i]);
			}
			restoredJobsTimerKeys[deviceId] = undefined;
		}
		
		//add new schedules:
		var jobs = new Array();
		for(var i=0;i<points.length;i++){
			var jobtemp = getJob(points[i], callbackFunc);
			jobs.push(jobtemp);
		}
		saveJobs(deviceId, jobs);
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
		restoredJobsTimerKeys = restoreJobsToSchedule(storedJobs, updateLastRun); //Start timers here, save keys so they can be removed if jobs are updated
		return storedJobs;
	}
	
	function restoreJobsToSchedule(storedJobs, updateLastRun){
		var deviceTimerKeys = {};
		for(var devicekey in storedJobs){
			var jobs = new Array();
			for(var i=0;i<storedJobs[devicekey].length;i++){
				if(storedJobs[devicekey][i] == undefined){
					continue;
				}
				var jobtemp = restoreJob(storedJobs[devicekey][i], updateLastRun);
				jobs.push(jobtemp);
			}
			if(jobs.length > 0){
				deviceTimerKeys[devicekey] = com.telldus.scheduler.addJobs(jobs); //have to do this per device, so that the timerkeys are stored for each device
			}
		}
		return deviceTimerKeys;
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
	
	//Update last run for this job from storage, then store it again
	function updateLastRun(job, lastRun){
		var settings = new com.telldus.settings();
		var storedjobs = settings.value("jobs", "");
		for(var devicekey in storedjobs){
			if(devicekey == job.v.device){
				for(var i=0;i<storedjobs[devicekey].length;i++){
					if(storedjobs[devicekey][i].v.id == job.v.id){
						storedjobs[devicekey][i].v.lastRun = lastRun;
						break;
					}
				}
				break;
			}
		}
		
		settings.setValue("jobs", storedjobs);
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
	
	function updateSunData(date){
		sunData = com.telldus.suncalculator.riseset(date);
		return sunData;
	}
	
	function getSun(riseset, rowWidth, pointWidth){
		var timevalues = sunData;
		
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
	function getSunData(date){
		print("GET SUN DATA - can be made more efficient if called often...");
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
