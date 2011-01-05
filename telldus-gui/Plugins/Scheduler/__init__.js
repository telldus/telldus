__setupPackage__( __extension__ );

include("DaylightSavingTime.js");

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.scheduler.init );
}

com.telldus.scheduler = function() {

	var JOBTYPE_ABSOLUTE = 0; //jobtypes? Not here at least...
	var JOBTYPE_RECURRING_DAY = 1;
	var JOBTYPE_RECURRING_WEEK = 2;
	var JOBTYPE_RECURRING_MONTH = 3;
	//var JOBTYPE_RELOAD = 4; //for summer/winter time

	var EVENTTYPE_ABSOLUTE = 0;
	var EVENTTYPE_SUNRISE = 1;
	var EVENTTYPE_SUNSET = 2;
	
	var storedJobs;
	var joblist;
	var timerid;
	var queuedJob;
	
	
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
	//"repeat", t.ex. om villkor inte uppfylls så vill man göra ett nytt försök, även det får vara en funktion i extended scen/makro. if->false->tryAgainAfterXSeconds...
	
	//TODO ordna upp, dela upp i flera filer, inte ladda jobb här (per plugin istället), bara "add job" här...
	//TODO ta bort absoluta events efter att de har passerats? Kan inte göras härifrån, får på ngt sätt ske därifrån de sparas/laddas
	//TODO update last run, i respektive executemetod... bara?
	//det enda varje jobb har är getNextRunTime (som ska override:as) (och ev. updateLastRun)

	
	function init(){
		loadJobs(); //load jobs from permanent storage TODO move
	}
	
	function runNextJob(){
		clearTimeout(timerid);
		print("Timer interrupted");
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
		
		if(nextRunTime == 0){
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
		print("Delay: " + delay);
		timerid = setTimeout(runJobFunc, delay); //start the timer
		print("Has started a job wait");
	}
	
	function runJob(id){
		print("Running job, will execute");
		queuedJob = null;
		var success = storedJobs.get(id).execute();
		print("Job run, after delay " + id);
		updateJobInList(id);	
	}
	
	function updateLastRun(id, lastRun){
		//TODO... this will not be stored of course, how to do that? Maybe in the execute function per job too...
		print("Update last run: " + id + " to " + lastRun);
		storedJobs.get(id).v.lastRun = lastRun; //update current list
	}
	
	function updateJobInList(id){
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
		
		if(nextRunTime == 0){
			removeFromJobList(id); //remove from joblist if it exists there (run time may have been updated to something invalid/already passed)
			runNextJob(); //resort list (may have changed), run next
			return;
		}
		
		joblist.push(new RunJob(id, nextRunTime));   //, job.v.type, job.v.device, job.v.method, job.v.value));
		
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
	
	function recalculateAllJobs(){
		print("Recalculating all jobs");
		
		joblist = new Array();
		
		for(var key in storedJobs.container){
			var job = storedJobs.get(key);
			var nextRunTime = job.getNextRunTime();
			print("Run time: " + new Date(nextRunTime));
			if(nextRunTime == 0){
				print("Will not run");
				continue;
			}
			joblist.push(new RunJob(key, nextRunTime));
		}
			
		joblist.sort(compareTime);
		runNextJob();
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
		
		var newRecurringMonthJob = getJob({id: 4, name: "testnamn14", type: JOBTYPE_RECURRING_WEEK, startdate: startdate1, lastRun: 0, device: 1, method: 1, value: ""});
		newRecurringMonthJob.addEvent(new Event({id: 0, value: 3, fuzzinessBefore: 0, fuzzinessAfter: 0, type: EVENTTYPE_ABSOLUTE, offset: 0, time: time1}));
		newRecurringMonthJob.save();
		
		var newAbsoluteJob = getJob({id: 5, name: "testnamn15", type: JOBTYPE_RECURRING_MONTH, startdate: startdate2, lastRun: 0, device: 1, method: 1, value: "", pastGracePeriod: 0});
		newAbsoluteJob.addEvent(new Event({id: 1, value: "00-05", fuzzinessBefore: 0, fuzzinessAfter: 0, type: EVENTTYPE_ABSOLUTE, offset: 0, time: time2}));
		newAbsoluteJob.addEvent(new Event({id: 2, value: "00-05", fuzzinessBefore: 0, fuzzinessAfter: 0, type: EVENTTYPE_ABSOLUTE, offset: 0, time: time3}));
		newAbsoluteJob.save();
				
		storedJobs = new MappedList();
		
		//get all jobs from permanent storage
		var settings = new com.telldus.settings();
		var storedJobsData = settings.value("jobs", "");
		
		for(var key in storedJobsData){
			var jobdata = storedJobsData[key];
			var job = getJob(jobdata);
			var tempkey = com.telldus.scheduler.addJob(job); //TODO, use different function than this = only sort list afterwards (one time) and dont start timer until all initial are added
		}	
	}
	
	function getJob(jobdata){
		//factory function... typ
		//TODO Även denna ska bort (iaf härifrån), för denna lagring/hämtning ska ske i respektive plugin eller gui sedan...
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
			default:
				break;
		}
		job.v = jobdata;
		return job;
	}
	
	function addJob(job){
		if(storedJobs.length == 0){
			print("Adding daylight saving time");
			var daylightSavingReloadKey = storedJobs.push(getDaylightSavingReloadJob());
			updateJobInList(daylightSavingReloadKey);
		}
		var key = storedJobs.push(job);
		print("Add job");
		updateJobInList(key);
		return key;
	}
	
	function updateJob(key, job){
		if(!storedJobs.contains(key)){
			return;
		}
		storedJobs.update(key, job);
		updateJobInList(key);
	}
	
	function removeJob(id){
		storedJobs.remove(id);
		updateJobInList(id);
		if(storedJobs.length == 1){
			//only one job left, it's only the DaylightSaving reload job, remove that too
			for(var key in storedJobs.container){
				storedJobs.remove(key);
				updateJobInList(key);
			}
		}
	}
	
	function updateJob(key, job){
		storedJobs.update(key, job);
		updateJobInList(key);
	}
	
	function getDaylightSavingReloadJob(){
		return new JobDaylightSavingReload();
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
		if((nextRunTime == 0 || currentEventRuntimeTimestamp < nextRunTime) && currentEventRuntimeTimestamp > (new Date().getTime() - pastGracePeriod)){   //earlier than other events, but later than "now"
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
			var rand =  Math.random(); //Random enough at the moment
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
	
	Job.prototype.execute = function(){
		//may be overridden if other than device manipulation should be performed
		var success = 0;
		print("Job id: " + this.v.id);
		id = this.v.id;
		var method = parseInt(this.method);
		switch(method){
			case com.telldus.core.TELLSTICK_TURNON:
				success = com.telldus.core.turnOn(id);
				break;
			case com.telldus.core.TELLSTICK_TURNOFF:
				success = com.telldus.core.turnOff(id);
				break;
			case com.telldus.core.TELLSTICK_DIM:
				success = com.telldus.core.dim(id, this.v.value);
				break;
			case com.telldus.core.TELLSTICK_BELL:
				success = com.telldus.core.bell(id);
				break;	
			default:
				break;
		}
		//if(success){
			//update last run even if not successful, else it may become an infinite loop (if using pastGracePeriod)
			com.telldus.scheduler.updateLastRun(id, new Date().getTime());
			//TODO update storage too
		//}
		return success;
	};
	
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
	
	//TODO move (or? maybe this could be here as default? maybe not, but keep the concept)
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
	
	function JobAbsolute(jobdata){
		
	}
	
	function JobRecurringDay(jobdata){
		
	}
	
	function JobRecurringWeek(jobdata){
		
	}
	
	function JobRecurringMonth(jobdata){
		
	}
	
	//keep this here
	function JobDaylightSavingReload(){
	}
	
	JobAbsolute.prototype = new Job(); 
	JobRecurringDay.prototype = new Job(); 
	JobRecurringWeek.prototype = new Job(); 
	JobRecurringMonth.prototype = new Job(); 
	JobDaylightSavingReload.prototype = new Job();
	
	JobDaylightSavingReload.prototype.getNextRunTime = function(){
		print("getNextRunTime DaylightSaving");
		var dst = DstDetect();
		var now = new Date().getTime();
		var time = dst[0].getTime();
		if(now > time){
			//already passed
			time = dst[1].getTime();
		}
		return time;
	}
	
	JobDaylightSavingReload.prototype.execute = function(){
		//override default
		print("Daylight savings job");
		//TODO Make sure this job is run "last", if other jobs are set to be runt the same second
		setTimeout(recalculateAllJobs(), 1); //sleep for one ms, to avoid strange calculations
		//this may lead to that things that should be executed exactly at 3.00 when
		//moving time forward one hour won't run, but that should be the only case
		return 0;
	};
	
	JobAbsolute.prototype.getNextRunTime = function(){
		print("getNextRunTime absolute");
		//Get all events in this job (absolute = 
		//kan vara flera absoluta datum och tidpunkter på ett jobb)
		//var events = job.events;
		var nextRunTime = 0;
			
		if(!this.v.events){
			return 0;
		}
		
		var pastGracePeriod = getGracePeriod(this.v);
		
		for(var key in this.v.events){
			nextRunTime = getNextEventRunTime(nextRunTime, this.v.events[key], parseInt(this.v.events[key].d.value), pastGracePeriod);
		}
		return nextRunTime;
	}
	
	JobRecurringDay.prototype.getNextRunTime = function(){
		print("getNextRunTime day");
		//Recurring day (every day, every other day or every x day)
		//only one event/job (at the moment at least)
		var nextRunTime = 0;
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
	
	JobRecurringWeek.prototype.getNextRunTime = function(){
		print("getNextRunTime week");
		var nextRunTime = 0;
		if(!this.v.events){
			return 0;
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
	
	JobRecurringMonth.prototype.getNextRunTime = function(){
		print("getNextRunTime month");
		var nextRunTime = 0;
		if(!this.v.events){
			return 0;
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
	
	function MappedList() {
		this.container = {};
		this.length = 0;
	}
	
	MappedList.prototype.push = function(element){
		//TODO reusing keys at the moment, that's ok, right?
		var length = this.length;
		this.container[length] = element;
		this.length = length + 1;
		return length;
	}
	
	MappedList.prototype.get = function(key){
		return this.container[key];
	}
	
	MappedList.prototype.update = function(key, element){
		this.container[key] = element;
	}
	
	MappedList.prototype.remove = function(key){
		delete this.container[key];
		this.length--;
	}
	
	MappedList.prototype.contains = function(key){
		return !(this.container[key] === undefined);
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
		addJob: addJob, //job, returns: storage id
		removeJob: removeJob, //storage id
		updateJob: updateJob, //storage id, job
		updateLastRun: updateLastRun, //id, datetimestamp
		//TODO getNextRunForJob? For all? (to see when job is due to run next)
		init:init //TODO change this
	}
}();

function compareTime(a, b) {
	return a.nextRunTime - b.nextRunTime;
}
