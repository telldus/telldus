import Qt 4.7
import "schedulerscripts.js" as Scripts

Rectangle{
	id: pointRect
	property string actionTypeColor: "blue" //TODO default value
	property int actionType: 1 //TODO default value
	property int dimvalue: 50  //percent, recalucluate it before use
	property double actionTypeOpacity: 1
	property string actionTypeImage: imageActionOn
	property string isPoint: "true"
	property variant isLoaded
	//property int xvalue
	property int fuzzyBefore: 0
	property int fuzzyAfter: 0
	property int offset: -100
	property int absoluteHour: parseInt(dialog.absoluteHour, 10)
	property int absoluteMinute: parseInt(dialog.absoluteMinute, 10)
	property alias triggerstate: trigger.state
	property variant parentPoint
	property int parentPointAbsoluteHour //TEST changed from int, want "undefined"
	property alias deviceRow: pointRect.parent
	
	Component.onCompleted: {
		//TODO useless really, still gets Cannot anchor to a null item-warning...
		isLoaded = "true"
		var actionBar = Qt.createComponent("ActionBar.qml")
		var dynamicBar = actionBar.createObject(pointRect)
		dynamicBar.hangOnToPoint = pointRect
		dynamicBar.state = "pointLoaded"
		//pointRect.hangOnToBar = dynamicBar
	}
	
	//use item instead of rectangle (no border then though) to make it invisible (opacity: 0)
	width: constPointWidth
	height: constDeviceRowHeight
	border.color: "black"
	opacity: 1 //0.8
	z: 100
	state: "on"
	focus: true
	//actionTypeColor: getColor()
	
	//TODO make this work:
	/*Keys.onLeftPressed: {
		pointRect.x = pointRect.x - 1
	}
	Keys.onRightPressed: {
		pointRect.x = pointRect.x + 1
	}
		   
		    Text {
        //focus: true
		text: pointRect.activeFocus ? "I HAVE active focus!" : "I do NOT have active focus"
     }
     */
	
	onAbsoluteHourChanged: {
		//print("ABSOLUTE HOUR CHANGED");
		Scripts.updateParentAbsoluteHour();
		Scripts.updateChildPoints();
		pointRect.x = getAbsoluteXValue();
	}
	
	/*
	onParentPointAbsoluteHourChanged: {
		//pointRect.absoluteHour = parentPoint.absolutHour
		print("IN ABSOLUTEHOURCHANGED...");
		if(parentPoint == undefined){  //TODO this is not correct, must remove, but...
			print("PP UNDEFINED!");  //Should never happen
			dialog.absoluteHour = absoluteHour; //this is needed for update 
			print("Absolute hour changed from " + dialog.absoluteHour + " to " + absoluteHour);
		}
		else if(parentPointAbsoluteHour != undefined){
			print("PPABSOLUTE HOUR NOT UNDEFINED!");
			dialog.absoluteHour = parentPointAbsoluteHour; //this is needed for update 
			print("Absolute hour changed from " + dialog.absoluteHour + " to " + parentPointAbsoluteHour);
		}
	}
	*/
	
	onDimvalueChanged: {
		updateBars();
	}
	
	MouseArea {
		id: pointRectMouseArea
		acceptedButtons: Qt.LeftButton | Qt.RightButton
		
		onClicked: {
			if (mouse.button == Qt.RightButton){
				pointRect.toggleType()
				dialog.show(pointRect)
			}
			else{
				dialog.show(pointRect)
			}
		}
		
		onPositionChanged: {
			//var rootCoordinates = pointRect.mapToItem(pointRect.parent, mouse.x, mouse.y);
			var rootCoordinates = pointRect.mapToItem(pointRect.parent, mouse.x, mouse.y);
			var hourMinute = getTimeFromPosition(rootCoordinates.x - mouse.x + pointRect.width/2)
			if((hourMinute[0] >= 0) && hourMinute[0] < 24){
				pointRect.absoluteHour = hourMinute[0]
				pointRect.absoluteMinute = hourMinute[1]
			}
		}
		
		onReleased: {
			pointRect.deviceRow.updateContinuingBars()
			dialog.show(pointRect)  //TODO not pointRect, but parentPoint if such exists
			//var rootCoordinates = pointRect.mapToItem(pointRect.parent, mouse.x, mouse.y);
			//var hourMinute = getTimeFromPosition(rootCoordinates.x)
			//print("Released reporting in");
			dialog.absoluteHour = Scripts.pad(pointRect.absoluteHour, 2) //Scripts.pad(hourMinute[0], 2)
			dialog.absoluteMinute = Scripts.pad(pointRect.absoluteMinute, 2) //Scripts.pad(hourMinute[1], 2)
			
			if(parentPoint != undefined){
				parentPoint.absoluteHour = parseInt(dialog.absoluteHour, 10);
			}
		}
		
		anchors.fill: parent
		drag.target: pointRect
		drag.axis: Drag.XAxis
		drag.minimumX: -1 * pointRect.width/2
		drag.maximumX: pointRect.parent.width - pointRect.width/2  //TODO: om pointRect.parent == null, då bara 0...
		drag.filterChildren: true //TODO testing this
		//TODO make it impossible to overlap (on release)
		//TODO drag to most right - jumps back, why?
		
		states: State{
			id: "hidden"; when: pointRectMouseArea.drag.active
			PropertyChanges { target: pointRect; opacity: 0.5; }
		}
	}
	/*
	ListModel{
		id: daysOfWeek
		ListElement{
			name: "on"
			imagesource: "on.png" //TODO cannot use javascript properties here... do in some other way, maybe a list with names here?
		}
		ListElement{
			name: "off"
			imagesource: "off.png"
		}
	}
	*/
	
	Column{
		spacing: 10
		anchors.horizontalCenter: parent.horizontalCenter
				
		Image {
			//opacity: 1
			id: actionImage
			width: 20; height: 20
			source: pointRect.actionTypeImage
		}
		
		Rectangle{
			id: trigger
			anchors.horizontalCenter: parent.horizontalCenter
				
			state: "absolute"
			width: 20; height: 20
			
			//TODO state should move the point to correct place... (sunrisetime, sunsettime or absolute (stored value, the one that is dragged)
			states: [
				State {
					//TODO if no sunrise/sunset exists (arctic circle...), check so it works anyway
					name: "sunrise"
					PropertyChanges { target: triggerImage; source: imageTriggerSunrise; opacity: 1 }
					PropertyChanges { target: triggerTime; opacity: 0 }
					PropertyChanges { target: pointRectMouseArea; drag.target: undefined }
					PropertyChanges { target: pointRect; x: getSunRiseTime.callWith(pointRect.parent.width, pointRect.width) + minutesToTimelineUnits(pointRect.offset) } //TODO se nedan
				},
				State {
					name: "sunset"
					PropertyChanges { target: triggerImage; source: imageTriggerSunset; opacity: 1 }
					PropertyChanges { target: triggerTime; opacity: 0 }
					PropertyChanges { target: pointRectMouseArea; drag.target: undefined }
					PropertyChanges { target: pointRect; x: getSunSetTime.callWith(pointRect.parent.width, pointRect.width) + minutesToTimelineUnits(pointRect.offset) } //TODO räkna om till tidsunits
				},
				State {
					name: "absolute"; when: !pointRectMouseArea.drag.active
					PropertyChanges { target: triggerImage; opacity: 0; }
					PropertyChanges { target: triggerTime; opacity: 1 }
					PropertyChanges { target: pointRectMouseArea; drag.target: parent }
					//PropertyChanges { target: pointRect; x: xvalue }
					PropertyChanges { target: pointRect; x: getAbsoluteXValue() }
				}
			]
			
			Rectangle{
				id: triggerTime
				opacity: 1
				width: 20; height: 20
				anchors.centerIn: parent
				Text{
					text: getTime(pointRect.x, pointRect.width); font.pointSize: 6; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignBottom
				}
			}
			
			Image {
				id: triggerImage
				opacity: 0
				anchors.fill: parent
				width: 20; height: 20
				source: imageTriggerAbsolute
			}
		}
	}
	
	states: [
		State {
			name: "on"
			PropertyChanges { target: pointRect; actionTypeColor: "blue"; actionTypeOpacity: 1 } 
			PropertyChanges { target: pointRect; actionTypeImage: imageActionOn }
			StateChangeScript{ name: "updateBars"; script: updateBars(); }
		},
		State{
			name: "off"
			PropertyChanges { target: pointRect; actionTypeColor: "gainsboro"; actionTypeOpacity: 0 }
			PropertyChanges { target: pointRect; actionTypeImage: imageActionOff }
			StateChangeScript{ name: "updateBars"; script: updateBars(); }
		},
		State{
			name: "dim"
			PropertyChanges { target: pointRect; actionTypeColor: "green"; actionTypeOpacity: dimvalue/100 }
			PropertyChanges { target: pointRect; actionTypeImage: imageActionDim }
			StateChangeScript{ name: "updateBars"; script: updateBars(); }
		},
		State{
			name: "bell"
			PropertyChanges { target: pointRect; actionTypeColor: getLastPointColor() }
			PropertyChanges { target: pointRect; actionTypeImage: imageActionBell }
			StateChangeScript{ name: "updateBars"; script: updateBars(); }
		}//,
		/*
		State{ //TODO test
			name: "test"; when: pointRect.parentPoint != undefined //&& pointRect.parentPoint.absoluteHour != undefined
			StateChangeScript{ name: "updateBars"; script: print("CHANGED TO TEST, " + pointRect.parentPoint.absoluteHour); }
			PropertyChanges{ target: pointRect; parentPointAbsoluteHour: pointRect.parentPoint.absoluteHour }
		}
		*/
	]
	
	Rectangle{
		width: minutesToTimelineUnits(fuzzyAfter + fuzzyBefore)
		height: constBarHeight
		anchors.verticalCenter: parent.verticalCenter
		x: parent.width/2 - minutesToTimelineUnits(fuzzyBefore)
		opacity: 0.2
		z: 140
		
		Image{
			anchors.fill: parent
			fillMode: Image.Tile
			source: "fuzzy.png"
		}
	}
	
	function updateBars(){
		if(pointRect.deviceRow != undefined){
			pointRect.deviceRow.updateContinuingBars();
		}
	}
	
	function getAbsoluteXValue(){
		if(pointRect.parent == null){
			return 0;
		}
		var hourSize = pointRect.parent.width / 24;
		var point = pointRect;
		if(pointRect.parentPoint != undefined){
			//print("Different x");
			point = pointRect.parentPoint;
		}
		//print("ABSOLUTE X-value: " + (point.absoluteHour * hourSize + hourSize * (point.absoluteMinute/60) - point.width/2));
		//print("AbsoluteHour: " +point.absoluteHour+ " hourSize: " + hourSize + " AbsoluteMinute: " + point.absoluteMinute + " Width: " + point.width); 
		return point.absoluteHour * hourSize + hourSize * (point.absoluteMinute/60) - point.width/2;
	}
	
	function toggleType(){ //TODO other kind of selection method
		var index = 0;
		var activeStates = Scripts.getActiveStates();
		if(activeStates == undefined || activeStates.length == 0){
			return;
		}
		
		for(var i=0;i<activeStates.length;i++){
			if (activeStates[i] == state) {
				index = i + 1;
				break;
			}
		}
		
		if(index == activeStates.length){
			index = 0; //return to beginning again
		}
		
		pointRect.state = activeStates[index];
	}
	
	function setType(name){
		pointRect.state = name;
	}
	
	function toggleTrigger(){ //TODO other kind of selection method
		if(trigger.state == "sunrise"){
			trigger.state = "sunset";
		}
		else if(trigger.state == "sunset"){
			trigger.state = "absolute";
			pointRect.x = getAbsoluteXValue();
		}
		else if(trigger.state == "absolute"){
			//pointRect.xvalue = pointRect.x;
			trigger.state = "sunrise";
		}
	}
	
	function getLastPointColor(){
		//get previous point:
		var prevPoint = null;
		var pointList = pointRect.parent.children;
		for(var i=1;i<pointList.length;i++){
			if (pointList[i].isPoint != undefined && pointList[i] != pointRect) {
				if(pointList[i].x < pointRect.x && (prevPoint == null || pointList[i].x > prevPoint.x)){
					prevPoint = pointList[i];
				}
			}
		}
		
		//TODO Binding loop here when moving transperent point over other point
		//...or just changing state to transparent
		//something with point depending on point depending on point?
		if(prevPoint == null || prevPoint.actionTypeOpacity == 0){
			//no point before, no bar after either
			actionTypeOpacity = 0
			return "papayawhip" //just return a color, will not be used
		}
		
		actionTypeOpacity = prevPoint.actionTypeOpacity
		return prevPoint.actionTypeColor
	}
	
	function getTime(){
		if(pointRect.parent == null){
			return "";
		}
		
		var hours = Scripts.pad(pointRect.absoluteHour, 2);
		var minutes = Scripts.pad(pointRect.absoluteMinute, 2);
		return hours + ":" + minutes;
	}
	
	function getTimeFromPosition(mouseX){
		if(pointRect.parent == null){
			return [0,0];
		}
		var timeOfDay = mouseX; // + (pointRect.width/2);
		var hourSize = pointRect.parent.width / 24;
		var hours = Math.floor(timeOfDay / hourSize);
		var partOfHour = ((timeOfDay - (hourSize * hours))/hourSize) * 60
		partOfHour = Math.floor(partOfHour);
		partOfHour = Scripts.pad(partOfHour, 2);
		hours = Scripts.pad(hours, 2);
		return [hours, partOfHour];
	}
	
	function addState(state){
		//print("Adding state: " + state);
		Scripts.addState(state);
	}
	
	function setFirstState(firstState){
	
		//print("SETTING FIRST STATE");
		var activeStates = Scripts.getActiveStates();
		
		if(activeStates == null || activeStates.length == 0){
			//nothing to do
			return;
		}
		
		//state may already be set:
		if(firstState != undefined && firstState != ""){
			pointRect.state = firstState;
			return;
		}
		
		//check that device has the "off" state:
		var exists = false;
		for(var i=1;i<activeStates.length;i++){
			if(activeStates[i] == "off"){
				exists = true;
				break;
			}
		}
		
		if(!exists){
			//no "off", just set state to the first added state
			
			pointRect.state = activeStates[0];
			return;
		}
		
		var previousState = Scripts.getPreviousState(pointRect);
		if(previousState == undefined || previousState == "" || previousState == "off"){
			//nothing on/dimmed at the moment, use first added state
			pointRect.state = activeStates[0];
			return;
		}
		
		pointRect.state = "off"; //previous point should be "on" or "dim"														 
	}
	
	function remove(keepDialogOpen, ignoreParent){
		if(ignoreParent == undefined && pointRect.parentPoint != undefined){
			//remove from parent instead
			pointRect.parentPoint.remove();
			return;
		}
		if(pointRect.hangOnToBar != null){
			hangOnToBar.destroy();
		}
		var x = pointRect.x;
		pointRect.isPoint = "false"
		var pointList = pointRect.parent.children;
		var deviceRow = pointRect.deviceRow;
		var childPoints = Scripts.getChildPoints();
		for(var child in childPoints){
			childPoints[child].remove(keepDialogOpen, "ignoreParent");
			delete childPoints[child];
		}
		pointRect.destroy();
		if(keepDialogOpen == undefined){
			dialog.hide();
		}
		deviceRow.updateContinuingBars();
	}
	
	function minutesToTimelineUnits(minutes){
		if(pointRect.parent == null){
			return 0;
		}
		return pointRect.parent.width/24 * (minutes/60);
	}
	
	function getTickedImageSource(index){
		//print("GETTING TICKED");
		index = Scripts.getOffsetWeekday(index);
		if(pointRect.deviceRow.parent == undefined || pointRect.deviceRow.parent.parent == undefined){ //to get rid of warnings on initialization
			print("UNDEFINED, should only be in beginning");
			return "unticked.png";
		}
		var originalPoint = pointRect; // pointRect.deviceRow.parent.parent;
		if(pointRect.parentPoint != undefined){
			originalPoint = pointRect.parentPoint; //.deviceRow.parent.parent;
		}
		if(index == pointRect.deviceRow.parent.parent.daydate.getDay()){ //TODO property or so
			//current day should always be ticked
			return "alwaysticked.png";
		}
		else if(originalPoint.getChildPoint(index) == undefined && index != originalPoint.deviceRow.parent.parent.daydate.getDay()){  //TODO turn this parent-parent into a property
			return "unticked.png";
		}
		else{
			return "ticked.png";
		}
	}
	
	function toggleTickedWeekDay(index){
		index = Scripts.getOffsetWeekday(index);
		var originalPoint = pointRect;
		if(pointRect.parentPoint != undefined){
			originalPoint = pointRect.parentPoint;
		}
		if(index == pointRect.deviceRow.parent.parent.daydate.getDay()){
			//cannot change this, do nothing
			return;
		}
		if(index == originalPoint.deviceRow.parent.parent.daydate.getDay()){
			//trying to remove the parentPoint, special removal procedure needed
			originalPoint.removeParentPoint(pointRect);
		}
		else if(originalPoint.getChildPoint(index) == undefined){
			print("CREATE NEW POINT");
			originalPoint.addChildPoint(index, deviceRow.createChildPoint(index, pointRect, deviceRow.deviceId));
		}
		else{
			print("REMOVE A POINT");
			originalPoint.removeChildPoint(index);
		}
	}
	
	function getChildPoint(index){
		return Scripts.getChildPoint(index);
	}
	function getChildPoints(){
		return Scripts.getChildPoints();
	}
	function addChildPoint(index, point){
		Scripts.addChildPoint(index, point);
	}
	function removeChildPoint(index){
		Scripts.removeChildPoint(index);
	}
	function removeParentPoint(newParentPoint){
		Scripts.removeParentPoint(newParentPoint);
	}
	function setChildPoints(childPoints){
		Scripts.setChildPoints(childPoints);
	}
}
