//TODO pragma instead? Which is best in this case?

var deviceIndex = [];
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

function getDays(){
	return days;
}

function getCurrentDay(){
	return days[currentDayIndex];
}

function getCurrentDayName(){
	var day = getCurrentDay().daydate;
	return weekday_name_array[day.getDay()] + " " + day.getDate();
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

function getPreviousState(currentPointRect, pointList){
	
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

function getNextAndPrevBarWidth(currentBar, currentPointRect, pointList){
	return getBarWidth(currentBar, currentPointRect, pointList);
}

function updateDeviceIndex(){
	deviceIndex = []; //empty list
	for(var i=0;i<deviceModel.length;i++){
		deviceIndex[deviceModel.get(i).id] = i;
	}
	//for(var listproperty in deviceIndex){
	//}
}

function initiatePointsInGUI(pointList){
	//för varje point, addPointToGUI, men ju även lägga till schemajobb... alltså i __init__.js... men därifrån kan inte denna anropas...
	//så det får väl bli varsin iteration då...
	//weekPointList -> från __init__.js
	for(var i=0;i<weekPointList.length;i++){
		addWeekPointToGUI(mainPointList[i]);
	}
}

function addWeekPointToGUI(point){
	//lägg till point till GUI
	var deviceId = point.deviceId;
	var dayOfWeek = point.day;
	
	//1. hitta rätt veckodag med getDay == dayOfWeek
	//2. i denna day, hitta rätt device... deviceIndex[deviceId] = rätt index
	//3. i denna listviewdelegates rektangel, lägg till en ny point, som man gör när man klickar ungefär... Lägg till i flera (med "parentpoint") om varje dag-punkt...)
	
}