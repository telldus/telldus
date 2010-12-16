__setupPackage__( __extension__ );

com.telldus.scheduler = function() {

	var JOBTYPE_ABSOLUTE = 0;
	
	var EVENTTYPE_ABSOLUTE = 0;
	var EVENTTYPE_SUNRISE = 1;
	var EVENTTYPE_SUNSET = 2;
	
	//SUNRISE
	
		var PI = Math.PI;
	
var DR = PI/180;
var K1 = 15*DR*1.0027379

var Sunrise = false;
var Sunset  = false;

var Rise_time = [0, 0];
var Set_time  = [0, 0];
var Rise_az = 0.0;
var Set_az  = 0.0;

var Sky = [0.0, 0.0];
var RAn = [0.0, 0.0, 0.0];
var Dec = [0.0, 0.0, 0.0];
var VHz = [0.0, 0.0, 0.0];
	
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
	var joblist = loadJobs(); //reload jobs on every job change
	//skicka första tidpunkten (+ id för säkerhetsskull?) till c++ som måste ha hand om timern (verkar det som)
	//hur den biten går till, hur ska kommunikationen fram och tillbaka (callback? vänta?) fungera? Tråden måste kunna
	//avbrytas om jobblistan uppdateras eller om programmet stängs av...
	var nextRunTime = joblist[0].nextRunTime;
	print("Next: " + nextRunTime);
	
	var runid = 1; //id som kommer tillbaka från c++ (timer)-delen
	var runJobFunc = function(){ runJob(joblist[0].id); };
	print("Hm");
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
	print("Ham");
	var now = new Date().getTime();
	print ("Now:" + new Date(now));
	print("Then:" + new Date(nextRunTime));
	var delay = nextRunTime - now;
	if(delay < 0){
		delay = 0;
	}
	print("Delay: " + delay);
	setTimeout(runJobFunc, delay);
	
	function addTime() {
	}
	
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
		var timevalues = riseset(date, long, lat);
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
	
	
	//MOVE
	//----------------
	//-------------------
	
	//sunrise/sunset calculator by Stephen R. Schmitt
//moved up:
/*
var PI = Math.PI;
	
var DR = PI/180;
var K1 = 15*DR*1.0027379

var Sunrise = false;
var Sunset  = false;

var Rise_time = [0, 0];
var Set_time  = [0, 0];
var Rise_az = 0.0;
var Set_az  = 0.0;

var Sky = [0.0, 0.0];
var RAn = [0.0, 0.0, 0.0];
var Dec = [0.0, 0.0, 0.0];
var VHz = [0.0, 0.0, 0.0];
*/

// calculate sunrise and sunset times
function riseset(date, lat, lon )
{
	var k;
    var zone = Math.round(date.getTimezoneOffset()/60);
    var jd = julian_day(date) - 2451545;           // Julian day relative to Jan 1.5, 2000
    
    if ((sgn(zone) == sgn(lon))&&(zone != 0))
        print("WARNING: time zone and longitude are incompatible!");
    
    lon = lon/360;
    var tz  = zone/24;
    var ct  = jd/36525 + 1;                    // centuries since 1900.0
    var t0 = lst(lon, jd, tz);                 // local sidereal time

    jd = jd + tz;                              // get sun position at start of day
    sun(jd, ct);
    var ra0  = Sky[0];
    var dec0 = Sky[1];

    jd = jd + 1;                               // get sun position at end of day
    sun(jd, ct);
    var ra1  = Sky[0];
    var dec1 = Sky[1];

    if (ra1 < ra0)                             // make continuous 
        ra1 = ra1 + 2*PI;

    Sunrise = false;                           // initialize
    Sunset  = false;
    RAn[0]   = ra0;
    Dec[0]  = dec0;
    
    for (k = 0; k < 24; k++)                   // check each hour of this day
    {
        ph = (k + 1)/24;
        
        RAn[2] = ra0  + (k + 1)*(ra1  - ra0)/24;
        Dec[2] = dec0 + (k + 1)*(dec1 - dec0)/24;
        VHz[2] = test_hour(k, zone, t0, lat);
        
        RAn[0] = RAn[2];                       // advance to next hour
        Dec[0] = Dec[2];
        VHz[0] = VHz[2];
    }

    // display results
    var sunrisevalue = zintstr(Rise_time[0], 2) + ":" + zintstr(Rise_time[1], 2);
    var sunsetvalue  = zintstr( Set_time[0], 2) + ":" + zintstr( Set_time[1], 2);
    
	var message = special_message();
	
	var values = new Array();
	values.push(sunrisevalue);
	values.push(sunsetvalue);
	values.push(message);
	return values;
}

// Local Sidereal Time for zone
function lst( lon, jd, z )
{
    var s = 24110.5 + 8640184.812999999*jd/36525 + 86636.6*z + 86400*lon;
    s = s/86400;
    s = s - Math.floor(s);
    return s*360*DR;
}

// test an hour for an event
function test_hour( k, zone, t0, lat )
{
    var ha = new Array(3);
    var a, b, c, d, e, s, z;
    var hr, min, time;
    var az, dz, hz, nz;
    
    ha[0] = t0 - RAn[0] + k*K1; 
    ha[2] = t0 - RAn[2] + k*K1 + K1; 

    ha[1]  = (ha[2]  + ha[0])/2;               // hour angle at half hour
    Dec[1] = (Dec[2] + Dec[0])/2 ;             // declination at half hour
    
    s = Math.sin(lat*DR);
    c = Math.cos(lat*DR);
    z = Math.cos(90.833*DR);                   // refraction + sun semidiameter at horizon

    if (k <= 0)
        VHz[0] = s*Math.sin(Dec[0]) + c*Math.cos(Dec[0])*Math.cos(ha[0]) - z;

    VHz[2] = s*Math.sin(Dec[2]) + c*Math.cos(Dec[2])*Math.cos(ha[2]) - z;
    
    if (sgn(VHz[0]) == sgn(VHz[2])) 
        return VHz[2];                         // no event this hour
    
    VHz[1] = s*Math.sin(Dec[1]) + c*Math.cos(Dec[1])*Math.cos(ha[1]) - z;
    
    a =  2* VHz[0] - 4*VHz[1] + 2*VHz[2]; 
    b = -3* VHz[0] + 4*VHz[1] - VHz[2];   
    d = b*b - 4*a*VHz[0];

    if (d < 0) 
        return VHz[2];                         // no event this hour
    
    d = Math.sqrt(d);    
    e = (-b + d)/(2 * a);
    
    if ((e > 1)||(e < 0))
        e = (-b - d)/(2*a);

    time = k + e + 1/120;                      // time of an event
    
    hr = Math.floor(time);
    min = Math.floor((time - hr)*60);

    hz = ha[0] + e*(ha[2] - ha[0]);            // azimuth of the sun at the event
    nz = -Math.cos(Dec[1])*Math.sin(hz);
    dz = c*Math.sin(Dec[1]) - s*Math.cos(Dec[1])*Math.cos(hz);
    az = Math.atan2(nz, dz)/DR;
    if (az < 0) az = az + 360;
    
    if ((VHz[0] < 0)&&(VHz[2] > 0))
    {
        Rise_time[0] = hr;
        Rise_time[1] = min;
        Rise_az = az;
        Sunrise = true;
    }
    
    if ((VHz[0] > 0)&&(VHz[2] < 0))
    {
        Set_time[0] = hr;
        Set_time[1] = min;
        Set_az = az;
        Sunset = true;
    }

    return VHz[2];
}

// check for no sunrise and/or no sunset
function special_message()
{
    if ((!Sunrise)&&(!Sunset))                 // neither sunrise nor sunset
    {
        if (VHz[2] < 0){
            return "Sun down all day";
		}
        else{
            return "Sun up all day";
		}

        return "";
    }
    else                                       // sunrise or sunset
    {
        if (!Sunrise)
            return "No sunrise this date";
        else if (!Sunset)
            return "No sunset this date";
    }
}

// sun's position using fundamental arguments 
// (Van Flandern & Pulkkinen, 1979)
function sun( jd, ct )
{
	jd = 4001.4583333333335;
	ct = 1.109555099247091;
    var g, lo, s, u, v, w;
    
    lo = 0.779072 + 0.00273790931*jd;
    lo = lo - Math.floor(lo);
    lo = lo*2*PI;
	
    g = 0.993126 + 0.0027377785*jd;
    g = g - Math.floor(g);
    g = g*2*PI;
    
    v = 0.39785*Math.sin(lo);
    v = v - 0.01*Math.sin(lo - g);
    v = v + 0.00333*Math.sin(lo + g);
    v = v - 0.00021*ct * Math.sin(lo);
    
    u = 1 - 0.03349*Math.cos(g);
    u = u - 0.00014*Math.cos(2*lo);
    u = u + 0.00008*Math.cos(lo);
    
    w = -0.0001 - 0.04129*Math.sin(2*lo);
    w = w + 0.03211*Math.sin(g );
    w = w + 0.00104*Math.sin(2*lo - g);
    w = w - 0.00035*Math.sin(2*lo + g);
    w = w - 0.00008*ct*Math.sin(g);
    
    s = w/Math.sqrt(u - v*v);                  // compute sun's right ascension  
	Sky[0] = lo + Math.atan(s/Math.sqrt(1 - s*s));
	
    s = v/Math.sqrt(u);                        // ...and declination 
    Sky[1] = Math.atan(s/Math.sqrt(1 - s*s));
}

// determine Julian day from calendar date
// (Jean Meeus, "Astronomical Algorithms", Willmann-Bell, 1991)
function julian_day(date)
{
    var a, b, jd;
    var gregorian;

    var month = date.getMonth() + 1;
    var day   = date.getDate();
    var year  = date.getFullYear();

    gregorian = (year < 1583) ? false : true;
    
    if ((month == 1)||(month == 2))
    {
        year  = year  - 1;
        month = month + 12;
    }

    a = Math.floor(year/100);
    if (gregorian) b = 2 - a + Math.floor(a/4);
    else           b = 0.0;

    jd = Math.floor(365.25*(year + 4716)) 
       + Math.floor(30.6001*(month + 1)) 
       + day + b - 1524.5;
    
    return jd;
}

// returns value for sign of argument
function sgn( x )
{
    var rv;
    if (x > 0.0)      rv =  1;
    else if (x < 0.0) rv = -1;
    else              rv =  0;
    return rv;
}

// format a positive integer with leading zeroes
function zintstr( num, width )
{
    var str = num.toString(10);
    var len = str.length;
    var intgr = "";
    var i;

    for (i = 0; i < width - len; i++)          // append leading zeroes
        intgr += '0';

    for (i = 0; i < len; i++)                  // append digits
        intgr += str.charAt(i);

    return intgr;
}

// format an integer
function cintstr( num, width )
{
    var str = num.toString(10);
    var len = str.length;
    var intgr = "";
    var i;

    for (i = 0; i < width - len; i++)          // append leading spaces
        intgr += ' ';

    for (i = 0; i < len; i++)                  // append digits
        intgr += str.charAt(i);

    return intgr;
}

// format a real number
function frealstr( num, width, fract )
{
    var str = num.toFixed(fract);
    var len = str.length;
    var real = "";
    var i;

    for (i = 0; i < width - len; i++)          // append leading spaces
        real += ' ';

    for (i = 0; i < len; i++)                  // append digits
        real += str.charAt(i);

    return real;
}



//-----------------------------
	
	

	return { //Public functions
		addTime: addTime,
		jobList: loadJobs
	}
}();

function compareTime(a, b) {
	return a.nextRunTime - b.nextRunTime;
}

__postInit__ = function() {
	print("Hello from scheduler");
}

