com.telldus.scheduler.JOBTYPE_ABSOLUTE = 0;
com.telldus.scheduler.JOBTYPE_RECURRING_DAY = 1;
com.telldus.scheduler.JOBTYPE_RECURRING_WEEK = 2;
com.telldus.scheduler.JOBTYPE_RECURRING_MONTH = 3;

com.telldus.scheduler.EVENTTYPE_ABSOLUTE = 0;
com.telldus.scheduler.EVENTTYPE_SUNRISE = 1;
com.telldus.scheduler.EVENTTYPE_SUNSET = 2;


function getEventRunTime(event, date){
	var currentEventRuntimeTimestamp = null;
	if(event.d.type == com.telldus.scheduler.EVENTTYPE_ABSOLUTE){
		currentEventRuntimeTimestamp = (event.d.time*1000) + date;
		currentEventRuntimeTimestamp = com.telldus.scheduler.fuzzify(currentEventRuntimeTimestamp, parseInt(event.d.fuzzinessBefore), parseInt(event.d.fuzzinessAfter));
	}
	else if(event.d.type == com.telldus.scheduler.EVENTTYPE_SUNRISE || event.d.type == com.telldus.scheduler.EVENTTYPE_SUNSET){
		currentEventRuntimeTimestamp = getSunUpDownForDate(date, parseInt(event.d.type));
		currentEventRuntimeTimestamp += (event.d.offset * 1000);
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
		if(timeSinceLastRun > v.pastGracePeriod){
			pastGracePeriod = v.pastGracePeriod;
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
* but wasn't, because of a reboot or similar (in ms)
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
	//TODO hur ska id på event och job sättas för nya jobb/events?
	//eventid är bara unika inom respektive jobb...
	//TODO temp - creating events
	var now = new Date();
	var time1 = now.getHours() * 3600 + now.getMinutes() * 60 + now.getSeconds();
	print(time1); // 48880
	time2 = time1 + 30;
	time3 = time1 - 60;
	time1 = time1 + 50;
	
	var startdate1 = new Date(2011,0,5).getTime();
	var startdate2 = new Date(2011,0,5).getTime();
	
	var newRecurringMonthJob = getJob({id: 4, deviceid: 11, name: "testnamn14", type: com.telldus.scheduler.JOBTYPE_RECURRING_WEEK, startdate: startdate1, lastRun: 0, device: 1, method: 1, value: ""});
	newRecurringMonthJob.addEvent(new Event({id: 0, value: 3, fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 0, time: time1}));
	newRecurringMonthJob.save();
	
	var newAbsoluteJob = getJob({id: 5, deviceid: 12, name: "testnamn15", type: com.telldus.scheduler.JOBTYPE_RECURRING_MONTH, startdate: startdate2, lastRun: 0, device: 1, method: 1, value: "", pastGracePeriod: 90000});
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


com.telldus.scheduler.JobAbsolute = function(jobdata){}
com.telldus.scheduler.JobRecurringDay = function(jobdata){}
com.telldus.scheduler.JobRecurringWeek = function(jobdata){}
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
		nextRunTime = getNextEventRunTime(nextRunTime, this.v.events[key], parseInt(this.v.events[key].d.value), pastGracePeriod);
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
	nextRunTime = getNextEventRunTime(0, this.v.events[0], date.getTime(), pastGracePeriod);
	if(nextRunTime < new Date().getTime()){
		var runTime = new Date(date);
		runTime.setDate(runTime.getDate() + parseInt(this.v.events[0].d.value));
		nextRunTime = getNextEventRunTime(0, this.v.events[0], runTime.getTime(), pastGracePeriod); //already passed this time today, try again after "interval" days 
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
		
		nextTempRunTime = getNextEventRunTime(0, this.v.events[key], returnDate.getTime(), pastGracePeriod);
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
