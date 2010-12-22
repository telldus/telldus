__setupPackage__( __extension__ );

com.telldus.suncalculator = function() { 

	//sunrise/sunset calculator, derived from the work of Stephen R. Schmitt
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

	// calculate sunrise and sunset times
	function riseset(date)
	{
		var settings = new com.telldus.settings();
		var lat = 56.1488; //55.6893; //default
		var lon = 13.3538; //13.2123; //default
		lat = settings.value("latitude", lat);
		lon = settings.value("longitude", lon);
		
		lat = 55 + 68.93/60.0;
		lon = 13 + 21.23/60.0;
		
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
	
	function setLocation(longitude, latitude){
		settings.setValue("longitude", longitude);
		settings.setValue("latitude", latitude);	
	}
	
	return { //Public functions
		riseset: riseset,
		setLocation: setLocation
	}
}();