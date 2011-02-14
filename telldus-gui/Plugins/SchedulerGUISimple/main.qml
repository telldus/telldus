//import QtQuick 1.0 TODO not until 4.7.1
import Qt 4.7
import "schedulerscripts.js" as Scripts

 Item{
	id: main
	width: 800 //TODO how?
	height: 700 //TODO how?
	property variant sunData
	property int dayListHeaderHeight: 15

	Component.onCompleted: {
		var dynamicDay = 0;
		var previousDayEndsWithPoint = null; //previous day, nothing to begin with
		var startday = new Date();
		startday.setDate(startday.getDate() - 6);
		for(var i=0;i<7;i++){  //One week, TODO dynamic
			dynamicDay = dayListViewComponent.createObject(mainContent)
			dynamicDay.state = "hiddenLeft";
			var currentDay = new Date(startday);
			currentDay.setDate(startday.getDate() + i);
			dynamicDay.daydate = currentDay;
			//TODO previousDayEndsWith = 
			Scripts.addDay(dynamicDay);
			Scripts.updateDeviceIndex();  //TODO, don't run in every iteration
		}
		dynamicDay.state = "visible" //set last one as visible
		weekDayText.text = Scripts.getCurrentDayName()
		//Scripts.updateDeviceIndex();
		Scripts.setLoading();
		var updateLastRunFunc = updateLastRun;
		restoreJobs.callWith(updateLastRunFunc);
		Scripts.initiateStoredPointsInGUI();
		Scripts.endLoading();
		Scripts.updateEndsWith();
	}

	Rectangle{
		id: mainTop
		height: 75
		width: parent.width
		anchors.top: parent.top
		anchors.left: parent.left
		Rectangle{
			anchors.right: weekDayText.left
			anchors.verticalCenter: weekDayText.verticalCenter
			height: 20
			width: 20
			border.color: "red"
			Text{
				anchors.centerIn: parent
				text: "<-"
			}
			MouseArea{
				anchors.fill: parent
				onClicked: {
					//step to prev weekday
					dialog.hide()
					Scripts.updateEndsWith()
					Scripts.decrementCurrentDay()
					weekDayText.text = Scripts.getCurrentDayName()
					//mainListView.positionViewAtIndex(mainListView.currentIndex, ListView.Center)
					//mainListView.currentIndex = mainListView.currentIndex - 1
				}
			}
		}
		Text{
			id: weekDayText
			anchors.centerIn: parent
			//text: ""
			/*states: State{
				name: "loaded";
				PropertyChanges{ target: weekDayText; text: Scripts.getCurrentDay().dayName }
			}
			*/
			
		}
		Rectangle{
			anchors.left: weekDayText.right
			anchors.verticalCenter: weekDayText.verticalCenter
			height: 20
			width: 20
			border.color: "red"
			Text{
				anchors.centerIn: parent
				text: "->"
			}
			MouseArea{
				anchors.fill: parent
				onClicked: {
					//step to next weekday
					dialog.hide()
					Scripts.updateEndsWith() //mainListView)
					Scripts.incrementCurrentDay()
					weekDayText.text = Scripts.getCurrentDayName()
					//mainListView.positionViewAtIndex(mainListView.currentIndex, ListView.Center)
					//mainListView.currentIndex = mainListView.currentIndex + 1
				}
			}
		}
	}

	Row{
		id: mainRow
		width: parent.width
		anchors.top: mainTop.bottom
		anchors.left: parent.left
			
		Column{
			id: mainHeader
			//anchors.left: parent.left
			//anchors.top: parent.top
			//anchors.topMargin: dayListHeaderHeight
			width: main.width - constDeviceRowWidth
			
			spacing: 0
			z: 60
			Rectangle{
				id: filler
				height: dayListHeaderHeight
				width: parent.width
				border.color: "red"
			}
			
			Repeater{
				
				model: deviceModel
				Rectangle{
					id: deviceNameRect
					property string isEnabled: modelData.isEnabled
					width: parent.width
					height: constDeviceRowHeight
					border.color: "green"
					Text{
						id: "deviceNameText"
						anchors.centerIn: parent
						text: modelData.name
					}
					Rectangle{
						width: 10
						height: 10
						anchors.top: deviceNameText.bottom
						anchors.horizontalCenter: parent.horizontalCenter
						color: deviceNameRect.isEnabled == "enabled" ? "red" : "blue"  //TODO use same as in script?
						MouseArea{
							anchors.fill: parent
							onClicked: {
								//TODO implement setProperty if this is to be stored... or do something else... modelData.setProperty(index, isEnabled, modelData.isEnabled == "enabled" ? "disabled" : "enabled");
								//TODO also implement disabled/enabled for all (deviceEnabled=false on all) and per point (set isPoint=false and opacity=0.4)...
								//take that value into consideration when saving and sending jobs for execution
								deviceNameRect.isEnabled = deviceNameRect.isEnabled == "enabled" ? "disabled" : "enabled"
								Scripts.deviceEnabled(modelData.id, deviceNameRect.isEnabled);
							}
						}
					}
				}
			}
		}
		
		Rectangle{
			id: mainContent
			height: 700 //TODO
			width: constDeviceRowWidth //TODO 
			//anchors.top: parent.top
			//anchors.left: mainListViewHeaderColumn.right
			clip: true
			z: 50
		}
		
	}
	
	Component{
		id: dayListViewComponent
		ListView {
			id: dayListView
			property date daydate
			width: constDeviceRowWidth
			height: 700 //TODO
			
			//anchors.fill: parent
			model: deviceModel
			delegate: listRow
			//focus: true
			//footer: addButtonComponent  //TODO move...
			header: timeline
			snapMode: ListView.SnapToItem
			interactive: false //no scroll between devices at the moment
			//anchors.top: parent.top;
			//state: "hidden" //default
			
			states: [
				State {
					name: "hiddenRight"; //when: parent.left != null
					AnchorChanges { target: dayListView; anchors.left: parent.right; anchors.right: undefined }
					//PropertyChanges { target: dayListView; opacity: 0 }
				},
				State {
					name: "hiddenLeft"; //when: parent.left != null
					AnchorChanges { target: dayListView; anchors.left: undefined; anchors.right: parent.left }
					//PropertyChanges { target: dayListView; opacity: 0 }
				},
				State {
					name: "visible"; //when: parent.left != null
					PropertyChanges { target: dayListView; opacity: 1 }
					AnchorChanges { target: dayListView; anchors.right: undefined; anchors.left: parent.left }
				}
			]
			
			onChildrenChanged: {
				//TODO REMOVE
				print("-------------- CHILDREN CHANGED ----------------");
			}
			
			transitions: [
				Transition {
					from: "hiddenLeft"
					to: "visible"
					AnchorAnimation { easing.type: Easing.InOutQuad; duration: 1000 } //PropertyAnimation { properties: "x"; duration: 1000; easing.type: Easing.InOutQuad }
				},
				Transition {
					from: "visible"
					to: "hiddenRight"
					AnchorAnimation { easing.type: Easing.InOutQuad; duration: 1000 } //PropertyAnimation { properties: "x"; duration: 1000; easing.type: Easing.InOutQuad }
				},
				Transition {
					from: "visible"
					to: "hiddenLeft"
					AnchorAnimation { easing.type: Easing.InOutQuad; duration: 1000 } //PropertyAnimation { properties: "x"; duration: 1000; easing.type: Easing.InOutQuad }
				},
				Transition {
					from: "hiddenRight"
					to: "visible"
					AnchorAnimation { easing.type: Easing.InOutQuad; duration: 1000 } //PropertyAnimation { properties: "x"; duration: 1000; easing.type: Easing.InOutQuad }
				}
			]
			
			//TODO transition between state, animation
			
		}
	}
	
	Component{
		id: listRow
		
		/*
		Row{
			id: mainRow
			
			width: constDeviceRowWidth //parent.width; //TODO relative
			height: constDeviceRowHeight
			/*
			Rectangle {
				border.color: "red"; width: 100; height:parent.height;
				Text{
					anchors.verticalCenter: parent.verticalCenter
					anchors.horizontalCenter: parent.horizontalCenter
					
					text: modelData.name
				}
			}
			*/
			Rectangle { id: "deviceRow"; border.color: "blue"; width: constDeviceRowWidth; height: constDeviceRowHeight;
				clip: true
				property variant endPoint: undefined //: Scripts.getEndsWith(deviceRow.children, index, modelData.id);
				property int deviceId: modelData.id;
				property alias continuingBar: continuingBar
				state: "enabled"
				
				states: [
					State{
						name: "enabled"
						PropertyChanges{ target: deviceRow; opacity: 1 }
					},
					State{
						name: "disabled"
						PropertyChanges{ target: deviceRow; opacity: 0.4 }
					}
				]
										 
				MouseArea {
					id: deviceMouseArea
					anchors.fill: parent
					//hoverEnabled: true

					//TODO ändra muspekaren kanske?
					//onEntered: parent.border.color = onHoverColor
					//onExited:  parent.border.color = borderColor
					onClicked: {
						//Scripts.debugPrintDeviceIndex(); 
						Scripts.setChanged(deviceId, true);
						Scripts.updateDeviceIndex(); //TODO why is this needed here? (but it is, adding points to empty device will get wrong state otherwise)
						//print("Mainrow: " + mainRow.parent.children[0].height);
						var component = Qt.createComponent("ActionPoint.qml")
						var dynamicPoint = component.createObject(deviceRow)
						//dynamicPoint.x = mouseX - dynamicPoint.width/2 //xposition
						var hourMinute = dynamicPoint.getTimeFromPosition(mouse.x)
						dynamicPoint.absoluteHour = hourMinute[0]
						dynamicPoint.absoluteMinute = hourMinute[1]
						dynamicPoint.x = dynamicPoint.getAbsoluteXValue();
						dynamicPoint.border.color = "blue"
						
						//TODO different states depending on the device							
						dynamicPoint.addActiveState("on");
						dynamicPoint.addActiveState("off");
						dynamicPoint.addActiveState("dim");
						dynamicPoint.addActiveState("bell");
						//dynamicPoint.setFirstState("dim"); //when type is a stored value
						dynamicPoint.setFirstState();
						dynamicPoint.pointId = new Date().getTime(); //just needed for storage update
						
						
						//TEST, proof of concept for repeat-everyday-points:
						//if deleted, must check if parent exists before deletion
						//in that case, parent must have list of all its children and delete them too...
						/*
						var dynamicPoint2 = component.createObject(deviceRow.parent.parent.children[1])
						dynamicPoint2.absoluteHour = dynamicPoint.absoluteHour
						dynamicPoint2.absoluteMinute = 30
						dynamicPoint2.parentPoint = dynamicPoint
						dynamicPoint2.x = dynamicPoint2.getAbsoluteXValue();
						dynamicPoint2.border.color = "blue"
						dynamicPoint2.addActiveState("on");
						dynamicPoint2.addActiveState("off");
						dynamicPoint2.addActiveState("dim");
						dynamicPoint2.addActiveState("bell");
						dynamicPoint2.setFirstState();
						*/
						//SLUT TEST
						
						deviceRow.updateContinuingBars();
						dialog.show(dynamicPoint) 
					}
				}
				
				ActionBar{
					id: "continuingBar"
					hangOnToPoint: deviceRow
					firstBar: "true"
						
					state: "continuing"
				}
				
				ListView.onAdd: SequentialAnimation {
					PropertyAction { target: mainRow; property: "height"; value: 0 }
					NumberAnimation { target: mainRow; property: "height"; to: 50; duration: 250; easing.type: Easing.InOutQuad }
				}
				
				function hasPoints(){
					return Scripts.hasBarChangingPoints(deviceRow.children);
				}
				
				function updateContinuingBars(){
					Scripts.updateEndsWith();
				}	
				
				function getDeviceRow(dayIndex, deviceId){
					print("CALLING getDeviceRow");
					return Scripts.getDeviceRow(dayIndex, deviceId);
				}
				
				function createChildPoint(index, pointRect, deviceId){
					print("CREATING child point");
					return Scripts.createChildPoint(index, pointRect, deviceId);
				}
				
				function setChanged(){
					Scripts.setChanged(deviceId, true);
				}
				
				function isLoading(){
					return Scripts.isLoading();
				}
			}
		//}
	}
	
	Component{
		id: timeline
		Item{
			width: constDeviceRowWidth //TODO parent.width
			height: dayListHeaderHeight
			
			Rectangle{
				id: morningDark
				color: "black"
				height: parent.height
				border.width: 0
				width: getMorningDarkWidth()
				opacity: 0.1
				x: getMorningDarkStart()
			}
			
			Rectangle{
				id: eveningDark
				color: "black"
				height: parent.height
				border.width: 0
				width: getEveningDarkWidth()
				opacity: 0.1
				x: getEveningDarkStart()
			}
			
			Rectangle{
				width: parent.width
				border.color: "red"

				Row{
					//x: 100
					anchors.horizontalCenter: parent.horizontalCenter
					anchors.horizontalCenterOffset: parent.width/24

					width: parent.width //listRow.width
					height: parent.height
					spacing: (parent.width-24)/24  //before: -124
					Repeater{
						model:24
						Rectangle{
							width: 1
							height: 15
							color: "green"
						}
					}
				}
			}
		}
	}

	Button{
		id: save
		text: "Save changes"
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.horizontalCenterOffset: -30
		anchors.top: mainRow.bottom
		anchors.topMargin: -100
		onClicked: {
			saveAll();
		}
	}
	
	/* Not in use, adding all devices always instead
	 Component{
		id: addButtonComponent
		Row {
			id: addButton
			
			Rectangle {
				height: 20
				width: 100
				anchors.verticalCenter: parent.verticalCenter
				anchors.verticalCenterOffset: 30

				property color buttonColor: "lightgrey"

				Text{
					text: "Add an item"
					anchors.verticalCenter: parent.verticalCenter
					anchors.horizontalCenter: parent.horizontalCenter
					anchors.margins: 2

				}
				MouseArea {
					id: buttonMouseArea
					anchors.fill: parent
					onClicked: {
						addDevice.call()
					}
				}

				color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor

			}
		}
	 }
	*/
	
	Dialog {
		id: dialog
		z: 150
	}
	
	//TODO move functions, and test odd cases:
	function getEveningDarkStart(){
		if(!willSunSet){
			return 0;
		}
		if(Scripts.isMidnightDark()){
			return sunToTimeUnits(main.sunData[1]); // + 100;
		}
		else{
			return 0;
		}
	}
	
	function getMorningDarkStart(){
		//TODO the day of the year when the sun "begins" not to set, will it work then?
		if(!willSunSet){
			return 0;
		}
		if(Scripts.isMidnightDark()){
			return 0; //100; //TODO constants
		}
		else{
			return sunToTimeUnits(main.sunData[1]); // + 100; //TODO constants
		}
	}
	
	function getEveningDarkWidth(){
		if(!willSunSet){
			return 0;
		}
		if(Scripts.isMidnightDark()){
			return constDeviceRowWidth - sunToTimeUnits(main.sunData[1]); //(main.width - 100) - sunToTimeUnits(main.sunData[1]); //TODO constant or something
		}
		else{
			return 0;
		}
		
	}
	
	function getMorningDarkWidth(){
		if(!willSunSet){
			return 0;
		}
		if(Scripts.isMidnightDark()){
			return sunToTimeUnits(main.sunData[0]);
		}
		else{
			return sunToTimeUnits(main.sunData[1]) - sunToTimeUnits(main.sunData[0]);
		}
	}
	
	function willSunSet(){
		if(main.sunData == undefined){
			main.sunData = getSunData.call();
		}
			
		return !(main.sunData[2] && main.sunData[2] != "")
	}

	function sunToTimeUnits(suntime){
		suntime = suntime.split(':');
		var hourSize = constDeviceRowWidth/24; //(main.width - 100)/24; //TODO constant or something?
		return hourSize * suntime[0] + hourSize * suntime[1]/60;
	}
	
	function saveAll(){
		print("Save all and reset jobs");
		//var jobs = new Array();
		//var points = new Array();
		var days = Scripts.getDays();
		if(dialog.dialogOpacity == 1){
			Scripts.setChanged(dialog.actionPoint.deviceRow.deviceId, true); //set the devicerow that the currently visible dialog is connected to as dirty
		}
		for(var i=0;i<deviceModel.length;i++){
			var points = new Array();
			var deviceId = deviceModel.get(i).id;
			if(!Scripts.hasChanged(deviceId)){
				//no point has been updated, removed or added, ignore this device
				print("Not updated: " + deviceId);
				continue;
			}
			Scripts.setChanged(deviceId, false); //reset hasChanged-status
			print("Updated: " + deviceId);
				
			if(Scripts.deviceIsEnabled(deviceId)){ //if device is disabled, don't add any points to schedule (but remove current)
				print("Device is enabled: " + deviceId);
				for(var j=0;j<days.length;j++){
					var row = Scripts.getDeviceRow(days[j].daydate.getDay(), deviceId);
					for(var k=0;k<row.children.length;k++){
						var point = row.children[k];
						if(point.isPoint && point.parentPoint == undefined){ //and not disabled
							points.push(pointToArray(point));
						}
					}
				}
			}
			
			var deviceTimerKeys = Scripts.getDeviceTimerKeys(deviceId); //get timer keys for this device, for removal
			var updateLastRunFunc = updateLastRun;
			deviceTimerKeys = addJobsToSchedule.callWith(deviceId, points, deviceTimerKeys, updateLastRunFunc);  //remove all schedules for this device, and add them again
			Scripts.setDeviceTimerKeys(deviceId, deviceTimerKeys); //save the new timer keys
		}	
	}
	
	function updateLastRun(deviceId, day, pointId, lastRun){
		print("deviceid: " + deviceId);
		print("day: " + day);
		print("pointid: " + pointId);
		Scripts.updateLastRun(deviceId, day, pointId, lastRun);
	}
	
	function pointToArray(point){ //TODO another way than using arrays...
		var deviceId = point.deviceRow.deviceId; //not really in use yet
		var pointName = "Job_" + deviceId;
		var startdate = new Date(); //startdate, not in use, always "now"
		var pointDimValue = point.dimvalue * (255/100);
		var pointMethod = getMethodFromState.callWith(point.state);
		var pointId = point.pointId;
		var lastRun = point.lastRun;
		print("LASTRUN pointToArray: " + lastRun);
		
		var pointTime = point.absoluteHour * 3600 + point.absoluteMinute * 60;
		var absolutePointTime = pointTime;
		var pointType = getTypeFromTriggerstate.callWith(point.triggerstate);
		if(point.triggerstate == "sunrise"){
			var suntime = main.sunData[0].split(':');
			pointTime = suntime[0] * 3600 + suntime[1] * 60;
		}
		else if(point.triggerstate == "sunset"){
			var suntime = main.sunData[1].split(':');
			pointTime = suntime[0] * 3600 + suntime[1] * 60;
		}
		var pointFuzzinessBefore = point.fuzzyBefore;
		var pointFuzzinessAfter = point.fuzzyAfter;
		var pointOffset = point.triggerstate == "absolute" ? 0 : point.offset;
		
		var pointDays = new Array();
		pointDays.push(point.deviceRow.parent.parent.daydate.getDay());
		var childPoints = point.getChildPoints();
		for(var child in childPoints){
			pointDays.push(childPoints[child].deviceRow.parent.parent.daydate.getDay()); //different per event
			print("GOT DAY " + childPoints[child].deviceRow.parent.parent.daydate.getDay());
		}
		return new Array(deviceId, pointName, startdate, lastRun, pointMethod, pointDimValue, pointTime, pointType, pointFuzzinessBefore, pointFuzzinessAfter, pointOffset, pointDays, absolutePointTime, pointId);
	}
}
