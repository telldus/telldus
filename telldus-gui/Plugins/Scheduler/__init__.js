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
	//"repeat", t.ex. om villkor inte uppfylls så vill man göra ett nytt försök, även det får vara en funktion i extended scen/makro. if->false->tryAgainAfterXSeconds...
	
	// gränssnittet... hur...?
	//
	function init(){
		var joblist = loadJobs(); //reload jobs on every job change
		//skicka första tidpunkten (+ id för säkerhetsskull?) till c++ som måste ha hand om timern (verkar det som)
		//hur den biten går till, hur ska kommunikationen fram och tillbaka (callback? vänta?) fungera? Tråden måste kunna
		//avbrytas om jobblistan uppdateras eller om programmet stängs av...
		var nextRunTime = joblist[0].nextRunTime;
		
		var runid = 1; //id som kommer tillbaka från c++ (timer)-delen
		var runJobFunc = function(){ runJob(joblist[0].id); };
		var now = new Date().getTime();
		print("Next: " + new Date(nextRunTime));
		var delay = nextRunTime - now;
		print("Delay: " + delay);
		setTimeout(runJobFunc, delay);
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
		if(runJob != null){
			switch(runJob.method){
				case com.telldus.core.TELLSTICK_TURNON:
					com.telldus.core.turnOn(runJob.id);
					break;
				case com.telldus.core.TELLSTICK_TURNOFF:
					com.telldus.core.turnOff(runJob.id);
					break;
				case com.telldus.core.TELLSTICK_DIM:
					com.telldus.core.dim(runJob.id, runJob.value);
					break;
				case com.telldus.core.TELLSTICK_BELL:
					com.telldus.core.bell(runJob.id);
					break;	
				default:
					break;
			}
		}
		//TODO set som last run (i lagringen)
		loadJobs();
	};
	
	function loadJobs(){
		print("Loading jobs");
		var joblist = new Array();
		//get all jobs from permanent storage
		//Hur hämta!?!?! (eller snarare, var spara?)
		var job1 = new Job(1);
		var job2 = new Job(2);
		var storedJobs = new Array();
		storedJobs.push(job1); //TODO
		storedJobs.push(job2);
		
		for(var j=0;j<storedJobs.length;j++){
			var job = storedJobs[j];
			var nextRunTime = 0;
			if(job.type == JOBTYPE_ABSOLUTE){
				//hämta alla events i detta jobb (absolut = kan vara flera absoluta datum och tidpunkter på ett jobb)
				var events = job.events;
				for(var i=0;i<events.length;i++){
					
					nextRunTime = getNextEventRunTime(nextRunTime, events[i], events[i].value);
				}
			}
			else if(job.type == JOBTYPE_RECURRING_DAY){
				//varje dag eller varannan dag eller så...
				//känns som att max ett event är aktuellt här (kör detta jobb en ggn varje/varannan/var x:e dag alltså)
				//kan väl ändras i framtiden annars
				//TODO test this
				var events = job.events;
				if(events.length > 0){
					var event = events[0];
					var lastRun = job.lastRun;
					var date;
					
					if(lastRun > 0){
						var lastRunDate = new Date(lastRun);	
						date = new Date(lastRunDate.getFullYear(), lastRunDate.getMonth(), lastRunDate.getDate());
						date = date.getTime() + event.value;   //add interval
					}
					else{
						var now = new Date(); //Now
						date = new Date(now.getFullYear(), now.getMonth(), now.getDate());
					}
					
					nextRunTime = getNextEventRunTime(0, event, date);
					if(nextRunTime < new Date().getTime()){
						var runTime = new Date(date);
						runTime.setDate(runTime.getDate() + event.value);
						nextRunTime = getNextEventRunTime(0, event, runTime.getTime()); //already passed this time today, try again after "interval" days 
					}
				}
			}
			else if(job.type == JOBTYPE_RECURRING_WEEK){
				//TODO test this
				var events = job.events;
				for(var i=0;i<events.length;i++){
					//plocka fram nästa veckodag med nummer x, kan vara idag också
					var returnDate = new Date();
					var returnDay = returnDate.getDay();
					if (events[i].value !== returnDay) {
						returnDate.setDate(returnDate.getDate() + (events[i].value + (7 - returnDay)) % 7);
					}
					
					returnDate = zeroTime(returnDate);
					
					nextTempRunTime = getNextEventRunTime(0, events[i], returnDate.getTime());
					if(nextTempRunTime < new Date().getTime()){ //event happened today, already passed, add to next week instead
						returnDate.setDate(returnDate.getDate() + 7);
						nextRunTime = getNextEventRunTime(nextRunTime, events[i], returnDate.getTime());	
					}
					else{
						nextRunTime = nextTempRunTime;
					}
				}
			}
			else if(job.type == JOBTYPE_RECURRING_MONTH){
				//TODO test this
				var events = job.events;
				var time;
				if(events.length > 0){
					time = getEventRunTime(events[0], new Date(0)); //get time-timestamp only, same for every day
				}
				for(var i=0;i<events.length;i++){
					//plocka fram nästa månadsdag med nummer x, kan vara idag också, i aktuell månad...
					
					var daymonth = events[i].value.split('-'); //month-day
					var month = daymonth[0];
					var day = daymonth[1];
					var now = new Date();
					print("Month: " + month + " Day: " + day + " Year: " + now);
					
					if((32 - new Date(now.getFullYear(), month, 32).getDate()) < day){
						if(month != 1 && day != 29){
							break; //this day doesn't exist for this month (at least not this year (leap year...))
						}
					}
					
					var nextdate = getNextLeapYearSafeDate(now.getFullYear(), month, day, time);
					
					print("DATE: " + new Date(nextdate));
					
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
			}
			
			print("Runtime for job: " + new Date(nextRunTime));
			
			joblist.push(new RunJob(job.id, nextRunTime, job.type, job.device, job.method, job.value));
		}
			
		joblist.sort(compareTime);
		
		return joblist;
	}
	
	function getNextLeapYearSafeDate(year, month, day, time){
		if(month == 1 && day == 29){
			//get leap year
			for(var i=0;i<5;i++){
				if(new Date(year+i,1,29).getDate() == 29){
					//this is a leap year
					datetimestamp = new Date(year+1, 1, 29).getTime() + time; //add time of day
					if(datetimestamp > new Date().getTime()){ //else, this was a leap year, but already passed for this year
						return datetimestamp;
					}
				}
			}
		}
		else{
			print("TIME: " + time);
			return new Date(year, month, day).getTime() + time; //add time of day
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
			currentEventRuntimeTimestamp = getSunUpDownForDate(date, event.type, 55.7, 13.1833); //TODO Long/lat
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
	
	function getSunUpDownForDate(datetimestamp, sun, long, lat){
	
		date = new Date(datetimestamp);
		var timevalues = com.telldus.suncalculator.riseset(date, long, lat);
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
	
	function Job(id) {
		//hämta detta
		//one job can have several events... more occurencies...
		this.id = 1;
		this.name = "testnamn";
		this.type = JOBTYPE_RECURRING_MONTH;
		this.startdate = "2010-01-01"; //format? Gör om till timestamp. Obs, används inte ännu, om det ska användas, se till att kolla detta också överallt
		this.lastrun = 0;
		this.device = 1;
		this.method = 1;
		this.value = "";
		var events = new Array();
		//foreach stored event...
		events.push(new Event(1));
		//events.push(new Event(2));
		this.events =  events;
	}

	function Event(id){
		this.id = id;
		if(id == 1){
			this.value = "05-10"; //day of week, day of month, day interval or specific date... Individual values for each type instead?
		}
		else{
			this.value = "12-12"; //day of week, day of month, day interval or specific date...	
		}
		this.fuzzinessBefore = 2;
		this.fuzzinessAfter = 0;
		this.type = EVENTTYPE_SUNSET;
		this.offset = -7000;
		this.time = 300000; //"" since using sunset... Timestamp
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

