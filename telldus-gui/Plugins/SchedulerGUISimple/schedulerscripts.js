
var weekday_name_array = new Array("Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"); //TODO share this with mainscripts.js

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

function getOffsetWeekday(index){
	//TODO this can be modified based on locale, not adding 1 if week should start with sunday
	index = parseInt(index);
	index = index + 1;
	if(index == weekday_name_array.length){
		index = 0;
	}
	return index;
}

function getOffsetWeekdayName(index){
	index = getOffsetWeekday(index);
	return weekday_name_array[index];
}

function hasBarChangingPoints(pointList){
	
	for(var i=0;i<pointList.length;i++){
		if (pointList[i].isPoint != undefined && pointList[i].isPoint == "true" && pointList[i].state != "bell") {  //TODO make "bell" dynamic?
			return true;
		}
	}
	return false;
}

function pad(number, length) {
   
    var str = '' + number;
    while (str.length < length) {
        str = '0' + str;
    }
   
    return str;
}

function setActionTypeModel(activeStates, typeListModel){
	
	typeListModel.clear();
	for(var i=0;i<activeStates.length;i++){
		typeListModel.append({"name": activeStates[i], "imagesource": activeStates[i] + ".png"});
	}
	
	return typeListModel;
}
