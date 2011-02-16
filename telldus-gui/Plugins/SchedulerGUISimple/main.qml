//import QtQuick 1.0 TODO not until 4.7.1
import Qt 4.7
import "schedulerscripts.js" as Scripts
import "mainscripts.js" as MainScripts

 Item{
	id: main
	width: mainWidth
	height: mainHeight
	property int dayListHeaderHeight: 15
	property variant selectedDate: MainScripts.getCurrentDate();  //Maybe just today
	property variant sunData: MainScripts.getSunData();
	
	Component.onCompleted: {
		var dynamicDay = 0;
		var previousDayEndsWithPoint = null; //previous day, nothing to begin with
		var startday = new Date();
		startday.setDate(startday.getDate() - 6);
		for(var i=0;i<7;i++){  //One week
			dynamicDay = dayListViewComponent.createObject(mainContent)
			dynamicDay.state = "hiddenLeft";
			var currentDay = new Date(startday);
			currentDay.setDate(startday.getDate() + i);
			dynamicDay.daydate = currentDay;
			MainScripts.addDay(dynamicDay);
		}
		dynamicDay.state = "visible" //set last day (today) as visible
		weekDayText.text = MainScripts.getCurrentDayName()
		MainScripts.updateDeviceIndex();
		MainScripts.setLoading();
		var updateLastRunFunc = updateLastRun;
		restoreJobs.callWith(updateLastRunFunc);
		MainScripts.initiateStoredPointsInGUI();
		MainScripts.endLoading();
		MainScripts.updateEndsWith();
	}

	Rectangle{
		id: mainTop
		height: 75
		width: parent.width
		anchors.top: parent.top
		anchors.left: parent.left
		NavButton{
			id: leftarrow
			anchors.right: weekDayText.left
			anchors.verticalCenter: weekDayText.verticalCenter
			arrowText: "<-"
			onClicked: {
				MainScripts.decrementCurrentDay();
			}
		}
		Text{
			id: weekDayText
			anchors.centerIn: parent
		}
		NavButton{
			id: rightarrow
			anchors.left: weekDayText.right
			anchors.verticalCenter: weekDayText.verticalCenter
			arrowText: "->"
			onClicked: {
				MainScripts.incrementCurrentDay();
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
								//else, remove this button
								deviceNameRect.isEnabled = deviceNameRect.isEnabled == "enabled" ? "disabled" : "enabled"
								MainScripts.deviceEnabled(modelData.id, deviceNameRect.isEnabled);
							}
						}
					}
				}
			}
		}
		
		Rectangle{
			id: mainContent
			height: mainHeight
			width: constDeviceRowWidth
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
			height: mainHeight
			
			model: deviceModel
			delegate: listRow
			header: timeline
			snapMode: ListView.SnapToItem
			interactive: false //no scroll between devices at the moment
			
			states: [
				State {
					name: "hiddenRight";
					AnchorChanges { target: dayListView; anchors.left: parent.right; anchors.right: undefined }
				},
				State {
					name: "hiddenLeft";
					AnchorChanges { target: dayListView; anchors.left: undefined; anchors.right: parent.left }
				},
				State {
					name: "visible";
					PropertyChanges { target: dayListView; opacity: 1 }
					AnchorChanges { target: dayListView; anchors.right: undefined; anchors.left: parent.left }
				}
			]
			
			transitions: [
				Transition {
					from: "hiddenLeft"
					to: "visible"
					AnchorAnimation { easing.type: Easing.InOutQuad; duration: 1000 }
				},
				Transition {
					from: "visible"
					to: "hiddenRight"
					AnchorAnimation { easing.type: Easing.InOutQuad; duration: 1000 }
				},
				Transition {
					from: "visible"
					to: "hiddenLeft"
					AnchorAnimation { easing.type: Easing.InOutQuad; duration: 1000 }
				},
				Transition {
					from: "hiddenRight"
					to: "visible"
					AnchorAnimation { easing.type: Easing.InOutQuad; duration: 1000 }
				}
			]
		}
	}
	
	Component{
		id: listRow
		
		Rectangle { id: "deviceRow"; border.color: "blue"; width: constDeviceRowWidth; height: constDeviceRowHeight;
			clip: true
			property variant endPoint: undefined
			property int deviceId: modelData.id;
			property alias continuingBar: continuingBar
			property variant selectedDate: main.selectedDate
			property int currentDayIndex: MainScripts.getCurrentDayIndex(main.selectedDate)
			
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
				onClicked: {
					MainScripts.setChanged(deviceId, true);
					MainScripts.updateDeviceIndex(); //needed here, adding points to empty device will get wrong state otherwise (test with one "on", put new device on the row below = off)
					var component = Qt.createComponent("ActionPoint.qml")
					var dynamicPoint = component.createObject(deviceRow)
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
					dynamicPoint.setFirstState();
					dynamicPoint.pointId = new Date().getTime(); //just needed for storage update
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
			
			//device functions:
			function createChildPoint(index, pointRect, deviceId){
				return MainScripts.createChildPoint(index, pointRect, deviceId);
			}
			
			function getDeviceRow(dayIndex, deviceId){
				return MainScripts.getDeviceRow(dayIndex, deviceId);
			}
			
			function hasPoints(){
				return Scripts.hasBarChangingPoints(deviceRow.children);
			}
			
			function isLoading(){
				return MainScripts.isLoading();
			}
			
			function setChanged(){
				MainScripts.setChanged(deviceId, true);
			}
			
			function updateContinuingBars(){
				MainScripts.updateEndsWith();
			}	
		}
	}
	
	Component{
		id: timeline
		Item{
			id: timelinerange
			width: constDeviceRowWidth
			height: dayListHeaderHeight
			
			property int morningDarkWidth: MainScripts.getMorningDarkWidth()
			property double morningDarkStart: MainScripts.getMorningDarkStart()
			property int eveningDarkWidth: MainScripts.getEveningDarkWidth()
			property double eveningDarkStart: MainScripts.getEveningDarkStart()
			property variant currentDate: main.selectedDate;
			
			onCurrentDateChanged: {
				//update values
				timelinerange.morningDarkWidth = MainScripts.getMorningDarkWidth()
				timelinerange.morningDarkStart = MainScripts.getMorningDarkStart()
				timelinerange.eveningDarkWidth = MainScripts.getEveningDarkWidth()
				timelinerange.eveningDarkStart = MainScripts.getEveningDarkStart()
			}
						
			Rectangle{
				id: morningDark
				color: "black"
				height: parent.height
				border.width: 0
				width: timelinerange.morningDarkWidth
				opacity: 0.1
				x: timelinerange.morningDarkStart
			}
			
			Rectangle{
				id: eveningDark
				color: "black"
				height: parent.height
				border.width: 0
				width: timelinerange.eveningDarkWidth
				opacity: 0.1
				x: timelinerange.eveningDarkStart
			}
			
			Rectangle{
				width: parent.width
				border.color: "red"

				Row{
					anchors.horizontalCenter: parent.horizontalCenter
					anchors.horizontalCenterOffset: parent.width/24

					width: parent.width
					height: parent.height
					spacing: (parent.width-24)/24
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
	
	Dialog {
		id: dialog
		z: 150
	}
	
	function mainGetMethodFromState(state){
		return getMethodFromState.callWith(state);
	}
	
	function mainGetTypeFromTriggerstate(triggerstate){
		return getTypeFromTriggerstate.callWith(triggerstate);
	}
		
	function saveAll(){
		var days = MainScripts.getDays();
		if(dialog.dialogOpacity == 1){
			MainScripts.setChanged(dialog.actionPoint.deviceRow.deviceId, true); //set the devicerow that the currently visible dialog is connected to as dirty
		}
		for(var i=0;i<deviceModel.length;i++){
			var points = new Array();
			var deviceId = deviceModel.get(i).id;
			if(!MainScripts.hasChanged(deviceId)){
				//no point has been updated, removed or added, ignore this device
				continue;
			}
			MainScripts.setChanged(deviceId, false); //reset hasChanged-status
				
			if(MainScripts.deviceIsEnabled(deviceId)){ //if device is disabled, don't add any points to schedule (but remove current)
				for(var j=0;j<days.length;j++){
					var row = MainScripts.getDeviceRow(days[j].daydate.getDay(), deviceId);
					for(var k=0;k<row.children.length;k++){
						var point = row.children[k];
						if(point.isPoint && point.parentPoint == undefined){ //and not disabled
							points.push(MainScripts.pointToArray(point));
						}
					}
				}
			}
			
			var deviceTimerKeys = MainScripts.getDeviceTimerKeys(deviceId); //get timer keys for this device, for removal
			var updateLastRunFunc = updateLastRun;
			deviceTimerKeys = addJobsToSchedule.callWith(deviceId, points, deviceTimerKeys, updateLastRunFunc);  //remove all schedules for this device, and add them again
			MainScripts.setDeviceTimerKeys(deviceId, deviceTimerKeys); //save the new timer keys
		}	
	}
	
	function updateCurrentDay(){
		main.selectedDate = MainScripts.getCurrentDate();
		main.sunData = updateSunData.callWith(main.selectedDate);
		MainScripts.updateSunData(main.sunData);
		weekDayText.text = MainScripts.getCurrentDayName();
	}
	
	function updateEndsWith(){
		MainScripts.updateEndsWith();
	}
	
	function updateLastRun(deviceId, day, pointId, lastRun){
		MainScripts.updateLastRun(deviceId, day, pointId, lastRun);
	}
}
