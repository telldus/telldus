__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.scheduler.init );
}

com.telldus.scheduler = function() {

	var JOBTYPE_ABSOLUTE = 0;
	var JOBTYPE_RECURRING_DAY = 1;
	var JOBTYPE_RECURRING_WEEK = 2;
	var JOBTYPE_RECURRING_MONTH = 3;
	var JOBTYPE_RELOAD = 4; //for summer/winter time, any better ideas?

	var EVENTTYPE_ABSOLUTE = 0;
	var EVENTTYPE_SUNRISE = 1;
	var EVENTTYPE_SUNSET = 2;
	
	
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
	//TODO ordna upp
	//TODO funktionerna, rätt placerade nu, eller ngn som eg. tillhör bara en subklass?
	//"repeat", t.ex. om villkor inte uppfylls så vill man göra ett nytt försök, även det får vara en funktion i extended scen/makro. if->false->tryAgainAfterXSeconds...
	
	// gränssnittet... hur...?
	//
	function init(){
		var joblist = loadJobs(); //TODO reload jobs on every job change
		if(joblist.length <= 0){
			return; //no jobs, abort
		}
		var nextRunTime = joblist[0].nextRunTime;
		
		//TODO:
		var runid = 1; //id som kommer tillbaka från c++ (timer)-delen
		var runJobFunc = function(){ runJob(joblist[0].id); };
		var now = new Date().getTime();
		print("Next: " + new Date(nextRunTime));
		var delay = nextRunTime - now;
		print("Delay: " + delay);
		setTimeout(runJobFunc, delay); //start the timer
	}
	
	function runJob(runid) {
		//should be the first one, but do a check
		print("RunJob: " + runid);
		var runJob = null;
		for(var i=0;i<joblist.length;i++){
			if(joblist[i].id == runid){
				runJob = joblist[i];
				break;
			}
		}
		var success = 0;
		if(runJob != null){
			switch(runJob.method){
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
			updateLastRun(runid, new Date().getTime());
		}
		loadJobs();
	};
	
	function saveJob(id){
		var job = new Job(id);
		//TODO set properties
		var settings = new com.telldus.settings();
		var storedJobs = settings.value("jobs", "");
		
		if(!storedJobs){
			storedJobs = {}; //initialize new
		}
		
		storedJobs[id] = job;
		
		settings.setValue("jobs", storedJobs);
	}
	
	function updateLastRun(id, lastRun){
		var settings = new com.telldus.settings();
		var storedJobs = settings.value("jobs", "");
		storedJobs[id].lastrun = lastRun;
		settings.setValue("jobs", storedJobs);
	}
	
	function loadJobs(){
		/*
		saveJob(1); //TODO, do this when saving something...
		saveJob(2);
		saveJob(3);
		updateLastRun(2, "2005-05-05");
		*/
		print("Loading jobs");
		
		var joblist = new Array();
		//get all jobs from permanent storage
		var settings = new com.telldus.settings();
		var storedJobs = settings.value("jobs", "");
		
		//for(var j=0;j<storedJobs.length;j++){
		for(var key in storedJobs){
			var job = storedJobs[key];
			print("LASTRUN: " + job.lastrun);
			print(key + ": " + job.lastrun);
			//var nextRunTime = job.getNextRunTime();
			var nextRunTime = 0;
			//print("Runtime for job: " + new Date(nextRunTime));
			
			joblist.push(new RunJob(job.id, nextRunTime, job.type, job.device, job.method, job.value));
		}
			
		joblist.sort(compareTime);
		
		return joblist;
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
			nextRunTime = currentEventRuntimeTimestamp;
		}
		
		return nextRunTime;
	}
	
	function getEventRunTime(event, date){
		var currentEventRuntimeTimestamp = 0;
		if(event.type == EVENTTYPE_ABSOLUTE){
			currentEventRuntimeTimestamp = event.time + date;
			currentEventRuntimeTimestamp = fuzzify(currentEventRuntimeTimestamp, event.fuzzinessBefore, event.fuzzinessAfter);
		}
		else if(event.type == EVENTTYPE_SUNRISE || event.type == EVENTTYPE_SUNSET){
			print("NEW DATE: " + new Date(date));
			currentEventRuntimeTimestamp = getSunUpDownForDate(date, event.type);
			print("NEW DATE: " + new Date(currentEventRuntimeTimestamp));
			currentEventRuntimeTimestamp += (event.offset * 1000);
			currentEventRuntimeTimestamp = fuzzify(currentEventRuntimeTimestamp, event.fuzzinessBefore, event.fuzzinessAfter);		
		}
		
		return currentEventRuntimeTimestamp;
	}
	
	function fuzzify(currentTimestamp, fuzzinessBefore, fuzzinessAfter){
		if(fuzzinessAfter != 0 || fuzzinessBefore != 0){
			var interval = fuzzinessAfter + fuzzinessBefore;
			var rand =  Math.random(); //TODO random enough?
			var fuzziness = Math.floor((interval+1) * rand);
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
		print("Formatted date: " + date);
		return date.getTime();
	
	}
	
	function Job(id) {
		//hämta detta
		//one job can have several events... more occurencies...
		this.id = 1;
		this.name = "testnamn";
		this.type = JOBTYPE_RECURRING_MONTH;
		this.startdate = "2010-01-01"; //TODO enforce:a där det är relevant. Format? Gör om till timestamp. Obs, används inte ännu, om det ska användas, se till att kolla detta också överallt
		this.lastrun = 0;
		this.device = 1;
		this.method = 1;
		this.value = "";
		
	}
	
	Job.prototype.getNextRunTime = function(){
		return 0; //default
	}
	
	function JobAbsolute(id){
		this.Job(id);
		this.type = JOBTYPE_ABSOLUTE; //TODO, remove this, shouldnt be neccessary later on
		var events = new Array();
		//foreach stored event...
		events.push(new Event(1)); //TODO
		//events.push(new Event(2));
		this.events = events;
	}
	
	function JobRecurringDay(id){
		this.Job(id);
		this.type = JOBTYPE_RECURRING_DAY;
		this.event = new Event(1); //TODO id
	}
	
	function JobRecurringWeek(id){
		this.Job(id);
		this.type = JOBTYPE_RECURRING_WEEK;
		var events = new Array();
		//foreach stored event...
		events.push(new Event(1)); //TODO
		//events.push(new Event(2));
		this.events = events;
	}
	
	function JobRecurringMonth(id){
		this.Job(id);
		this.type = JOBTYPE_RECURRING_MONTH;
		var events = new Array();
		//foreach stored event...
		events.push(new Event(1)); //TODO
		//events.push(new Event(2));
		this.events = events;
		//this.time = 30000; hm, using events instead for now
	}
	
	JobAbsolute.prototype = Job.prototype;
	JobRecurringDay.prototype = Job.prototype;
	JobRecurringWeek.prototype = Job.prototype;
	JobRecurringMonth.prototype = Job.prototype;
	
	JobAbsolute.prototype.getNextRunTime = function(){
		//TODO like this, or more like this.getNextRunTime = function(name){  inside the class?
		//Get all events in this job (absolute = 
		//kan vara flera absoluta datum och tidpunkter på ett jobb)
		//var events = job.events;
		var nextRunTime = 0;
		for(var i=0;i<this.events.length;i++){
			nextRunTime = getNextEventRunTime(nextRunTime, this.events[i], this.events[i].value);
		}
		return nextRunTime;
	}
	
	JobRecurringDay.prototype.getNextRunTime = function(){
		//Recurring day (every day, every other day or every x day)
		//only one event/job (at the moment at least)
		//TODO test this
		var nextRunTime = 0;
		var date;
			
		if(this.lastRun > 0){
			var lastRunDate = new Date(this.lastRun);	
			date = new Date(lastRunDate.getFullYear(), lastRunDate.getMonth(), lastRunDate.getDate());
			date = date.getTime() + this.event.value;   //add interval
		}
		else{
			var now = new Date(); //Now
			date = new Date(now.getFullYear(), now.getMonth(), now.getDate());
		}
		
		nextRunTime = getNextEventRunTime(0, this.event, date);
		if(nextRunTime < new Date().getTime()){
			var runTime = new Date(date);
			runTime.setDate(runTime.getDate() + this.event.value);
			nextRunTime = getNextEventRunTime(0, this.event, runTime.getTime()); //already passed this time today, try again after "interval" days 
		}
		return nextRunTime;
	}
	
	JobRecurringWeek.prototype.getNextRunTime = function(){
		var nextRunTime = 0;
		for(var i=0;i<this.events.length;i++){
			//plocka fram nästa veckodag med nummer x, kan vara idag också
			var returnDate = new Date();
			var returnDay = returnDate.getDay();
			if (this.events[i].value !== returnDay) {
				returnDate.setDate(returnDate.getDate() + (this.events[i].value + (7 - returnDay)) % 7);
			}
			
			returnDate = zeroTime(returnDate);
			
			nextTempRunTime = getNextEventRunTime(0, this.events[i], returnDate.getTime());
			if(nextTempRunTime < new Date().getTime()){ //event happened today, already passed, add to next week instead
				returnDate.setDate(returnDate.getDate() + 7);
				nextRunTime = getNextEventRunTime(nextRunTime, this.events[i], returnDate.getTime());	
			}
			else{
				nextRunTime = nextTempRunTime;
			}
		}
		return nextRunTime;
	}
	
	JobRecurringMonth.prototype.getNextRunTime = function(){
		//TODO test this
		var nextRunTime = 0;
		
		for(var i=0;i<this.events.length;i++){
			//plocka fram nästa månadsdag med nummer x, kan vara idag också, i aktuell månad...
			
			var daymonth = this.events[i].value.split('-'); //month-day
			var month = daymonth[0];
			var day = daymonth[1];
			var now = new Date();
			
			if((32 - new Date(now.getFullYear(), month, 32).getDate()) < day){ //check that day exists for this month
				if(month != 1 && day != 29){
					break; //this day doesn't exist for this month (at least not this year (leap year...))
				}
			}
			
			print("NEXTDATE: " + month);
			var nextdate = getNextLeapYearSafeDate(now.getFullYear(), month, day, this.events[i]);
			print("NEXTDATE: " + new Date(nextdate));
			
			if(nextdate < new Date().getTime()){ //event already happened this year, add to next year instead
				var tempdate = new Date(nextdate);
				tempdate.setYear(now.getFullYear() + 1);
				print(tempdate);
				nextdate = tempdate.getTime();
			}
			if(nextRunTime == 0 || nextRunTime > nextdate){
				nextRunTime = nextdate;
			}
		}
		return nextRunTime;
	}
	
	
	function Event(id){
		this.id = id;
		if(id == 1){
			this.value = "11-21"; //day of week, day of month, day interval or specific date... Individual values for each type instead?
		}
		else{
			this.value = new Date().getTime(); //day of week, day of month, day interval or specific date...	
		}
		this.fuzzinessBefore = 0;
		this.fuzzinessAfter = 0;
		this.type = EVENTTYPE_SUNSET;
		this.offset = 0;
		this.time = 0; //"" since using sunset... Timestamp
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
		jobList: loadJobs,
		init:init
	}
}();

function compareTime(a, b) {
	return a.nextRunTime - b.nextRunTime;
}
