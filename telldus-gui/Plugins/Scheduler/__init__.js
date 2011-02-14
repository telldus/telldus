__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.scheduler.init );
}
	
com.telldus.scheduler = function() {
	
	var storedJobs = new MappedList();  //all jobs, added by "addJob"
	var joblist; //sorted list containing next run time (and id) for all storedJobs
	var timerid = null; //id of currently running timer, used when timer should be aborted
	var queuedJob; //job currently in running timer, is already removed from joblist
	
	//TODO (perhaps) run something on the LAST day of the month?
	//TODO remove absolute events after being run... Cannot do that from here, must be done from loading/saving plugin
	//TODO Logic in future extended scenes/macros (not here, but somewhere in telldus center): Delays - job that then continues a scene from a certain step after the delay
	//TODO Logic in future extended scenes/macros (not here, but somewhere in telldus center): Conditions, see if certain condition is fulfilled when the schedule should
	//     run, for example engine heater if the temperature is below 5 degrees, or turn on light if it is cloudy (if not cloudy, wait 20 minutes
	//     Not only for schedule, other input (for example motion detectors) can be conditioned in the same way (for example, house inhabitants are set as "Gone").
	//     Repeat, if condition is not fulfilled, try again in x seconds
	//TODO Logic in future extended scenes/macros (not here, but somewhere in telldus center): "Stop scene", i. e. when device is turned on, automatically turn it off again after x seconds
	
	function init(){
		JobDaylightSavingReload.prototype = new com.telldus.scheduler.Job();
		setDaylightSavingJobFunctions();
		//loadJobs(); //TODO remove this after testing is done
	}
	
	
	function addJob(job){
		if(storedJobs.length == 0){
			print("Adding daylight saving time");
			var daylightSavingReloadKey = storedJobs.push(getDaylightSavingReloadJob());
			updateJobInList(daylightSavingReloadKey);
		}
		var key = storedJobs.push(job);
		job.key = key;
		updateJobInList(key);
		return key;
	}
	
	//add several jobs at once, withou recalculating timer between each
	function addJobs(jobs){
		if(storedJobs.length == 0){
			print("Adding daylight saving time");
			var daylightSavingReloadKey = storedJobs.push(getDaylightSavingReloadJob());
			updateJobInList(daylightSavingReloadKey, true); //waitForMore = don't sort array and recalculate timer yet
		}
		var returnKeys = new Array();
		for(var i=0;i<jobs.length;i++){
			var job = jobs[i];
			var key = storedJobs.push(job);
			job.key = key;
			updateJobInList(key, true); //waitForMore = don't sort array and recalculate timer yet
			returnKeys.push(key);
		}
		joblist.sort(compareTime);
		runNextJob();
		
		return returnKeys;
	}
	
	function fuzzify(currentTimestamp, fuzzinessBefore, fuzzinessAfter, lastRun){
		if(fuzzinessAfter != 0 || fuzzinessBefore != 0){
			var now = new Date().getTime();
			var fuzzinessBeforeOrig = fuzzinessBefore*1000;
			if(currentTimestamp - (fuzzinessBefore*1000) < now){
				//print("Already in interval");
				fuzzinessBefore = (currentTimestamp - now)/1000; //we have already entered the fuzzy-interval, move the start point to "now"...
			}
			//print("LASTRUN: " + lastRun + " - " + new Date(lastRun));
			//print("COMPARE: " + (currentTimestamp - (fuzzinessBefore*1000)) + " - " + new Date(currentTimestamp - (fuzzinessBefore*1000)));
			//print("COMPARE: " + (currentTimestamp - fuzzinessBeforeOrig) + " - " + new Date(currentTimestamp - fuzzinessBeforeOrig));
			if(lastRun != undefined && lastRun >= (currentTimestamp - fuzzinessBeforeOrig)){
				//print("JUST RETURNING");
				return 0; //job already run in this interval, don't run it again...
			}
			//print("CONTINUING HAPPILY");
			var interval = fuzzinessAfter + fuzzinessBefore;
			var rand =  Math.random(); //Random enough at the moment
			var fuzziness = Math.floor((interval+1) * rand);
			fuzziness = fuzziness - fuzzinessBefore;
			currentTimestamp += (fuzziness * 1000);
		}
		return currentTimestamp;
	}
	
	function getDaylightSavingReloadJob(){
		return new JobDaylightSavingReload();
	}
	
	function recalculateAllJobs(){
		print("Recalculating all jobs");
		
		joblist = new Array();
		
		for(var key in storedJobs.container){
			var job = storedJobs.get(key);
			var nextRunTime = job.getNextRunTime();
			print("Run time: " + new Date(nextRunTime));
			if(nextRunTime === null){
				print("Will not run");
				continue;
			}
			joblist.push(new RunJob(key, nextRunTime));
		}
			
		joblist.sort(compareTime);
		runNextJob();
	}
	
	function removeFromJobList(id){
		if(!joblist){
			return;
		}
		for(i=0;i<joblist.length;i++){
			if(id==joblist[i].id){
				joblist.splice(i, 1);
				return;
			}
		}
	}
	
	function removeJob(id, waitForMore){
		if(queuedJob.id == id){
			queuedJob = undefined;
		}
		
		storedJobs.remove(id);
		updateJobInList(id, waitForMore); //wait for more
		if(storedJobs.length == 1){
			//only one job left, it's only the DaylightSaving reload job, remove that too
			for(var key in storedJobs.container){
				storedJobs.remove(key);
				updateJobInList(key);
			}
		}
	}
	
	function runJob(id){
		print("Running job, will execute");
		queuedJob = null;
		var success = storedJobs.get(id).execute();
		print("Job run, after delay " + id);
		updateJobInList(id);	
	}
	
	function runNextJob(){
		if(timerid != null){
			clearTimeout(timerid);
			print("Timer interrupted");
		}
		if(joblist.length <= 0){
			print("No jobs");
			return; //no jobs, abort
		}
		if(queuedJob){
			//put the currently queued job back in the list, so that it can be compared again
			print("Queued job is something, put it back in list");
			joblist.push(queuedJob);
			joblist.sort(compareTime);
		}
		var job = joblist.shift(); //get first job in list (and remove it from the list)
		queuedJob = job; //put it in list, to keep track of current job
		var nextRunTime = job.nextRunTime;
		
		if(nextRunTime === null){
			//something is wrong
			print("Something is wrong");
			updateJobInList(job.id); //This will just recalculate the job, and probably return 0 again, but updateJobInList won't add it to the list in that case (shouldnt end up here at all now actually)
			return;
		}
		var runJobFunc = function(){ runJob(job.id); };
		var now = new Date().getTime();
		var delay = nextRunTime - now;
		print("Will run " + storedJobs.get(job.id).v.name + " (" + job.id + ") at " + new Date(nextRunTime)); //Note not all will have a name
		print("(Now is " + new Date() + ")");
		timerid = setTimeout(runJobFunc, delay); //start the timer
		
		print("Has started a job wait");
	}
	
	function updateJobInList(id, waitForMore){
		if(!joblist){
			joblist = new Array();
		}
		if(!storedJobs.contains(id)){
			removeFromJobList(id);
			runNextJob();
			return;
		}
		
		var job = storedJobs.get(id);
		var nextRunTime = job.getNextRunTime();
		print("Time updated to: " + new Date(nextRunTime));
		
		if(nextRunTime === null){
			print("Will not run this one");
			removeFromJobList(id); //remove from joblist if it exists there (run time may have been updated to something invalid/already passed)
			runNextJob(); //resort list (may have changed), run next
			return;
		}
		
		joblist.push(new RunJob(id, nextRunTime));
		if(waitForMore == undefined){		
			joblist.sort(compareTime);
			runNextJob();
		}
	}
	
	function updateJob(key, job){
		if(!storedJobs.contains(key)){
			return;
		}
		storedJobs.update(key, job);
		updateJobInList(key);
	}
	
	function updateLastRun(id, lastRun){
		print("Update last run: " + id + " to " + lastRun);
		storedJobs.get(id).v.lastRun = lastRun; //update current list
	}
	
	
	function JobDaylightSavingReload(){
		this.v.name = "Daylight Saving Time job";
	}
	
	function setDaylightSavingJobFunctions(){
	
		JobDaylightSavingReload.prototype.execute = function(){
			//override default
			print("Daylight savings job");
			//TODO Make sure this job is run "last", if other jobs are set to be runt the same second
			setTimeout(recalculateAllJobs(), 1); //sleep for one ms, to avoid strange calculations
			//this may lead to that things that should be executed exactly at 3.00 when
			//moving time forward one hour won't run, but that should be the only case
			return 0;
		};
		
		JobDaylightSavingReload.prototype.getNextRunTime = function(){
			print("getNextRunTime DaylightSaving");
			var dst = DstDetect();
			if(dst[0] == ""){
				//not using dst in this timezone, still add it to the lists to keep it consistent (will be added as 1/1 1970)
				print("Not using timezone");;
				return null;
			}
			var now = new Date().getTime();
			var time = dst[0].getTime();
			if(now > time){
				//already passed
				time = dst[1].getTime();
			}
			return time;
		}
	}
	
	function MappedList() {
		this.container = {};
		this.length = 0;
	}
	
	MappedList.prototype.contains = function(key){
		return !(this.container[key] === undefined);
	}
	
	MappedList.prototype.get = function(key){
		return this.container[key];
	}
	
	MappedList.prototype.push = function(element){
		//TODO reusing keys at the moment, that's ok, right?
		var length = this.length;
		this.container[length] = element;
		this.length = length + 1;
		return length;
	}
	
	MappedList.prototype.remove = function(key){
		delete this.container[key];
		this.length--;
	}
	
	MappedList.prototype.update = function(key, element){
		this.container[key] = element;
	}

	function RunJob(id, nextRunTime){ //, type, device, method, value){
		this.id = id;
		this.nextRunTime = nextRunTime;
	}	

	return { //Public functions
		addJob: addJob, //job, returns: storage id
		addJobs: addJobs, //jobs (array), returns: storage id's in array
		fuzzify: fuzzify, //timestamp, max fuzziness before, max fuzziness after, returns: new random timestamp within min/max fuzziness-boundries
		removeJob: removeJob, //storage id
		updateJob: updateJob, //storage id, job
		updateLastRun: updateLastRun, //id, datetimestamp
		//TODO getNextRunForJob? For all? (to see when job is due to run next)
		init:init
	}
}();

function compareTime(a, b) {
	return a.nextRunTime - b.nextRunTime;
}

com.telldus.scheduler.Job = function(jobdata) {
	if(jobdata){
		this.v = jobdata;
	}
	else{
		this.v = {};
	}
}

com.telldus.scheduler.Job.prototype.execute = function(){
	//may be overridden if other than device manipulation should be performed
	var success = 0;
	if(this.v.executeFunc != null){
		success = this.v.executeFunc(this);
		this.updateJobLastRun();
		return success;
	}	
	
	success = this.executeDefault();
	
	//if(success){
		//update last run even if not successful, else it may become an infinite loop (if using pastGracePeriod)
		this.updateJobLastRun();
	//}
	return success;
};

com.telldus.scheduler.Job.prototype.executeDefault = function(){
	var success = 0;
	deviceid = this.v.device;
	var method = parseInt(this.v.method);
	//print("Job id: " + this.v.device + " Method: " + method);
	switch(method){
		case com.telldus.core.TELLSTICK_TURNON:
			success = com.telldus.core.turnOn(deviceid);
			break;
		case com.telldus.core.TELLSTICK_TURNOFF:
			success = com.telldus.core.turnOff(deviceid);
			break;
		case com.telldus.core.TELLSTICK_DIM:
			success = com.telldus.core.dim(deviceid, this.v.value);
			break;
		case com.telldus.core.TELLSTICK_BELL:
			success = com.telldus.core.bell(deviceid);
			break;	
		default:
			break;
	}
	return success;
};

com.telldus.scheduler.Job.prototype.getNextRunTime = function(){
	print("getNextRunTime default");
	return null; //default
}

com.telldus.scheduler.Job.prototype.updateJobLastRun = function(){
	var timestamp = new Date().getTime();
	com.telldus.scheduler.updateLastRun(this.key, timestamp);
	//override this to save last run to storage too, but don't forget to call the above method too		
}

include("DefaultJobTypes.js");
include("DaylightSavingTime.js");