//TODO pragma library instead? Which is best in this case? "pad" can be moved there

var deviceIndex = [];

//TODO active states - move to actionPoint-only-js file
var activeStates = new Array();

function addActiveState(state){
	activeStates.push(state);
}

function getActiveStates(){
	return activeStates;
}

function setActiveStates(newActiveStates){
	activeStates = newActiveStates;
}

//from main:
var deviceProperties = {};

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
		
		var prevDayDevice = getDeviceRow(dayIndex, deviceId);
		if(prevDayDevice == undefined){ // || prevDayDevice.endPoint == undefined){ //TODO this is due to an error, but in the future use it to avoid loops
			return null; 
		}
		return prevDayDevice.endPoint;  // previousDayEndsWithPoint;
	}
	if(prevPoint.state == "off"){
		return null;
	}
	return prevPoint;  //only on or dim
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
			
			var dayIndex = currentDayIndex - 1;
			if(dayIndex == -1){ //too far, begin from end again
				dayIndex = days.length - 1;
			}
			var prevDayDevice = currentPointRect.parent.getDeviceRow(dayIndex, currentPointRect.deviceRow.deviceId);
			if(prevDayDevice != undefined){ 
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
		if(deviceListItem.deviceId != undefined){
			startIndex = i;
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

//Init:
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
			print("SETTING POINTID TO: " + pointId);
			parentPoint = dynamicPoint;
		}
		else{
			dynamicPoint.parentPoint = parentPoint;
			dynamicPoint.setActiveStates(parentPoint.getActiveStates());
			parentPoint.addChildPoint(dayIndex, dynamicPoint);
		}
		
		dynamicPoint.border.color = "blue"; //default blue at the moment
		
	}
		
	updateChildPoints(parentPoint);  //if any child points exists, update them with parent values
}

function getTimeFromSeconds(seconds){
	var totalMinutes = seconds/60;
	var minutes = totalMinutes%60;
	var hours = Math.floor(totalMinutes/60);
	return [hours, minutes];
}
//End init

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
	//print("DeviceIndex: " + currentDeviceIndex + " och " + deviceId + ", och sedan " + days.length);
	var pointParent = dayListViewComp.children[0].children[currentDeviceIndex];
	return pointParent;
}

//per point
var childPoints = {};

function getChildPoint(index){
	//print("INDEX");
	//print(index + ": " + childPoints[index]);
	return childPoints[index];
}

function getChildPoints(){
	return childPoints;
}

function addChildPoint(index, point){
	print("Adding child point " + point + " at " + index);
	childPoints[index] = point;
}

function removeChildPoint(index){
	print("INDEX BEFORE REMOVE: " + childPoints[index]);
	var toBeRemoved = childPoints[index];
	delete childPoints[index];
	toBeRemoved.remove("true");
	print("INDEX AFTER REMOVE: " + childPoints[index]);
}

function removeParentPoint(newParentPoint){
	print("Removing parent point...");
	delete childPoints[newParentPoint.deviceRow.parent.parent.daydate.getDay()]; //remove the current point from the child list
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
	
	var children;
	if(parentPoint != undefined){
		children = parentPoint.getChildPoints();
	}
	else{
		if(pointRect.parentPoint != undefined){
			children = pointRect.parentPoint.getChildPoints();
			parentPoint = pointRect.parentPoint;
		}
		else{
			children = getChildPoints();
			parentPoint = pointRect;
		}
	}
	
	for(var point in children){
		children[point].absoluteHour = parentPoint.absoluteHour;
		children[point].absoluteMinute = parentPoint.absoluteMinute;
		children[point].fuzzyBefore = parentPoint.fuzzyBefore;
		children[point].fuzzyAfter = parentPoint.fuzzyAfter;
		children[point].offset = parentPoint.offset;
		children[point].triggerstate = parentPoint.triggerstate;
		children[point].dimvalue = parentPoint.dimvalue;
		children[point].state = parentPoint.state;
		if(children[point].triggerstate == "absolute"){
			children[point].x = children[point].getAbsoluteXValue();
		}
	}
}

function updateParentWithCurrentValues(){
	if(pointRect.parentPoint != undefined){
		pointRect.parentPoint.absoluteHour = pointRect.absoluteHour; //TODO check if this can be done with binding without loops...
		pointRect.parentPoint.absoluteMinute = pointRect.absoluteMinute;
		pointRect.parentPoint.fuzzyBefore = pointRect.fuzzyBefore;
		pointRect.parentPoint.fuzzyAfter = pointRect.fuzzyAfter;
		pointRect.parentPoint.offset = pointRect.offset;
		pointRect.parentPoint.triggerstate = pointRect.triggerstate;
		pointRect.parentPoint.dimvalue = pointRect.dimvalue;
		pointRect.parentPoint.state = pointRect.state;
	}
}

//end per point

//must be run in "main"
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

function getOffsetWeekdayName(index){
	index = getOffsetWeekday(index);
	return weekday_name_array[index];
}

function getOffsetWeekday(index){
	//TODO this can be modified based on locale, not adding 1 if week should start with sunday
	index = parseInt(index);
	index = index + 1;
	if(index == weekday_name_array.length){
		index = 0;
	}
	return index;
}

function updateLastRun(deviceId, day, pointId, lastRun){
	print("0");
	day = getDayIndexForDayOfWeek(day);
	print("1: " + day);
	var row = getDeviceRow(day,deviceId);
	print("2: " + row);
	for(var k=0;k<row.children.length;k++){
		print("3: " + row.children[k]);
		var point = row.children[k];
		if(point.isPoint && point.parentPoint == undefined && point.pointId == pointId){ //and not disabled
			print("UPDATING LAST RUN, " + lastRun);
			point.lastRun = lastRun;
			print("IS, " + point.lastRun);
			
		}
	}
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
	setChanged(deviceId, true);
}

function deviceIsEnabled(deviceId){
	return days[0].children[0].children[deviceIndex[deviceId]].state == "enabled";
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
	dynamicPoint.triggerstate = pointRect.triggerstate;
	dynamicPoint.dimvalue = pointRect.dimvalue;
	
	dynamicPoint.parentPoint = pointRect
	dynamicPoint.x = dynamicPoint.getAbsoluteXValue();
	dynamicPoint.border.color = "blue"
	dynamicPoint.setActiveStates(pointRect.getActiveStates());
	
	/*
	dynamicPoint.addActiveState("on"); //TODO, add same states as in pointRect
	dynamicPoint.addActiveState("off");
	dynamicPoint.addActiveState("dim");
	dynamicPoint.addActiveState("bell");
	*/
	dynamicPoint.setFirstState(pointRect.state);
	//print("RETURNING " + dynamicPoint);
	return dynamicPoint;
}