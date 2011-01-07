com.telldus.scheduler.JOBTYPE_ABSOLUTE = 0;
com.telldus.scheduler.JOBTYPE_RECURRING_DAY = 1;
com.telldus.scheduler.JOBTYPE_RECURRING_WEEK = 2;
com.telldus.scheduler.JOBTYPE_RECURRING_MONTH = 3;

com.telldus.scheduler.EVENTTYPE_ABSOLUTE = 0;
com.telldus.scheduler.EVENTTYPE_SUNRISE = 1;
com.telldus.scheduler.EVENTTYPE_SUNSET = 2;


function getEventRunTime(event, date){
	var currentEventRuntimeTimestamp = null;
	var offset = event.d.offset;
	if(isNaN(offset)){
		offset = 0;
	}
	if(event.d.type == com.telldus.scheduler.EVENTTYPE_ABSOLUTE){
		if(isNaN(date)){
			date = 0;
		}
		currentEventRuntimeTimestamp = (event.d.time * 1000) + date;
		currentEventRuntimeTimestamp += (offset * 1000); //this is really not useful for absolute values, but exists for consistency
		currentEventRuntimeTimestamp = com.telldus.scheduler.fuzzify(currentEventRuntimeTimestamp, parseInt(event.d.fuzzinessBefore), parseInt(event.d.fuzzinessAfter));
	}
	else if(event.d.type == com.telldus.scheduler.EVENTTYPE_SUNRISE || event.d.type == com.telldus.scheduler.EVENTTYPE_SUNSET){
		currentEventRuntimeTimestamp = getSunUpDownForDate(date, parseInt(event.d.type));
		currentEventRuntimeTimestamp += (offset * 1000);
		currentEventRuntimeTimestamp = com.telldus.scheduler.fuzzify(currentEventRuntimeTimestamp, parseInt(event.d.fuzzinessBefore), parseInt(event.d.fuzzinessAfter));		
	}
	
	return currentEventRuntimeTimestamp;
}

function getGracePeriod(v){
	var pastGracePeriod = 0;
	var lastRun = 0;
	if(v.lastRun){
		lastRun = v.lastRun;
	}
	if(v.pastGracePeriod){
		var timeSinceLastRun = (new Date().getTime() - lastRun);
		if(timeSinceLastRun > (v.pastGracePeriod * 1000)){
			pastGracePeriod = (v.pastGracePeriod * 1000);
		}
		else{
			pastGracePeriod = timeSinceLastRun;
		}
	}
		
	return pastGracePeriod;
}


function getJob(jobdata){
	//factory function... typ
	var job = new com.telldus.scheduler.Job();
	var type = parseInt(jobdata.type);
	
	switch(type){
		case com.telldus.scheduler.JOBTYPE_ABSOLUTE:
			job = new com.telldus.scheduler.JobAbsolute(jobdata);
			break; 
		case com.telldus.scheduler.JOBTYPE_RECURRING_DAY:
			job = new com.telldus.scheduler.JobRecurringDay(jobdata);
			break; 
		case com.telldus.scheduler.JOBTYPE_RECURRING_WEEK:
			job = new com.telldus.scheduler.JobRecurringWeek(jobdata);
			break; 
		case com.telldus.scheduler.JOBTYPE_RECURRING_MONTH:
			job = new com.telldus.scheduler.JobRecurringMonth(jobdata);
			break; 	
		default:
			break;
	}
	job.v = jobdata;
	return job;
}

/*
* pastGracePeriod - optional, run events that should have been run, for example, 5 minutes ago,
* but wasn't, because of a reboot or similar (in seconds)
* note, must in that case have checked last run	 
*/
function getNextEventRunTime(nextRunTime, event, date, pastGracePeriod){
	
	if(!pastGracePeriod){
		var pastGracePeriod = 0;
	}
	
	var currentEventRuntimeTimestamp = getEventRunTime(event, date);
	if((nextRunTime === null || currentEventRuntimeTimestamp < nextRunTime) && currentEventRuntimeTimestamp > (new Date().getTime() - pastGracePeriod)){   //earlier than other events, but later than "now"
		nextRunTime = currentEventRuntimeTimestamp;
	}
	
	return nextRunTime;
}

function getNextLeapYearSafeDate(year, month, day, event){
	if(month == 1 && day == 29){
		//get leap year
		for(var i=0;i<5;i++){
			if(new Date(year+i,1,29).getDate() == 29){
				//this is a leap year
				datetimestamp = getEventRunTime(event, new Date(year+1, 1, 29).getTime());
				if(datetimestamp > new Date().getTime()){ //else, this was a leap year, but already passed for this year
					return datetimestamp;
				}
			}
		}
	}
	else{
		var date = new Date(year, month, day);
		return getEventRunTime(event, date.getTime());
	}
}

function getSunUpDownForDate(datetimestamp, sun){
	date = new Date(datetimestamp);
	var timevalues = com.telldus.suncalculator.riseset(date);
	if(timevalues[2] && timevalues[2] != ""){
		return ""; //no sun up or down this day, do nothing
	}
	var hourminute;
	if(sun == com.telldus.scheduler.EVENTTYPE_SUNRISE){
		hourminute = timevalues[0].split(':');
	}
	else{
		hourminute = timevalues[1].split(':');
	}
	date.setHours(hourminute[0]);
	date.setMinutes(hourminute[1]);
	return date.getTime();
}

function loadJobs(){
	print("Loading jobs");
	//TODO detta ska inte göras från denna plugin, utan från respektive...
	//TODO temp - creating events
	var now = new Date();
	var time1 = now.getHours() * 3600 + now.getMinutes() * 60 + now.getSeconds();
	print(time1); // 48880
	time2 = time1 + 30;
	time3 = time1 - 60;
	time1 = time1 + 50;
	
	var startdate1 = new Date(2011,0,5).getTime();
	var startdate2 = new Date(2011,0,5).getTime();
	
	//ID = ID for storage
	//Key is position in list, returned from "addJob"
	var newRecurringMonthJob = getJob({id: 4, name: "testnamn14", type: com.telldus.scheduler.JOBTYPE_ABSOLUTE, startdate: startdate1, lastRun: 0, device: 1, method: 1, value: ""});
	newRecurringMonthJob.addEvent(new Event({id: 0, value: "", fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 10, time: (new Date().getTime())/1000 + 50}));
	newRecurringMonthJob.save();
	
	var newAbsoluteJob = getJob({id: 5, name: "testnamn15", type: com.telldus.scheduler.JOBTYPE_RECURRING_MONTH, startdate: startdate2, lastRun: 0, device: 1, method: 1, value: "", pastGracePeriod: 90});
	newAbsoluteJob.addEvent(new Event({id: 1, value: "00-05", fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 0, time: time2}));
	newAbsoluteJob.addEvent(new Event({id: 2, value: "00-05", fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 0, time: time3}));
	newAbsoluteJob.save();
			
	//get all jobs from permanent storage
	var settings = new com.telldus.settings();
	var storedJobsData = settings.value("jobs", "");
	
	for(var key in storedJobsData){
		var jobdata = storedJobsData[key];
		var job = getJob(jobdata);
		var tempkey = com.telldus.scheduler.addJob(job); //TODO, use different function than this = only sort list afterwards (one time) and dont start timer until all initial are added
	}
}

/*
 * updateLastRun-example:
	Job.prototype.updateJobLastRun(){
	var timestamp = new Date().getTime();
	com.telldus.scheduler.updateLastRun(this.key, timestamp);
	//override this to save last run to storage too, but don't forget to call the above method too
	var jobs = settings.value("jobs", "");
	jobs[this.v.id].v.lastRun = timestamp;
	settings.setValue("jobs", jobs);
}
*/

function zeroTime(date){
	date.setHours(0);
	date.setMinutes(0);
	date.setSeconds(0);
	date.setMilliseconds(0);
	return date;			
}


com.telldus.scheduler.Job.prototype.addEvent = function(event){
	if(!this.v.events){
		this.v.events = {};
	}
	this.v.events[event.d.id] = event;
}

com.telldus.scheduler.Job.prototype.save = function(){
	//TODO set properties
	var settings = new com.telldus.settings();
	var jobs = settings.value("jobs", "");
		
	if(!jobs){
		jobs = {}; //initialize new
	}
	
	jobs[this.v.id] = this.v;
	
	settings.setValue("jobs", jobs);
}

/*
 * Job for device manipulation on absolute run times (i.e. one time jobs). Job will run at one or several occations, where each occation is added as a separate event
 * To control more than one device, create a scene and run that one in the job.
 * Job properties:
 * id - id for storage purposes (e.g. removal/update), name - optional,
 * lastRun - 0 for all new jobs, device - id of device to perform action on,
 * method - action to perform on device, value - optional value to pass to method,
 * pastGracePeriod - if job run time has been missed (for example due to a reboot), run it on program startup, if it should have run
 *  within the last "pastGracePeriod" seconds
 * type is com.telldus.scheduler.JOBTYPE_ABSOLUTE
 * Event properties:
 * id - for storage purposes (e.g. removal/update)
 * fuzzinessBefore/fuzzinessAfter - an interval (in seconds) in which the runtime will be randomized, if the job should run at slightly different times each time
 * type - EVENTTYPE_ABSOLUTE, EVENTTYPE_SUNRISE or EVENTTYPE_SUNSET
 * time - time and date for event run, in seconds since 1970-01-01
 * offset - time (in seconds, positive or negative) to adjust the runtime, useful if something for example should occur 20 minutes past sunrise
 */
com.telldus.scheduler.JobAbsolute = function(jobdata){}

/*
 * Recurring job for device manipulation, same time every day or every x day ("value")
 * To control more than one device, create a scene and run that one in the job.
 * Schedule will not come into effect until "startdate" (timestamp) is passed
 * Extra/different job properties:
 * startdate - when schedule will come into effect,
 * pastGracePeriod - if job run time has been missed (for example due to a reboot), run it on program startup, if it should have run
 *  within the last "pastGracePeriod" seconds
 * type is com.telldus.scheduler.JOBTYPE_RECURRING_DAY
 * Extra/different event properties:
 * value - repeat every "value" day, starting from startdate,
 */
com.telldus.scheduler.JobRecurringDay = function(jobdata){}

/*
 * Recurring job for device manipulation, can have several events, each event has a time of day ("time") and weekday ("value", 0-6) when to run
 * To control more than one device, create a scene and run that one in the job.
 * If a job should run every weekday, 5 events are needed with values from 1-5
 * Schedule will not come into effect until "startdate" (timestamp) is passed
 * Extra/different job properties:
 * type is com.telldus.scheduler.JOBTYPE_RECURRING_WEEK
 * Extra/different event properties:
 * value - day of week
 */
com.telldus.scheduler.JobRecurringWeek = function(jobdata){}

/*
 * Recurring job for device manipulation, each event runs on a certain date of a certain month (i.e. next run for an event will be a year later)
 * To control more than one device, create a scene and run that one in the job.
 * If a job should run on a special day each month, 12 events will have to be created
 * Event value should be stored in the format "00-05", 00 being the month (January, 0-11) and 05 the day of the month
 * Schedule will not come into effect until "startdate" (timestamp) is passed
 * Extra/different job properties:
 * type is com.telldus.scheduler.JOBTYPE_RECURRING_MONTH
 * Extra/different event properties:
 * value - "month-day"
 */
com.telldus.scheduler.JobRecurringMonth = function(jobdata){}

com.telldus.scheduler.JobAbsolute.prototype = new com.telldus.scheduler.Job(); 
com.telldus.scheduler.JobRecurringDay.prototype = new com.telldus.scheduler.Job(); 
com.telldus.scheduler.JobRecurringWeek.prototype = new com.telldus.scheduler.Job(); 
com.telldus.scheduler.JobRecurringMonth.prototype = new com.telldus.scheduler.Job(); 

com.telldus.scheduler.JobAbsolute.prototype.getNextRunTime = function(){
	print("getNextRunTime absolute");
	var nextRunTime = null;
		
	if(!this.v.events){
		return null;
	}
	
	var pastGracePeriod = getGracePeriod(this.v);
	
	for(var key in this.v.events){
		nextRunTime = getNextEventRunTime(nextRunTime, this.v.events[key], 0, pastGracePeriod);
	}
	return nextRunTime;
}

com.telldus.scheduler.JobRecurringDay.prototype.getNextRunTime = function(){
	print("getNextRunTime day");
	//Recurring day (every day, every other day or every x day)
	//only one event/job (at the moment at least)
	var nextRunTime = null;
	var pastGracePeriod = getGracePeriod(this.v);
	var date;
	
	if(this.v.lastRun > 0){
		var lastRunDate = new Date(this.lastRun);	
		date = new Date(lastRunDate.getFullYear(), lastRunDate.getMonth(), lastRunDate.getDate());
		date = date.getTime() + this.v.events[0].d.value;   //add interval
	}
	else{
		var startTime = new Date(); //Now
		var minStartTime = new Date(this.v.startdate);
		
		if(minStartTime > startTime){
			//next run not until startTime is passed
			startTime = minStartTime;
		}
		date = new Date(startTime.getFullYear(), startTime.getMonth(), startTime.getDate());
	}
	nextRunTime = getNextEventRunTime(null, this.v.events[0], date.getTime(), pastGracePeriod);
	if(nextRunTime < new Date().getTime()){
		var runTime = new Date(date);
		runTime.setDate(runTime.getDate() + parseInt(this.v.events[0].d.value));
		nextRunTime = getNextEventRunTime(null, this.v.events[0], runTime.getTime(), pastGracePeriod); //already passed this time today, try again after "interval" days 
	}
	
	return nextRunTime;
}

com.telldus.scheduler.JobRecurringWeek.prototype.getNextRunTime = function(){
	print("getNextRunTime week");
	var nextRunTime = null;
	if(!this.v.events){
		return null;
	}
	
	var pastGracePeriod = getGracePeriod(this.v);
	
	for(var key in this.v.events){
		//get next correct day of week, may be today too
		var weekday = parseInt(this.v.events[key].d.value);
		if(weekday > 6 || weekday <0){
			print("Incorrect weekday value");
			continue;
		}
		var returnDate = new Date();
		var minStartTime = new Date(this.v.startdate);
		if(minStartTime > returnDate){
			//next run not until startTime is passed
			returnDate = minStartTime;
		}
		var returnDay = returnDate.getDay();
		if (weekday !== returnDay) {
			returnDate.setDate(returnDate.getDate() + (weekday + (7 - returnDay)) % 7);
		}
		
		returnDate = zeroTime(returnDate);
		
		nextTempRunTime = getNextEventRunTime(null, this.v.events[key], returnDate.getTime(), pastGracePeriod);
		if(nextTempRunTime < new Date().getTime()){ //event happened today, already passed, add to next week instead
			returnDate.setDate(returnDate.getDate() + 7);
			nextRunTime = getNextEventRunTime(nextRunTime, this.v.events[key], returnDate.getTime(), pastGracePeriod);	
		}
		else{
			nextRunTime = nextTempRunTime;
		}
	}
	
	return nextRunTime;
}

com.telldus.scheduler.JobRecurringMonth.prototype.getNextRunTime = function(){
	print("getNextRunTime month");
	var nextRunTime = null;
	if(!this.v.events){
		return null;
	}
	
	var pastGracePeriod = getGracePeriod(this.v);
	
	for(var key in this.v.events){
		//get next x day of month, may be today, for the current month
		if(!this.v.events[key].d.value || this.v.events[key].d.value.toString().indexOf("-") == -1){ //make sure value is of correct format
			continue;
		}
		var daymonth = this.v.events[key].d.value.split('-'); //month-day
		var month = daymonth[0];
		var day = daymonth[1];
		var startTime = new Date(); //Now
		var minStartTime = new Date(this.v.startdate);
		
		if((32 - new Date(startTime.getFullYear(), month, 32).getDate()) < day){ //check that day exists for this month
			if(month != 1 && day != 29){
				break; //this day doesn't exist for this month (at least not this year (leap year...))
			}
		}
		var nextdate = getNextLeapYearSafeDate(startTime.getFullYear(), month, day, this.v.events[key]);
		
		if((nextdate + pastGracePeriod) < new Date().getTime() || (nextdate + pastGracePeriod) < minStartTime){ //event already happened this year, or startTime not reached yet, add to next year instead
			var tempdate = new Date(nextdate);
			tempdate.setYear(startTime.getFullYear() + 1);
			if(tempdate < minStartTime){
				tempdate.setYear(tempdate.getFullYear() + 1); //no need to check if start date is reached this time, will always be recalculated before that, if nothing else at least by daylight saving time
			}
			nextdate = tempdate.getTime();
		}
		print("Candidate date: " + new Date(nextdate));
		if(nextRunTime === null || nextRunTime > nextdate){
			nextRunTime = nextdate;
		}
	}
	
	return nextRunTime;
}

function Event(data){
	if(data){
		this.d = data;
	}
	else{
		this.d = {};
	}
}
