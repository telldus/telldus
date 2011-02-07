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

function hasBarChangingPoints(pointList){
	
	for(var i=0;i<pointList.length;i++){
		if (pointList[i].isPoint != undefined && pointList[i].isPoint == "true" && pointList[i].state != "bell") {  //TODO make "bell" dynamic?
			return true;
		}
	}
	return false;
}

function getEndsWith(pointList, dayIndex, deviceId){ //previousDayEndsWithPoint){
	//print("DAY INDEX: " + dayIndex);
	//print("DEVICEID: " + deviceId);
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
		var prevDayDevice = getDeviceRow(dayIndex, deviceId);
		//print("PREVDAYHERE: index: " + dayIndex + " id: " + deviceId + " Daydevice: " + prevDayDevice);
		if(prevDayDevice == undefined){ // || prevDayDevice.endPoint == undefined){ //TODO this is due to an error, but in the future use it to avoid loops
			print("DayDeviceUndefined....................");
			return null; 
		}
		//print("RETURNING A PREV POINT");
		//print("Prev day: " + days[dayIndex].daydate.getDate());
		//print("Containing: " + prevDayDevice.endPoint);
		return prevDayDevice.endPoint;  // previousDayEndsWithPoint;
	}
	if(prevPoint.state == "off"){
		return null;
	}
	//print("RETURNING A POINT");
	return prevPoint;  //.state, only on or dim
}

function getPreviousState(currentPointRect){
	var pointList = currentPointRect.parent.children
	var prevPoint = null;
	var firstBarStateIndex;
	for(var i=0;i<pointList.length;i++){
		if(pointList[i].isPoint != undefined && pointList[i].isPoint == "true" && pointList[i] != currentPointRect){
			if(pointList[i].x < currentPointRect.x && (prevPoint == null || pointList[i].x > prevPoint.x) && pointList[i].state != "bell"){ //TODO when more than "bell", make dynamic
				prevPoint = pointList[i];
			}
		}
		else if(pointList[i].firstBar != undefined && pointList[i].firstBar == "true"){
			firstBarStateIndex = i;
		}
	}
	
	if(prevPoint == null){
		//no previous point,see if state continues from previous day
		if(firstBarStateIndex != undefined && pointList[firstBarStateIndex].firstBar != undefined){
			//print("ENDPOINT!!");
			
			var dayIndex = currentDayIndex - 1;
			if(dayIndex == -1){ //too far, begin from end again
				dayIndex = days.length - 1;
			}
			//print("CORRECT DEVICE ID? : " + currentPointRect.deviceRow.deviceId);
			var prevDayDevice = currentPointRect.parent.getDeviceRow(dayIndex, currentPointRect.deviceRow.deviceId);
			if(prevDayDevice != undefined){ 
				//print("Setting end point: ** " + prevDayDevice.endPoint);
				return prevDayDevice.endPoint;
			}
		}
		
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
	//TODO !!!!!! are devices reordered sometimes? Or even different for different days? Check that!
	//Kan det vara så här:
	// days[0].children[0].children består av alla delegat-element, plus header och footer
	//header, footer och delegaternas ordning kan skifta, men delegaternas interna ordning är (FÖRHOPPNINGSVIS!) alltid samma
	//och samma som i modellen: Delegaternas börjar antingen på 0 (delegater, header, footer eller delegater, footer, header)
	//eller 1 (header/footer, delegater, header/footer) eller 2 (header/footer, header/footer, delegater)
}

function debugPrintDeviceIndex(){ 
	//TODO debug
	if(days.length != undefined){
		print("DEBUG: " + days[0].children[0].children.length);	
	}
	for(var key in deviceIndex){
		print("** " + key + ": " + deviceIndex[key] + " **");
	}
}

function updateEndsWith(){
	updateDeviceIndex(); //TODO needed?
	//print("UPDATEENDSWITH");
	for(var device in deviceIndex){
		//for each device, order doesn't matter
		var previousEndPoint = undefined;
		//print("DeviceIndex: " + deviceIndex[device]);
		//print("DeviceId: " + device);
		
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
				//print("Dayindex " + dayIndex  + " HAS POINTS ..........");
				//print("Current day index: " + currentDayIndex);
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
			//print("ID " + deviceRow.deviceId);
			
			previousEndPoint = assignContinuingBarProperties(deviceRow, previousEndPoint, dayIndex, i==0);
			
			if(i == days.length-1){
				//last one, bind the first one too then
				deviceRow = days[startIndex].children[0].children[parseInt(deviceIndex[device])];
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

//Upstart:
function initiatePointsInGUI(){ //weekPointList){
	//för varje point, addPointToGUI, men ju även lägga till schemajobb... alltså i __init__.js... men därifrån kan inte denna anropas...
	//så det får väl bli varsin iteration då...
	//weekPointList -> från __init__.js
	var weekPointList = new Array();
	var dummypoint = []
	dummypoint["day"] = 0;
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
	print("DeviceIndex: " + currentDeviceIndex + " och " + deviceId + ", och sedan " + days.length);
	var pointParent = dayListViewComp.children[0].children[currentDeviceIndex];
	print("Picked device id (must be same as above): " + dayListViewComp.daydate); //children[0].children[currentDeviceIndex].deviceId);
	return pointParent;
}

function addWeekPointToGUI(point){
	//lägg till point till GUI
	var deviceId = point.deviceId;
	var dayOfWeek = point.day;
	//set dayOfWeek to correct index in the days-table
	dayOfWeek = getDayIndexForDayOfWeek(dayOfWeek);
	print("Inserting point at: " + weekday_name_array[days[dayOfWeek].daydate.getDay()]);
	var pointParent = getDeviceRow(dayOfWeek, deviceId);
	
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

//per point
var childPoints = [];

function getChildPoint(index){
	//print("INDEX");
	//print(index + ": " + childPoints[index]);
	return childPoints[index];
}

function getChildPoints(){
	return childPoints;
}

function addChildPoint(index, point){
	childPoints[index] = point;
}

function removeChildPoint(index){
	print("INDEX BEFORE REMOVE: " + childPoints[index]);
	var toBeRemoved = childPoints[index];
	childPoints[index] = undefined;
	toBeRemoved.remove("true");
	print("INDEX AFTER REMOVE: " + childPoints[index]);
}

function removeParentPoint(newParentPoint){
	print("Removing parent point...");
	delete childPoints[newParentPoint.deviceRow.parent.parent.daydate.getDay()]; //TODO remove this: = undefined; //remove the current point from the child list
	newParentPoint.setChildPoints(childPoints); //copy child list to current child (making it a parent)
	newParentPoint.parentPoint = undefined;
	updateParentsInChildList(newParentPoint); //update all other child points (if any) with the current point as their parent
	pointRect.remove("true");
}

function setChildPoints(newChildPoints){
	childPoints = newChildPoints;
}

function updateParentsInChildList(newParentPoint){
	var children = newParentPoint.getChildPoints();
	for(var point in children){
		children[point].parentPoint = newParentPoint;
	}
}

function updateChildPoints(parentPoint){
	for(var point in childPoints){
		childPoints[point].absoluteHour = pointRect.absoluteHour;
	}
}

function updateParentAbsoluteHour(){
	if(pointRect.parentPoint != undefined){
		pointRect.parentPoint.absoluteHour = pointRect.absoluteHour; //TODO check if this can be done with binding without loops...
	}
}

//end per point

//must be run in "main"
function getDayIndexForDayOfWeek(dayOfWeek){
	var offset = days[0].daydate.getDay();
	dayOfWeek = days.length - offset + dayOfWeek;
	if(dayOfWeek == -1){
		dayOfWeek = days.length - 1;
	}
	if(dayOfWeek > days.length-1){
		dayOfWeek = dayOfWeek - days.length;
	}
	return dayOfWeek;
}

function getOffsetWeekdayName(index){
	index = getOffsetWeekday(index);
	return weekday_name_array[index];
}

function getOffsetWeekday(index){
	//TODO this can be modified based on locale, not adding 1 of week should start with sunday
	index = index + 1;
	if(index == weekday_name_array.length){
		index = 0;
	}
	return index;
}

//TODO move, pragma safe:
function getFirstPointWidth(deviceRow){
	var pointList = deviceRow.children;  //TODO should really try to avoid using "children"... make own list instead?
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

function deviceEnabled(deviceId, enabled){
	updateDeviceIndex(); //TODO make this unnessessary
	for(var i=0;i<days.length;i++){
		var deviceRow = days[i].children[0].children[deviceIndex[deviceId]];
		deviceRow.state = enabled; //TODO connect directly instead... if possible
	}
}

function createChildPoint(index, pointRect, deviceId){
	index = getDayIndexForDayOfWeek(index);
	var deviceRow = getDeviceRow(index, deviceId); //TODO correct index?
	var component = Qt.createComponent("ActionPoint.qml")
	var dynamicPoint = component.createObject(deviceRow)
	dynamicPoint.absoluteHour = pointRect.absoluteHour
	//print("The absolute hour is: " + pointRect.absoluteHour);
	dynamicPoint.absoluteMinute = 30  //TODO
	dynamicPoint.parentPoint = pointRect
	dynamicPoint.x = dynamicPoint.getAbsoluteXValue();
	dynamicPoint.border.color = "blue"
	dynamicPoint.addState("on"); //TODO, add same states as in pointRect
	dynamicPoint.addState("off");
	dynamicPoint.addState("dim");
	dynamicPoint.addState("bell");
	dynamicPoint.setFirstState(pointRect.state);
	//print("RETURNING " + dynamicPoint);
	return dynamicPoint;
}