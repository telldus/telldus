var calculatedSunData;

function getEveningDarkStart(){
	if(!willSunSet()){
		return 0;
	}
	if(isMidnightDark()){
		var sunData = getSunData();
		return sunToTimeUnits(sunData[1]);
	}
	else{
		return 0;
	}
}

function getEveningDarkWidth(){
	if(!willSunSet()){
		return 0;
	}
	if(isMidnightDark()){
		var sunData = getSunData();
		return constDeviceRowWidth - sunToTimeUnits(sunData[1]);
	}
	else{
		return 0;
	}
}

function getMorningDarkStart(){
	//TODO the day of the year when the sun "begins" not to set, will it work then?
	if(!willSunSet()){
		return 0;
	}
	if(isMidnightDark()){
		return 0;
	}
	else{
		var sunData = getSunData();
		var start = sunToTimeUnits(sunData[1]);
		if(start == undefined){
			return 0;
		}
		return start;
	}
}

function getMorningDarkWidth(){
	if(!willSunSet()){
		return 0;
	}
	var sunData = getSunData();
	if(isMidnightDark()){
		return sunToTimeUnits(sunData[0]);
	}
	else{
		return sunToTimeUnits(sunData[1]) - sunToTimeUnits(sunData[0]);
	}
}

function getSunData(){
	if(calculatedSunData == undefined){
		main.updateCurrentDay();
	}
	if(calculatedSunData[0] == undefined){
		return ["0:0","0:0","0:0"]; //TODO
	}
	return calculatedSunData;
}

function updateSunData(sunData){
	calculatedSunData = sunData;
}

function isMidnightDark(){
	var sunData = getSunData();
	
	var sunrise = sunData[0].split(':');;
	var sunset = sunData[1].split(':');
	
	if(sunset[0] < sunrise[0] || (sunset[0] == sunrise[0] && sunset[1] < sunrise[1])){
		return false;
	}
	return true;
}

function pointToArray(point){ //TODO another way than using arrays...
	var deviceId = point.deviceRow.deviceId;
	var pointName = "Job_" + deviceId;
	var startdate = new Date(); //startdate, not in use, always "now"
	var pointDimValue = point.dimvalue * (255/100);
	var pointMethod = main.mainGetMethodFromState(point.state);
	var pointId = point.pointId;
	var lastRun = point.lastRun;
	
	var pointTime = point.absoluteHour * 3600 + point.absoluteMinute * 60;
	var absolutePointTime = pointTime;
	var pointType = main.mainGetTypeFromTriggerstate(point.triggerstate);
	var sunData = getSunData();
	if(point.triggerstate == "sunrise"){
		var suntime = sunData[0].split(':');
		pointTime = suntime[0] * 3600 + suntime[1] * 60;
	}
	else if(point.triggerstate == "sunset"){
		var suntime = sunData[1].split(':');
		pointTime = suntime[0] * 3600 + suntime[1] * 60;
	}
	var pointFuzzinessBefore = point.fuzzyBefore;
	var pointFuzzinessAfter = point.fuzzyAfter;
	var pointOffset = point.triggerstate == "absolute" ? 0 : point.offset;
	
	var pointDays = new Array();
	pointDays.push(point.deviceRow.parent.parent.daydate.getDay());
	var childPoints = point.getChildPoints();
	for(var child in childPoints){
		pointDays.push(childPoints[child].deviceRow.parent.parent.daydate.getDay()); //value (day) different per event
	}
	return new Array(deviceId, pointName, startdate, lastRun, pointMethod, pointDimValue, pointTime, pointType, pointFuzzinessBefore, pointFuzzinessAfter, pointOffset, pointDays, absolutePointTime, pointId);
}	

function sunToTimeUnits(suntime){
	suntime = suntime.split(':');
	var hourSize = constDeviceRowWidth/24; //24 hours...
	return hourSize * suntime[0] + hourSize * suntime[1]/60;
}

function willSunSet(){
	var sunData = getSunData();
	return !(sunData[2] && sunData[2] != "")
}
