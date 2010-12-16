__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.scheduler.init );
}

com.telldus.scheduler = function() {

	var JOBTYPE_ABSOLUTE = 0;
	
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
		print ("Now:" + new Date(now));
		print("Then:" + new Date(nextRunTime));
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
				case 1:
					//TODO: get constants from somewhere
					com.telldus.core.turnOn(runJob.id);
					break;
				case 2:
					com.telldus.core.turnOff(runJob.id);
					break;
				default:
					break;
			}
		}	
	};
	
	function loadJobs(){
		print("Loading jobs");
		var joblist = new Array();
		//get all jobs from permanent storage
		//Hur hämta!?!?! (eller snarare, var spara?)
		var job1 = new Job(1);
		var job2 = new Job(2);
		//foreach job:
		var nextRunTime = 0;
		if(job1.type == JOBTYPE_ABSOLUTE){
			//hämta alla events i detta jobb
			//kan man bryta ut och återanvända en del kanske?
			//fuzzinessiseringen kan bli en metod iaf...
			var events = job1.events;
			for(var i=0;i<events.length;i++){
				
				var currentEventRuntimeTimestamp = 0;
				if(events[i].type == EVENTTYPE_ABSOLUTE){
					//var fuzzinessinterval = events[i].fuzzinessBefore + events[i].fuzzinessAfter;
					currentEventRuntimeTimestamp = events[i].time;
					if(events[i].fuzzinessAfter != 0 || events[i].fuzzinessBefore != 0){
						var fuzziness = Math.floor((events[i].fuzzinessAfter - events[i].fuzzinessBefore)*Math.random()) + events[i].fuzzinessBefore + 1; //TODO check when result is a negative value
						currentEventRuntimeTimestamp += (fuzziness * 1000);
					}
				}
				else if(events[i].type == EVENTTYPE_SUNRISE || events[i].type == EVENTTYPE_SUNSET){
					
					currentEventRuntimeTimestamp = getSunUpDownForDate(events[i].value, events[i].type, 55.7, 13.1833); //TODO Long/lat
					print("Timestampet: " + currentEventRuntimeTimestamp);
					currentEventRuntimeTimestamp += (events[i].offset * 1000);
					print("Timestampet: " + currentEventRuntimeTimestamp);
					
					if(events[i].fuzzinessAfter != 0 || events[i].fuzzinessBefore != 0){
						var fuzziness = Math.floor((events[i].fuzzinessAfter - events[i].fuzzinessBefore)*Math.random()) + events[i].fuzzinessBefore + 1; //TODO check when result is a negative value
						currentEventRuntimeTimestamp += (fuzziness * 1000);
					}
					print("Timestampet: " + currentEventRuntimeTimestamp);
				}
				
				if(nextRunTime == 0 || currentEventRuntimeTimestamp < nextRunTime){
					nextRunTime = currentEventRuntimeTimestamp;
				}
			}
		}
		
		//nextRunTime = nextRunTime.setDate('2010-01-01 23:59:00');
		//nextRunTime = new Date("July 27, 2010 23:59:00");
		//nextRunTime = nextRunTime.getTime();
		
		var nextRunTime1 = new Date("July 27, 2012 23:59:00");
		nextRunTime1 = nextRunTime1.getTime();
		var nextRunTime2 = new Date("July 27, 2011 23:59:00");
		nextRunTime2 = nextRunTime2.getTime();
		
		print("NEXTRUNTIME: " + nextRunTime);
		
		//listan ska vara ordnad
		joblist.push(new RunJob(job1.id, nextRunTime, job1.device, job1.method, job1.value));
		joblist.push(new RunJob(2, nextRunTime1, job1.device, job1.method, job1.value));
		joblist.push(new RunJob(3, nextRunTime2, job1.device, job1.method, job1.value));
		
		for(var i=0;i<joblist.length;i++){
			print(joblist[i].id + ": " + joblist[i].nextRunTime);
		}
		print("after sort");
		joblist.sort(compareTime);
		for(var i=0;i<joblist.length;i++){
			print(joblist[i].id + ": " + joblist[i].nextRunTime);
		}
		
		return joblist;
	}
	
	function getSunUpDownForDate(datetimestamp, sun, long, lat){
	
		date = new Date(datetimestamp);
		//date = "Thu Dec 16 2010 12:57:47 GMT+0100 (CET)";
		print("Date: " + date);
		//print(new Date(date));
		print(com.telldus.suncalculator);
		var timevalues = com.telldus.suncalculator.riseset(date, long, lat);
		if(timevalues[2] && timevalues[2] != ""){
			return ""; //no sun up or down, do nothing
		}
		print("Time values: " + timevalues[0]);
		var hourminute;
		if(sun == EVENTTYPE_SUNRISE){
			hourminute = timevalues[0].split(':');
		}
		else{
			hourminute = timevalues[1].split(':');
		}
		date.setHours(hourminute[0]);
		date.setMinutes(hourminute[1]);
		print("Date after:" + date);
		return date.getTime();
	
	}
	
	function Job(id) {
		//hämta detta
		//one job can have several events... more occurencies...
		this.id = 1;
		this.name = "testnamn";
		this.type = JOBTYPE_ABSOLUTE;
		this.startdate = "2010-01-01"; //format?
		this.lastrun = "";
		this.device = 1;
		this.method = 1;
		this.value = "";
		var events = new Array();
		//foreach stored event...
		events.push(new Event());
		this.events =  events;
	}

	function Event(id){
		this.id = 1;
		this.value = new Date("December 16, 2010").getTime(); //day of week, day of month, day interval or specific date...
		this.fuzzinessBefore = 60;
		this.fuzzinessAfter = 60;
		this.type = EVENTTYPE_SUNSET;
		this.offset = -800;
		this.time = ""; //"" since using sunset...
	}

	function RunJob(id, nextRunTime, device, method, value){
		print (nextRunTime);
		this.id = id;
		this.nextRunTime = nextRunTime;
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

