var activeStates = new Array();

function addState(state){
	activeStates.push(state);
}

function getActiveStates(){
	return activeStates;
}

function getBarWidth(currentBar, currentPointRect, pointList, currentDay){
	
	//TODO REMOVE currentDay too, currentDay.endsWith = ""; //reset
	var maxWidth = currentPointRect.parent.width;
	var nextX = maxWidth - currentPointRect.width / 2;
	for(var i=0;i<pointList.length;i++){
		if (pointList[i].isPoint != undefined && pointList[i].isPoint == "true") {
			if(pointList[i].x < nextX && pointList[i].x > currentPointRect.x){
				nextX = pointList[i].x;
			}
		}
	}
	/* TODO
	if(nextX == maxWidth - currentPointRect.width/2 && currentPointRect.state != "" && currentPointRect.state != "off"){
		currentDay.endsWith = currentPointRect.state;
		print("Ends with: " + currentDay.endsWith);
	}
	*/
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

function getEndsWith(pointList, previousDayEndsWith){
	var prevPoint = null;
	for(var i=0;i<pointList.length;i++){
		if(pointList[i].isPoint != undefined && pointList[i].isPoint == "true"){
			if((prevPoint == null || pointList[i].x > prevPoint.x) && pointList[i].state != "bell"){ //TODO when more than "bell", make dynamic
				prevPoint = pointList[i];
			}
		}
	}
	
	if(prevPoint == null){
		return previousDayEndsWith;
	}
	if(prevPoint.state == "off"){
		return "";
	}
	return prevPoint.state;  //only on or dim
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
/*
function getNextAndPrevBarWidth(currentBar, currentPointRect, pointList){
	return getBarWidth(currentBar, currentPointRect, pointList);
}
*/