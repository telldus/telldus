//TODO pragma library instead? Which is best in this case? "pad" can be moved there

var deviceIndex = [];

//TODO active states - move to actionPoint-only-js file
var activeStates = new Array();

function addState(state){
	activeStates.push(state);
}

function getActiveStates(){
	return activeStates;
}

//Days:
var weekday_name_array = new Array("Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday")
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

function getCurrentDayName(){
	var day = getCurrentDay().daydate;
	return weekday_name_array[day.getDay()] + " " + day.getDate();
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

//Other:
function getBarWidth(currentBar, currentPointRect, pointList){
	
	var maxWidth = currentPointRect.parent.width;
	var nextX = maxWidth - currentPointRect.width / 2
	for(var i=0;i<pointList.length;i++){
		if (pointList[i].isPoint != undefined && pointList[i].isPoint == "true") {
			if(pointList[i].x < nextX && pointList[i].x > currentPointRect.x){
				nextX = pointList[i].x;
			}
		}
	}
	return (nextX - currentPointRect.x);
}

function hasPoints(pointList){
	
	for(var i=0;i<pointList.length;i++){
		if (pointList[i].isPoint != undefined && pointList[i].isPoint == "true") {
			return true;
		}
	}
	return false;
}

function getEndsWith(pointList, dayIndex, deviceId){ //previousDayEndsWithPoint){
	print("DAY INDEX: " + dayIndex);
	print("DEVICEID: " + deviceId);
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
		
		//TODO Avoid loop here!
		var prevDayDevice = getDeviceRow(dayIndex, deviceId, days, deviceIndex);
		print("PREVDAYHERE: index: " + dayIndex + " id: " + deviceId + " Daydevice: " + prevDayDevice);
		if(prevDayDevice == undefined){ // || prevDayDevice.endPoint == undefined){ //TODO this is due to an error, but in the future use it to avoid loops
			print("DayDeviceUndefined....................");
			return null; 
		}
		print("RETURNING A PREV POINT");
		print("Prev day: " + days[dayIndex].daydate.getDate());
		print("Containing: " + prevDayDevice.endPoint);
		return prevDayDevice.endPoint;  // previousDayEndsWithPoint;
	}
	if(prevPoint.state == "off"){
		return null;
	}
	print("RETURNING A POINT");
	return prevPoint;  //.state, only on or dim
}

function getPreviousState(currentPointRect){
	var pointList = currentPointRect.parent.children
	var prevPoint = null;
	for(var i=0;i<pointList.length;i++){
		if(pointList[i].isPoint != undefined && pointList[i].isPoint == "true" && pointList[i] != currentPointRect){
			if(pointList[i].x < currentPointRect.x && (prevPoint == null || pointList[i].x > prevPoint.x) && pointList[i].state != "bell"){ //TODO when more than "bell", make dynamic
				prevPoint = pointList[i];
			}
		}
	}
	
	if(prevPoint == null){
		return "";
	}
	
	return prevPoint.state;
}

function isMidnightDark(){
	if(main.sunData == undefined){
		main.sunData = getSunData.call();
	}
	
	var sunrise = main.sunData[0].split(':');;
	var sunset = main.sunData[1].split(':');
	
	if(sunset[0] < sunrise[0] || (sunset[0] == sunrise[0] && sunset[1] < sunrise[1])){
		return false;
	}
	return true;
}

function pad(number, length) {
   
    var str = '' + number;
    while (str.length < length) {
        str = '0' + str;
    }
   
    return str;
}

function updateDeviceIndex(){  //TODO, better way, please...
	deviceIndex = []; //empty list
	var startIndex = 0;
	for(var i=0;i<days[0].children[0].children.length;i++){
		var deviceListItem = days[0].children[0].children[i];
		//print("** DEVICELISTITEM: ** " + deviceListItem);
		if(deviceListItem.deviceId != undefined){
			startIndex = i;
			//print("** CORRECT DEVICELISTITEM **");
			break;
		}
	}
	for(var i=0;i<deviceModel.length;i++){
		deviceIndex[deviceModel.get(i).id] = i + startIndex;
	}
	//TODO !!!!!! are devices reordered sometimes? Check that!
}

function updateEndsWith(){
	updateDeviceIndex(); //TODO needed?
	print("UPDATEENDSWITH");
	for(var device in deviceIndex){
		//for each device, order doesn't matter
		var previousEndPoint = undefined;
		print("DeviceIndex: " + deviceIndex[device]);
		print("DeviceId: " + device);
		
		//loop through days, beginning with oldest, in search for the first Point
		var startIndex = 0;
		for(var dayIndex=0;dayIndex<days.length;dayIndex++){
			var deviceRow = days[dayIndex].children[0].children[(parseInt(deviceIndex[device]))]; //+1 TODO property somehow? Or function...
			/*
			 * DEBUG
			print("day: " + days[dayIndex]);
			print("child zero: " + days[dayIndex].children[0]);
			print("deviceRow: " + deviceRow);
			print("index: " + (parseInt(deviceIndex[device])));
			print("Length: " + days[dayIndex].children[0].children.length);
			*/
			//print("deviceRow, id: " + deviceRow.deviceId);
			//print("deviceRow: " + deviceRow.children.length);
			
			/*
			for (var myKey in days[dayIndex].children[0]) {
				print(myKey + " == (" + days[dayIndex].children[0][myKey] + ")");
			}
			*/
			
			if(deviceRow.hasPoints()){
				print("Dayindex " + dayIndex  + " HAS POINTS ..........");
				print("Current day index: " + currentDayIndex);
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
			
			var deviceRow = days[dayIndex].children[0].children[parseInt(deviceIndex[device])]; //+1 TODO property somehow?
			//print("ID " + deviceRow.deviceId);
			
			previousEndPoint = assignContinuingBarProperties(deviceRow, previousEndPoint, dayIndex, i==0);
			
			if(i == days.length-1){
				//last one, bind the first one too then
				deviceRow = days[startIndex].children[0].children[parseInt(deviceIndex[device])];
				print("In last one");
				assignContinuingBarProperties(deviceRow, previousEndPoint, dayIndex, false);
			}
		}
	}
}

function assignContinuingBarProperties(deviceRow, previousEndPoint, dayIndex, firstRow){
	
	if(previousEndPoint == undefined){ //the first has no point to bind to
		deviceRow.continuingBar.prevDayColor = "white";
		deviceRow.continuingBar.prevDayOpacity = 0;
		deviceRow.continuingBar.prevDayWidth = 0;
	}
	else{
		print("Color: " + previousEndPoint.actionTypeColor);
		print("Opacity: " + previousEndPoint.actionTypeOpacity);
		deviceRow.continuingBar.prevDayColor = previousEndPoint.actionTypeColor;
		if(previousEndPoint.state == "dim"){
			deviceRow.continuingBar.prevDayOpacity = previousEndPoint.dimvalue/100;
		}
		else{
			deviceRow.continuingBar.prevDayOpacity = previousEndPoint.actionTypeOpacity;
		}
		
	}

	if(deviceRow.hasPoints()){
		print("A Point!!");
		//TODO barWidth here... Must depend on first point... bind... somehow...
		deviceRow.endPoint = getEndsWith(deviceRow.children, days[dayIndex].daydate.getDay(), deviceRow.deviceId);
		previousEndPoint = deviceRow.endPoint;
		deviceRow.continuingBar.prevDayWidth = 0;
		deviceRow.continuingBar.state = "continuingWithLimitedWidth";
	}
	else{
		//print("NO points");
		deviceRow.continuingBar.state = "continuing";
		deviceRow.endPoint = previousEndPoint;
		deviceRow.continuingBar.prevDayWidth = deviceRow.width;
	}
	
	return previousEndPoint;
}

//Upstart:
function initiatePointsInGUI(){ //weekPointList){
	//för varje point, addPointToGUI, men ju även lägga till schemajobb... alltså i __init__.js... men därifrån kan inte denna anropas...
	//så det får väl bli varsin iteration då...
	//weekPointList -> från __init__.js
	var weekPointList = new Array();
	var dummypoint = []
	dummypoint["day"] = 5;
	dummypoint["deviceId"] = 1;
	weekPointList.push(dummypoint);
	for(var i=0;i<weekPointList.length;i++){
		print("Inne");
		addWeekPointToGUI(weekPointList[i]);
	}
}

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
	var pointParent = dayListViewComp.children[0].children[currentDeviceIndex]; //TEST was +1
	print("Pointparent: " + pointParent);
	return pointParent;
}

function addWeekPointToGUI(point){
	//lägg till point till GUI
	var deviceId = point.deviceId;
	var dayOfWeek = point.day;
	print("Dayofweek: " + dayOfWeek);
	var pointParent = getDeviceRow(dayOfWeek, deviceId, days, deviceIndex);
	
	var component = Qt.createComponent("ActionPoint.qml")
	var dynamicPoint = component.createObject(pointParent)
	dynamicPoint.absoluteHour = 12
	dynamicPoint.absoluteMinute = 30
	dynamicPoint.x = dynamicPoint.getAbsoluteXValue();
	dynamicPoint.border.color = "blue"
	dynamicPoint.addState("on");
	dynamicPoint.addState("off");
	dynamicPoint.addState("dim");
	dynamicPoint.addState("bell");
	dynamicPoint.setFirstState("dim");	
}

//TODO move, pragma safe:
function getFirstPointWidth(deviceRow){
	var pointList = deviceRow.children;
	var firstX = deviceRow.width;
	var pointWidth = 0;
	for(var i=0;i<pointList.length;i++){
		if (pointList[i].isPoint != undefined && pointList[i].isPoint == "true") {
			if(pointList[i].x < firstX){
				pointWidth = pointList[i].width; //same width for all
				firstX = pointList[i].x;
			}
		}
	}
	return firstX + pointWidth/2;
}