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
		return ["0:0","0:0",""]; //TODO
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


//INIT:
function initiateStoredPointsInGUI(){
	for(var devicekey in storedPoints){
		for(var i=0;i<storedPoints[devicekey].length;i++){
			addPointToGUI(devicekey, storedPoints[devicekey][i]);
		}
	}
}

function addPointToGUI(key, job){
	if(job == undefined){
		return;
	}
	var deviceId = key;
	var jobdata = job.v;
	var state = getStateFromMethod.callWith(jobdata.method);
	var activeStates = new Array("on", "off", "dim", "bell"); //TODO get dynamically, depending on device...
	var dimvalue = jobdata.value;
	var absoluteTime = jobdata.absoluteTime;
	var pointId = jobdata.id;
	print("POINTID: " + jobdata.id);
	var lastRun = jobdata.lastRun;
	print("Adding point to GUI with lastrun: " + lastRun);
	var events = jobdata.events;
	var parentPoint;
	
	for(var key in events){
		var eventdata = events[key].d;
		var dayIndex = eventdata.value;
		var dayOfWeek = getDayIndexForDayOfWeek(dayIndex); //set dayOfWeek to correct index in the days-table
		
		print("Inserting point at: " + weekday_name_array[days[dayOfWeek].daydate.getDay()] + " (" + dayOfWeek + ")" + ", id: " + deviceId);
		
		var pointParentDevice = getDeviceRow(dayOfWeek, deviceId);
		
		var component = Qt.createComponent("ActionPoint.qml");
		var dynamicPoint = component.createObject(pointParentDevice);
		
		if(parentPoint == undefined){
			//set common values
			var time = getTimeFromSeconds(absoluteTime); //eventdata.time);
			dynamicPoint.absoluteHour = time[0]; //same time for all
			dynamicPoint.absoluteMinute = time[1]; //same time for all
			dynamicPoint.triggerstate = getTriggerstateFromType.callWith(eventdata.type);
			dynamicPoint.fuzzyBefore = eventdata.fuzzinessBefore/60;
			dynamicPoint.fuzzyAfter = eventdata.fuzzinessAfter/60;
			dynamicPoint.offset = eventdata.offset/60;
			dynamicPoint.lastRun = lastRun;
			if(dynamicPoint.triggerstate == "absolute"){
				dynamicPoint.x = dynamicPoint.getAbsoluteXValue();
			}
			dynamicPoint.setActiveStates(activeStates); //TODO: active states depending on the device (get this from __init__ etc)
			dynamicPoint.setFirstState(state);
			dynamicPoint.dimvalue = dimvalue * (100/255);
			dynamicPoint.pointId = pointId;
			parentPoint = dynamicPoint;
		}
		else{
			dynamicPoint.parentPoint = parentPoint;
			dynamicPoint.setActiveStates(parentPoint.getActiveStates());
			parentPoint.addChildPoint(dayIndex, dynamicPoint);
		}
		
		dynamicPoint.border.color = "blue"; //default blue at the moment
		
	}
		
	parentPoint.updateChildPoints();  //if any child points exists, update them with parent values
}

function getTimeFromSeconds(seconds){
	var totalMinutes = seconds/60;
	var minutes = totalMinutes%60;
	var hours = Math.floor(totalMinutes/60);
	return [hours, minutes];
}


//DEVICE PROPERTIES:

var deviceProperties = {};

function deviceEnabled(deviceId, enabled){
	updateDeviceIndex(); //TODO make this unnessessary
	for(var i=0;i<days.length;i++){
		var deviceRow = days[i].children[0].children[deviceIndex[deviceId]];
		deviceRow.state = enabled; //TODO connect directly instead... if possible
	}
	setChanged(deviceId, true);
}

function deviceIsEnabled(deviceId){
	return days[0].children[0].children[deviceIndex[deviceId]].state == "enabled";
}

function getDeviceTimerKeys(deviceId){
	var device = deviceProperties[deviceId];
	return device == undefined ? {} : device["timerkeys"];
}

function setDeviceTimerKeys(deviceId, deviceTimerKeys){
	if(deviceProperties[deviceId] == undefined){
		deviceProperties[deviceId] = {};
	}
	deviceProperties[deviceId]["timerkeys"] = deviceTimerKeys;
}

function hasChanged(deviceId){
	var device = deviceProperties[deviceId];
	var hasChanged = device == undefined ? false : device["hasChanged"];
	return hasChanged;
}

function setChanged(deviceId, change){
	if(deviceProperties[deviceId] == undefined){
		deviceProperties[deviceId] = {};
	}
	deviceProperties[deviceId]["hasChanged"] = change;
}


//DEVICE INDEX:

var deviceIndex = [];

function assignContinuingBarProperties(deviceRow, previousEndPoint, dayIndex, firstRow){
	
	if(previousEndPoint == undefined){ //the first has no point to bind to
		deviceRow.continuingBar.prevDayColor = "white";
		deviceRow.continuingBar.prevDayOpacity = 0;
		deviceRow.continuingBar.prevDayWidth = 0;
	}
	else{
		deviceRow.continuingBar.prevDayColor = previousEndPoint.actionTypeColor;
		if(previousEndPoint.state == "dim"){
			deviceRow.continuingBar.prevDayOpacity = previousEndPoint.dimvalue/100;
		}
		else{
			deviceRow.continuingBar.prevDayOpacity = previousEndPoint.actionTypeOpacity;
		}
		
	}

	if(deviceRow.hasPoints()){
		deviceRow.endPoint = getEndsWith(deviceRow.children, days[dayIndex].daydate.getDay(), deviceRow.deviceId);
		previousEndPoint = deviceRow.endPoint;
		deviceRow.continuingBar.prevDayWidth = 0;
		deviceRow.continuingBar.state = "continuingWithLimitedWidth";
	}
	else{
		deviceRow.continuingBar.state = "continuing";
		deviceRow.endPoint = previousEndPoint;
		deviceRow.continuingBar.prevDayWidth = deviceRow.width;
	}
	
	return previousEndPoint;
}

function getEndsWith(pointList, dayIndex, deviceId){
	var prevPoint = null;
	for(var i=0;i<pointList.length;i++){
		if(pointList[i].isPoint != undefined && pointList[i].isPoint == "true"){
			if((prevPoint == null || pointList[i].x > prevPoint.x) && pointList[i].state != "bell"){ //TODO when more than "bell", make dynamic
				prevPoint = pointList[i];
			}
		}
	}
	
	if(prevPoint == null){
		dayIndex = dayIndex - 1;
		if(dayIndex == -1){ //too far, begin from end again
			dayIndex = days.length - 1;
		}
		
		var prevDayDevice = getDeviceRow(dayIndex, deviceId);
		if(prevDayDevice == undefined){
			return null; 
		}
		return prevDayDevice.endPoint;
	}
	if(prevPoint.state == "off"){
		return null;
	}
	return prevPoint;  //only on or dim
}

function updateDeviceIndex(){  //TODO, better way, please...
	deviceIndex = []; //empty list
	var startIndex = 0;
	for(var i=0;i<days[0].children[0].children.length;i++){
		var deviceListItem = days[0].children[0].children[i];
		if(deviceListItem.deviceId != undefined){
			startIndex = i;
			break;
		}
	}
	for(var i=0;i<deviceModel.length;i++){
		deviceIndex[deviceModel.get(i).id] = i + startIndex;
	}
}

function updateEndsWith(){
	updateDeviceIndex(); //TODO needed?
	for(var device in deviceIndex){
		//for each device, order doesn't matter
		var previousEndPoint = undefined;
		//loop through days, beginning with oldest, in search for the first Point
		var startIndex = 0;
		for(var dayIndex=0;dayIndex<days.length;dayIndex++){
			var deviceRow = days[dayIndex].children[0].children[(parseInt(deviceIndex[device]))]; //+1 TODO property somehow? Or function...
			
			if(deviceRow.hasPoints()){
				startIndex = dayIndex;
				break;
			}
		}
		
		//Second loop, start from first point
		for(var i=0;i<days.length;i++){
			var dayIndex = i + startIndex;
			if(dayIndex > days.length-1){
				dayIndex = dayIndex - days.length;
			}
			
			var deviceRow = days[dayIndex].children[0].children[parseInt(deviceIndex[device])];
			
			previousEndPoint = assignContinuingBarProperties(deviceRow, previousEndPoint, dayIndex, i==0);
			
			if(i == days.length-1){
				//last one, bind the first one too then
				deviceRow = days[startIndex].children[0].children[parseInt(deviceIndex[device])];
				assignContinuingBarProperties(deviceRow, previousEndPoint, dayIndex, false);
			}
		}
	}
}


//DAYS:

var weekday_name_array = new Array("Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday");
var currentDayIndex = 6; //Today...
var days = new Array();

function addDay(day){
	days.push(day);
}

function decrementCurrentDay(){
	days[currentDayIndex].state = "hiddenRight";
	var workDate = days[currentDayIndex].daydate;
	workDate.setDate(workDate.getDate() - 7);
	days[currentDayIndex].daydate = workDate;
	if(currentDayIndex == 0){
		currentDayIndex = days.length - 1;
	}
	else{
		currentDayIndex--;
	}
	days[currentDayIndex].state = "hiddenLeft"; //place on right side of center, for correct slid
	days[currentDayIndex].state = "visible";
}

function getCurrentDay(){
	return days[currentDayIndex];
}

function getCurrentDayIndex(dummyvalue){
	//by sending in this dummyvalue, this function will be evaluated when that value is changed...
	return currentDayIndex;
}

function getCurrentDate(){
	if(days.length < 7){
		//not initiated yet
		return new Date(); //now
	}
	var date = new Date(days[currentDayIndex].daydate);
	return date;
}

function getCurrentDayName(){
	var day = getCurrentDay().daydate;
	return weekday_name_array[day.getDay()] + " " + day.getDate();
}

function getDayIndexForDayOfWeek(dayOfWeek){
	var offset = days[0].daydate.getDay();
	dayOfWeek = days.length - offset + parseInt(dayOfWeek);
	if(dayOfWeek == -1){
		dayOfWeek = days.length - 1;
	}
	if(dayOfWeek > days.length-1){
		dayOfWeek = dayOfWeek - days.length;
	}
	return dayOfWeek;
}

function getDays(){
	return days;
}

function incrementCurrentDay(){
	days[currentDayIndex].state = "hiddenLeft";
	var workDate = days[currentDayIndex].daydate;
	workDate.setDate(workDate.getDate() + 1);
	if(currentDayIndex == (days.length - 1)){
		currentDayIndex = 0;
	}
	else{
		currentDayIndex++;
	}
	days[currentDayIndex].daydate = workDate;
	days[currentDayIndex].state = "hiddenRight"; //place on right side of center, for correct slide
	days[currentDayIndex].state = "visible";	
}

function updateLastRun(deviceId, day, pointId, lastRun){
	day = getDayIndexForDayOfWeek(day);
	var row = getDeviceRow(day,deviceId);
	for(var k=0;k<row.children.length;k++){
		var point = row.children[k];
		if(point.isPoint && point.parentPoint == undefined && point.pointId == pointId){ //and not disabled
			point.lastRun = lastRun;
		}
	}
}


//OTHER:

function getDeviceRow(dayOfWeek, deviceId){
	var dayListViewComp = days[dayOfWeek];
	if(dayListViewComp == undefined){
		print("DayListViewComp undefined");
		return null;
	}
	var currentDeviceIndex = deviceIndex[deviceId];
	if(dayListViewComp.children.length == undefined){
		return null;
	}
	var pointParent = dayListViewComp.children[0].children[currentDeviceIndex];
	return pointParent;
}

function createChildPoint(index, pointRect, deviceId){
	index = getDayIndexForDayOfWeek(index);
	var deviceRow = getDeviceRow(index, deviceId);
	var component = Qt.createComponent("ActionPoint.qml");
	var dynamicPoint = component.createObject(deviceRow);
	dynamicPoint.absoluteHour = pointRect.absoluteHour;
	dynamicPoint.absoluteMinute = pointRect.absoluteMinute;
	dynamicPoint.fuzzyBefore = pointRect.fuzzyBefore;
	dynamicPoint.fuzzyAfter = pointRect.fuzzyAfter;
	dynamicPoint.offset = pointRect.offset;
	dynamicPoint.dimvalue = pointRect.dimvalue;
	
	dynamicPoint.parentPoint = pointRect
	dynamicPoint.x = dynamicPoint.getAbsoluteXValue();
	dynamicPoint.triggerstate = pointRect.triggerstate;
	dynamicPoint.border.color = "blue"
	dynamicPoint.setActiveStates(pointRect.getActiveStates());
	
	dynamicPoint.setFirstState(pointRect.state);
	return dynamicPoint;
}


//LOADING:
var loading = false;
function setLoading(){
	loading = true;
}

function endLoading(){
	loading = false;
}

function isLoading(){
	return loading;
}