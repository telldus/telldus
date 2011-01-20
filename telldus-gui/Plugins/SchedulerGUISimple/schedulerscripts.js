var activeStates = new Array();

function addState(state){
	activeStates.push(state);
}

function getActiveStates(){
	return activeStates;
}

function getBarWidth(currentBar, currentPointRect, pointList){
	
	var maxWidth = currentPointRect.parent.width;
	var nextX = maxWidth
	var halfPointWidth = currentPointRect.width / 2
	for(var i=0;i<pointList.length;i++){
		if (pointList[i].isPoint != undefined && pointList[i].isPoint == "true") {
			if((pointList[i].x + halfPointWidth) < nextX && (pointList[i].x - halfPointWidth) > (currentPointRect.x + halfPointWidth)){
				nextX = pointList[i].x + halfPointWidth;
			}
		}
	}
	return nextX - (currentPointRect.x + halfPointWidth);
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
		print("Rise: " + sunrise);
		print("Set: " + sunset);
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
	//TODO error whn < 30 (width)
	//recalculateWidth(currentPointRect.x, pointList);
	return getBarWidth(currentBar, currentPointRect, pointList);
}

/*
function recalculateWidth(currentx, pointList){
	var prevPoint = null;
	for(var i=0;i<pointList.length;i++){
		if(pointList[i].isPoint != undefined && pointList[i].isPoint == "true"){
			if(pointList[i].x < currentx && (prevPoint == null || pointList[i].x > prevPoint.x)){
				prevPoint = pointList[i];
			}
		}
	}
	
	if(prevPoint == null || prevPoint.hangOnToBar == undefined){
		return;
	}
	
	var temp = getBarWidth(prevPoint.hangOnToBar, prevPoint, pointList);
	print(temp);
	
	//prevPoint.hangOnToBar.width = temp;
}
*/
