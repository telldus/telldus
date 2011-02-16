//STATES:
var activeStates = new Array();

function addActiveState(state){
	activeStates.push(state);
}

function getActiveStates(){
	return activeStates;
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
			
			var dayIndex = currentPointRect.deviceRow.currentDayIndex - 1;
			if(dayIndex == -1){ //too far, begin from end again
				dayIndex = 6; //week length - 1
			}
			var prevDayDevice = currentPointRect.parent.getDeviceRow(dayIndex, currentPointRect.deviceRow.deviceId);
			if(prevDayDevice != undefined && prevDayDevice.endPoint != undefined){ 
				return prevDayDevice.endPoint.state;
			}
		}
		
		return "";
	}
	
	return prevPoint.state;
}

function setActiveStates(newActiveStates){
	activeStates = newActiveStates;
}


//CHILD POINTS:

var childPoints = {};

function addChildPoint(index, point){
	childPoints[index] = point;
}

function getChildPoint(index){
	return childPoints[index];
}

function getChildPoints(){
	return childPoints;
}

function removeChildPoint(index){
	var toBeRemoved = childPoints[index];
	delete childPoints[index];
	toBeRemoved.remove("true");
}

function removeParentPoint(newParentPoint){
	delete childPoints[newParentPoint.deviceRow.parent.parent.daydate.getDay()]; //remove the current point from the child list
	newParentPoint.setChildPoints(childPoints); //copy child list to current child (making it a parent)
	newParentPoint.lastRun = newParentPoint.parentPoint.lastRun;
	newParentPoint.pointId = newParentPoint.parentPoint.pointId;
	newParentPoint.parentPoint = undefined;
	updateParentsInChildList(newParentPoint); //update all other child points (if any) with the current point as their parent
	childPoints = {};
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

function updateChildPoints(){
	
	var children;
	var localParentPoint;
	if(pointRect.parentPoint != undefined){
		children = pointRect.parentPoint.getChildPoints();
		localParentPoint = pointRect.parentPoint;
	}
	else{
		children = getChildPoints();
		localParentPoint = pointRect;
	}
	
	for(var point in children){
		children[point].absoluteHour = localParentPoint.absoluteHour;
		children[point].absoluteMinute = localParentPoint.absoluteMinute;
		children[point].fuzzyBefore = localParentPoint.fuzzyBefore;
		children[point].fuzzyAfter = localParentPoint.fuzzyAfter;
		children[point].offset = localParentPoint.offset;
		children[point].triggerstate = localParentPoint.triggerstate;
		children[point].dimvalue = localParentPoint.dimvalue;
		children[point].state = localParentPoint.state;
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