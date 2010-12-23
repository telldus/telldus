__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.scheduler.init );
}

com.telldus.scheduler = function() {

	var JOBTYPE_ABSOLUTE = 0;
	var JOBTYPE_RECURRING_DAY = 1;
	var JOBTYPE_RECURRING_WEEK = 2;
	var JOBTYPE_RECURRING_MONTH = 3;
	var JOBTYPE_RELOAD = 4; //TODO for summer/winter time, any better ideas?

	var EVENTTYPE_ABSOLUTE = 0;
	var EVENTTYPE_SUNRISE = 1;
	var EVENTTYPE_SUNSET = 2;
	
	var storedJobs;
	var joblist;
	
	
	//1. hämta redan satta jobb
	//(kolla om något jobb borde ha körts sedan förra ggn (och att det inte kördes då))
	//2. räkna ut när de ska köras nästa ggn, inkludera solens upp/nergång, fuzziness etc
	//3. ordna i lista, spara undan första tidsvärdet enkelt åtkomligt
	//4. kör tills tidsvärdet <= timestamp, kolla en ggn/sekund...
	
	// räkna om varje ggn ngn ändring sker med tider, läggs till/tas bort...


	//hur ska jobb sparas?
	//vilka typer av tider ska finnas?
	// absoluta värden - ett enda tillfälle
	// fr.o.m viss tidpunkt:
	// återkommande, viss tid varje dag
	// återkommande, viss tid vissa veckodagar (ev. var x:e vecka) (samlat i ett jobb, eller samma tid för alla markerade dagar?)
	// återkommande, viss tid vissa dagar i månaden (datum och "siste", eventuellt (är detta någonsin intressant för tellstickstyrning?) första-andra-tredje-fjärde-sista veckodagen i månaden)
	// återkommande, viss tid var x:e dag (hyfsat enkelt att implementera, men är detta intressant för tellstickstyrning?)
	// fuzzy på alla, x min före och x min efter (kan klart sättas till noll), vid TelldusCenter-start slumpas ett absolutvärde inom detta intervall fram
	// solens upp/nedgång kan väljas som tidpunkt på alla dagar
	// viss tid från viss action bestäms från den actionen, om t.ex. en scen ska utföra ngt efter en viss tid så får den göra ett exakt-tid-tillägg (hur det ska fungera vet jag inte, 
			//men tycker att scener ska kunna innehålla tidsfördröjningar mha detta... Men hur återuppta scen liksom? Kanske kunde man låta "värde" i en scen vara from vilket steg det ska
			//återupptas? Varför inte? I framtiden alltså. Execute får alltså ett värde (default inget = från start))
	// varje jobbtidpunkt måste alltså lagras som: 
	// ----------------------------
	// typ - dagintervall (det normala, 1), - viss veckodag (lista med aktiva veckodagar), - viss dag i månaden, speciellt värde för "den siste", - speciella tidpunkt(er)
	// varje dag/tidpunkt kan ha följande värden (om man gör det per dag):
		// fuzzy innan
		// fuzzy efter
		// använd solens uppgång/nedgång/absolut tidpunkt
		// på/avdrag från solens upp/nedgång
	// startdag
	// tid för föregående körning, om någon
	// -------------------------------
	// Framtidssäkring: condition för t.ex. väderstation - nej, inga conditions finns ännu... vill man ha det i framtiden?
	// typ, när schemat ska exekveras, kolla om ngt är uppfyllt... Nej, det får i så fall ske i en scen (inte scen, något annat mellansteg i TelldusCenter (inte tellduscore)...
	// när scenen körs, kolla väderdata, om det är gråmulet=tänd, annars kör scen som pausar i 30 minuter och tänder då...
	// Samma sak med villkoret "hemma" och "borta"... Det får vara per scen (mellanstegsscen). Andra events (t.ex. rörelsekontrollevents) vill ju vara villkorade på precis samma sätt,
	// alltså kör scenen om villkoret är "borta" eller temperatur < -10...
	// Och en mellanstegsscen kan (ska kunna innehålla) en "stopscen" som ska köras en viss tid senare (t.ex. låta ljuset vara igång i 2 minuter, sedan släcka), då skapar scenen
	// ett nytt schemajobb med argumentet +2 minuter...
	// solen går upp/ner +/- visst antal minuter...
	// hur kommer net:en + schemaläggare att fungera? Kommer det att finnas en webvariant?
	//jobben i listan = deviceid (även grupp/scen såklart), action, värde. En enda / jobb, får grupperas med grupper/scener om man vill att mer ska hända på en ggn

	//TODO obs, om något jobb existerar, lägg till 1 nytt jobb som körs vid nästa sommartid/vintertid och bara laddar om befintliga jobb... Listan måste ju göras om då. (EVENTTYPE_RELOAD)
	//TODO fem-minuter-bakåt-grejen
	//TODO reload...?
	//TODO ordna upp, dela upp i flera filer
	//TODO funktionerna, rätt placerade nu, eller ngn som eg. tillhör bara en subklass?
	//TODO ta bort absoluta events efter att de har passerats?
	//TODO functions for remove event and job
	//TODO how to create new jobs and their events? Needs different arguments (all in v) but...
	
	//"repeat", t.ex. om villkor inte uppfylls så vill man göra ett nytt försök, även det får vara en funktion i extended scen/makro. if->false->tryAgainAfterXSeconds...
	
	// gränssnittet... hur...?
	//
	
	function init(){
		loadJobs(); //load jobs from permanent storage
		calculateJobs(); //TODO recalculate jobs on every run/change, reload jobs on every job change
		runNextJob();
	}
	
	function runNextJob(){
		if(joblist.length <= 0){
			print("No jobs");
			return; //no jobs, abort
		}
		var job = joblist.shift(); //get first job in list (and remove it from the list)
		var nextRunTime = job.nextRunTime;
		print("Will run " + storedJobs[job.id].v.name + " when the time is right");
		
		if(nextRunTime == 0){
			//something is wrong
			print("Something is wrong");
			updateJobInList(job.id); //This will just recalculate the job, and probably return 0 again, but updateJobInList won't add it to the list in that case (shouldnt end up here at all now actually)
			runNextJob();
			return;
		}
		
		var runJobFunc = function(){ runJob(job); };
		var now = new Date().getTime();
		var delay = nextRunTime - now;
		print("Runtime: " + new Date(nextRunTime));
		print("Delay: " + delay);
		setTimeout(runJobFunc, delay); //start the timer
	}
	
	function runJob(runJob) {
		var success = 0;
		print("Job id: " + runJob.id);
		if(runJob){
			var method = parseInt(runJob.method);
			switch(method){
				case com.telldus.core.TELLSTICK_TURNON:
					success = com.telldus.core.turnOn(runJob.id);
					break;
				case com.telldus.core.TELLSTICK_TURNOFF:
					success = com.telldus.core.turnOff(runJob.id);
					break;
				case com.telldus.core.TELLSTICK_DIM:
					success = com.telldus.core.dim(runJob.id, runJob.value);
					break;
				case com.telldus.core.TELLSTICK_BELL:
					success = com.telldus.core.bell(runJob.id);
					break;	
				default:
					break;
			}
		}
		
		if(success){
			updateLastRun(runJob.id, new Date().getTime());
		}
		print("Job run " + runJob.id);
		updateJobInList(runJob.id);
		runNextJob();
	};
	
	function updateLastRun(id, lastRun){
		var settings = new com.telldus.settings();
		var jobs = settings.value("jobs", "");
		jobs[id].lastrun = lastRun; //update permanent storage
		settings.setValue("jobs", jobs);
		
		storedJobs[id].lastrun = lastRun; //update current list
	}
	
	function updateJobInList(id){
		var job = storedJobs[id];
		var nextRunTime = job.getNextRunTime();
		print("Time updated: " + new Date(nextRunTime));
		
		if(nextRunTime == 0){
			return;
		}
		
		joblist.push(new RunJob(job.v.id, nextRunTime, job.v.type, job.v.device, job.v.method, job.v.value));
		
		joblist.sort(compareTime);
	}
	
	function calculateJobs(){
		print("Calculate jobs");
		if(!joblist){
			joblist = new Array();
		}
		for(var key in storedJobs){
			var job = storedJobs[key];
			var nextRunTime = job.getNextRunTime();
			print("Run time: " + new Date(nextRunTime));
			if(nextRunTime == 0){
				print("Will not run");
				return;
			}
			joblist.push(new RunJob(job.v.id, nextRunTime, job.v.type, job.v.device, job.v.method, job.v.value));
		}
			
		joblist.sort(compareTime);
	}
	
	function loadJobs(){
		print("Loading jobs");
		//TODO hur ska id på event och job sättas för nya jobb/events?
		//eventid är bara unika inom respektive jobb...
		//TODO temp - creating events
		//TODO datum i new date? 11 eller 12?
		/*
		var newRecurringMonthJob = getJob({id: 4, name: "testnamn14", type: JOBTYPE_RECURRING_WEEK, startdate: "2010-01-01", lastrun: 0, device: 1, method: 1, value: ""});
		newRecurringMonthJob.addEvent(new Event({id: 0, value: 3, fuzzinessBefore: 0, fuzzinessAfter: 0, type: EVENTTYPE_SUNSET, offset: 0, time: 0}));
		newRecurringMonthJob.save();
		
		var newAbsoluteJob = getJob({id: 5, name: "testnamn15", type: JOBTYPE_RECURRING_MONTH, startdate: "2010-01-01", lastrun: 0, device: 1, method: 1, value: ""});
		newAbsoluteJob.addEvent(new Event({id: 1, value: "11-24", fuzzinessBefore: 0, fuzzinessAfter: 30, type: EVENTTYPE_ABSOLUTE, offset: 0, time: 43200}));
		newAbsoluteJob.addEvent(new Event({id: 2, value: "11-05", fuzzinessBefore: 0, fuzzinessAfter: 30, type: EVENTTYPE_SUNSET, offset: 0, time: 0}));
		newAbsoluteJob.save();
		*/		
		storedJobs = {};
		//get all jobs from permanent storage
		var settings = new com.telldus.settings();
		var storedJobsData = settings.value("jobs", "");
		
		for(var key in storedJobsData){
			var jobdata = storedJobsData[key];
			var job = getJob(jobdata);
			storedJobs[key] = job;
		}
		
		//updateLastRun(newRecurringMonthJob.v.id, "2005-05-05"); //TODO remove
	}
	
	function getJob(jobdata){
		//factory function... typ
		var job = new Job();
		var type = parseInt(jobdata.type);
		
		switch(type){
			case JOBTYPE_ABSOLUTE:
				job = new JobAbsolute(jobdata);
				break; 
			case JOBTYPE_RECURRING_DAY:
				job = new JobRecurringDay(jobdata);
				break; 
			case JOBTYPE_RECURRING_WEEK:
				job = new JobRecurringWeek(jobdata);
				break; 
			case JOBTYPE_RECURRING_MONTH:
				job = new JobRecurringMonth(jobdata);
				break; 	
			//TODO reload-job
			default:
				break;
		}
		job.v = jobdata;
		return job;
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
	
	function zeroTime(date){
		date.setHours(0);
		date.setMinutes(0);
		date.setSeconds(0);
		date.setMilliseconds(0);
		return date;			
	}
	
	function getNextEventRunTime(nextRunTime, event, date){
		
		var currentEventRuntimeTimestamp = getEventRunTime(event, date);
		if((nextRunTime == 0 || currentEventRuntimeTimestamp < nextRunTime) && currentEventRuntimeTimestamp > new Date().getTime()){   //earlier than other events, but later than "now"
			//TODO later than "now" could be later than "now - 5 minutes" if settings says so, if reboot for example (and in that case check last run)
			nextRunTime = currentEventRuntimeTimestamp;
		}
		
		return nextRunTime;
	}
	
	function getEventRunTime(event, date){
		var currentEventRuntimeTimestamp = 0;
		if(event.d.type == EVENTTYPE_ABSOLUTE){
			currentEventRuntimeTimestamp = (event.d.time*1000) + date;
			currentEventRuntimeTimestamp = fuzzify(currentEventRuntimeTimestamp, parseInt(event.d.fuzzinessBefore), parseInt(event.d.fuzzinessAfter));
		}
		else if(event.d.type == EVENTTYPE_SUNRISE || event.d.type == EVENTTYPE_SUNSET){
			currentEventRuntimeTimestamp = getSunUpDownForDate(date, parseInt(event.d.type));
			currentEventRuntimeTimestamp += (event.d.offset * 1000);
			currentEventRuntimeTimestamp = fuzzify(currentEventRuntimeTimestamp, parseInt(event.d.fuzzinessBefore), parseInt(event.d.fuzzinessAfter));		
		}
		
		return currentEventRuntimeTimestamp;
	}
	
	function fuzzify(currentTimestamp, fuzzinessBefore, fuzzinessAfter){
		if(fuzzinessAfter != 0 || fuzzinessBefore != 0){
			var interval = fuzzinessAfter + fuzzinessBefore;
			var rand =  Math.random(); //TODO random enough?
			var fuzziness = Math.floor((interval+1) * rand);
			fuzziness = fuzziness - fuzzinessBefore;
			currentTimestamp += (fuzziness * 1000);
		}
		return currentTimestamp;
	}
	
	function getSunUpDownForDate(datetimestamp, sun){
	
		date = new Date(datetimestamp);
		var timevalues = com.telldus.suncalculator.riseset(date);
		if(timevalues[2] && timevalues[2] != ""){
			return ""; //no sun up or down this day, do nothing
		}
		var hourminute;
		if(sun == EVENTTYPE_SUNRISE){
			hourminute = timevalues[0].split(':');
		}
		else{
			hourminute = timevalues[1].split(':');
		}
		date.setHours(hourminute[0]);
		date.setMinutes(hourminute[1]);
		return date.getTime();
	
	}
	
	function Job(jobdata) {
		if(jobdata){
			this.v = jobdata;
		}
		else{
			this.v = {};
		}
	}
	
	Job.prototype.addEvent = function(event){
		if(!this.v.events){
			this.v.events = {};
		}
		this.v.events[event.d.id] = event;
	}
	
	Job.prototype.getNextRunTime = function(){
		print("getNextRunTime default");
		return 0; //default
	}
	
	Job.prototype.save = function(){
		//TODO set properties
		var settings = new com.telldus.settings();
		var jobs = settings.value("jobs", "");
		
		if(!jobs){
			jobs = {}; //initialize new
		}
		
		jobs[this.v.id] = this.v;
		
		settings.setValue("jobs", jobs);
	}
	
	function JobAbsolute(jobdata){
		//this.Job(jobdata);
		/*
		this.v.type = JOBTYPE_ABSOLUTE;
		var events = new Array();
		//foreach stored event...
		events.push(new Event(1)); //TODO
		//events.push(new Event(2));
		this.v.events = events;
		*/
	}
	
	function JobRecurringDay(jobdata){
		//this.Job(jobdata);
		/*this.v.type = JOBTYPE_RECURRING_DAY;
		this.v.event = new Event(1); //TODO id
		*/
	}
	
	function JobRecurringWeek(jobdata){
		//this.Job(jobdata);
		/*this.v.type = JOBTYPE_RECURRING_WEEK;
		var events = new Array();
		//foreach stored event...
		events.push(new Event(1)); //TODO
		//events.push(new Event(2));
		this.v.events = events;
		*/
	}
	
	function JobRecurringMonth(jobdata){
		//this.Job(jobdata);
		/*this.v.type = JOBTYPE_RECURRING_MONTH;
		var events = new Array();
		//foreach stored event...
		events.push(new Event(1)); //TODO
		//events.push(new Event(2));
		this.v.events = events;
		//this.time = 30000; hm, using events instead for now
		*/
	}
	
	JobAbsolute.prototype = new Job(); //Job.prototype;
	JobRecurringDay.prototype = new Job(); //Job.prototype;
	JobRecurringWeek.prototype = new Job(); //Job.prototype;
	JobRecurringMonth.prototype = new Job(); //Job.prototype;
	
	JobAbsolute.prototype.getNextRunTime = function(){
		print("getNextRunTime absolute");
		//Get all events in this job (absolute = 
		//kan vara flera absoluta datum och tidpunkter på ett jobb)
		//var events = job.events;
		var nextRunTime = 0;
		if(!this.v.events){
			return 0;
		}
		for(var key in this.v.events){
			nextRunTime = getNextEventRunTime(nextRunTime, this.v.events[key], parseInt(this.v.events[key].d.value));
		}
		return nextRunTime;
	}
	
	JobRecurringDay.prototype.getNextRunTime = function(){
		print("getNextRunTime day");
		//Recurring day (every day, every other day or every x day)
		//only one event/job (at the moment at least)
		//TODO test this
		var nextRunTime = 0;
		var date;
			
		if(this.v.lastRun > 0){
			var lastRunDate = new Date(this.lastRun);	
			date = new Date(lastRunDate.getFullYear(), lastRunDate.getMonth(), lastRunDate.getDate());
			date = date.getTime() + this.v.events[0].d.value;   //add interval
		}
		else{
			var now = new Date(); //Now
			date = new Date(now.getFullYear(), now.getMonth(), now.getDate());
		}
		nextRunTime = getNextEventRunTime(0, this.v.events[0], date.getTime());
		if(nextRunTime < new Date().getTime()){
			var runTime = new Date(date);
			runTime.setDate(runTime.getDate() + parseInt(this.v.events[0].d.value));
			nextRunTime = getNextEventRunTime(0, this.v.events[0], runTime.getTime()); //already passed this time today, try again after "interval" days 
		}
		
		return nextRunTime;
	}
	
	JobRecurringWeek.prototype.getNextRunTime = function(){
		print("getNextRunTime week");
		var nextRunTime = 0;
		if(!this.v.events){
			return 0;
		}
		for(var key in this.v.events){
			//get next correct day of week, may be today too
			var weekday = parseInt(this.v.events[key].d.value);
			if(weekday > 6 || weekday <0){
				print("Incorrect weekday value");
				continue;
			}
			var returnDate = new Date();
			var returnDay = returnDate.getDay();
			if (weekday !== returnDay) {
				returnDate.setDate(returnDate.getDate() + (weekday + (7 - returnDay)) % 7);
			}
			
			returnDate = zeroTime(returnDate);
			
			nextTempRunTime = getNextEventRunTime(0, this.v.events[key], returnDate.getTime());
			if(nextTempRunTime < new Date().getTime()){ //event happened today, already passed, add to next week instead
				returnDate.setDate(returnDate.getDate() + 7);
				nextRunTime = getNextEventRunTime(nextRunTime, this.v.events[key], returnDate.getTime());	
			}
			else{
				nextRunTime = nextTempRunTime;
			}
		}
		
		return nextRunTime;
	}
	
	JobRecurringMonth.prototype.getNextRunTime = function(){
		print("getNextRunTime month");
		//TODO test this
		var nextRunTime = 0;
		if(!this.v.events){
			return 0;
		}
		for(var key in this.v.events){
			//get next x day of month, may be today, for the current month
			if(this.v.events[key].d.value.toString().indexOf("-") == -1){ //make sure value is of correct format
				continue;
			}
			var daymonth = this.v.events[key].d.value.split('-'); //month-day
			var month = daymonth[0];
			var day = daymonth[1];
			var now = new Date();
			
			if((32 - new Date(now.getFullYear(), month, 32).getDate()) < day){ //check that day exists for this month
				if(month != 1 && day != 29){
					break; //this day doesn't exist for this month (at least not this year (leap year...))
				}
			}
			print("TESTDATE: " + new Date(now.getFullYear(), month, day));
			var nextdate = getNextLeapYearSafeDate(now.getFullYear(), month, day, this.v.events[key]);
			
			if(nextdate < new Date().getTime()){ //event already happened this year, add to next year instead
				var tempdate = new Date(nextdate);
				tempdate.setYear(now.getFullYear() + 1);
				nextdate = tempdate.getTime();
			}
			print("Candidate date: " + new Date(nextdate));
			if(nextRunTime == 0 || nextRunTime > nextdate){
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

	function RunJob(id, nextRunTime, type, device, method, value){
		this.id = id;
		this.nextRunTime = nextRunTime;
		this.type = type;
		this.device = device;
		this.method = method;
		this.value = value;
	}	

	return { //Public functions
		loadJobs: loadJobs,
		calculateJobs: calculateJobs,
		runNextJob: runNextJob, //TODO which methods should be exposed? Should some of these methods always call another?
		init:init
	}
}();

function compareTime(a, b) {
	return a.nextRunTime - b.nextRunTime;
}
