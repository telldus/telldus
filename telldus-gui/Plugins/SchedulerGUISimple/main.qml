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
		Scripts.initiatePointsInGUI();
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
						dynamicPoint.addState("on");
						dynamicPoint.addState("off");
						dynamicPoint.addState("dim");
						dynamicPoint.addState("bell");
						//dynamicPoint.setFirstState("dim"); //when type is a stored value
						dynamicPoint.setFirstState();
						
						
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
						dynamicPoint2.addState("on");
						dynamicPoint2.addState("off");
						dynamicPoint2.addState("dim");
						dynamicPoint2.addState("bell");
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
		anchors.top: mainContent.bottom
		anchors.topMargin: 30
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
		var points = new Array();
		var days = Scripts.getDays();
		for(var i=0;i<deviceModel.length;i++){
			//punkt 1 och 2 (och 3)
			var deviceId = deviceModel.get(i).id;
			for(var j=0;j<days.length;j++){
				var row = Scripts.getDeviceRow(days[j].daydate.getDay(), deviceId);
				for(var k=0;k<row.children.length;k++){
					var point = row.children[k];
					if(point.isPoint && point.parentPoint == undefined){ //and not disabled
						print("and step 4");
						points.push(pointToArray(point));
						//jobs.push(pointToJob(point));
					}
				}
			}
		}
		
		print("after loops");
		//if(jobs.length > 0){
			//var timerkeys = addJobsToSchedule.callWith(jobs);
		//}
		if(points.length > 0){
			/*
			print("Nerifrån: " + points[0].absoluteHour);
			var koll = {"TEST": 55};
			print("Koll: " + koll["TEST"]);
			var underarray = new Array();
			underarray.push("hejsan");
			underarray.push("hoppsan");
			koll = new Array();
			koll.push(underarray);
			koll.push(8);
			*/
			addJobsToSchedule.callWith(points); //{"TEST": 55}); //points);
		}
		//1. for each device (som har hasChanged set):
		//2. ta bort alla timers med keys till denna device
		//3. om disabled = gör inte mer... annars:
		//4. alla punkter på alla dagar här, gå igenom:
		//5. om punkt, om inte har parentPoint, och inte disabled (senare):
		//6. gör om punkt till jobb (pointToJob), lägg till array, med events för alla childPoints (och sig själv)
		//7. skicka in denna array till schedulern (addJobs)
		//8. få tillbaka ny array med timerkeys, spara denna på devicen
	}
	
	function pointToArray(point){ //TODO another way than using arrays...
		var deviceId = point.deviceRow.deviceId; //not really in use yet
		var pointName = "Job_" + deviceId;
		var lastrun = 0; //TODO
		var startdate = new Date(); //startdate, not in use, always "now"
		var pointDimValue = point.dimvalue;
		var pointMethod = getMethodFromState.callWith(point.state);
		
		var pointTime = point.absoluteHour * 3600 + point.absoluteMinute * 60;
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
		for(var childPoint in point.childPoints){
			pointDays.push(point.childPoints[childPoint].deviceRow.parent.parent.daydate.getDay()); //different per event
		}
		return new Array(deviceId, pointName, startdate, lastrun, pointMethod, pointDimValue, pointTime, pointType, pointFuzzinessBefore, pointFuzzinessAfter, pointOffset, pointDays);
	}
	
	/*
	function pointToJob(point){
	
		var execFunc = function(job){ print("Custom execute function running"); print("Job: " + job.v.name); return 42; }; //TODO default later
		
		var deviceId = point.deviceRow.deviceId; //not really in use yet
		var pointName = "Job_" + deviceId;
		var lastrun = 0; //TODO
		var startdate = new Date(); //startdate, not in use, always "now"
		var pointDimValue = point.dimvalue;
		var pointMethod = getMethodFromState.callWith(point.state);
		
		var job = getJob.callWith(deviceId, execFunc, pointName, startdate, lastrun, deviceId, pointMethod, pointDimValue); //ERROR here, cannot return this
		print("Jobtest: " + job);
		
		var pointTime = point.absoluteHour * 3600 + point.absoluteMinute * 60;
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
		var pointOffset = point.offset;
		
		var pointDay = point.deviceRow.parent.parent.daydate.getDay(); //different per event
		var event = {};
		
		event.d = {id: deviceId, value: pointDay, fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointType, offset: pointOffset, time: pointTime};
		print("Job: " + job.v.name);
		job.addEvent(event);
		for(var childPoint in point.childPoints){
			event = {};
			pointDay = point.childPoints[childPoint].deviceRow.parent.parent.daydate.getDay(); //different per event
			event.d = {id: deviceId, value: pointDay, fuzzinessBefore: pointFuzzinessBefore, fuzzinessAfter: pointFuzzinessAfter, type: pointType, offset: pointOffset, time: pointTime};
			job.addEvent(event);
		}
		
		//job.addEvent(new Event({id: 0, value: "", fuzzinessBefore: 0, fuzzinessAfter: 0, type: com.telldus.scheduler.EVENTTYPE_ABSOLUTE, offset: 10, time: (new Date().getTime())/1000 + 20}));
		
		return job;
		
		/*
		var jobs = new Array();
		jobs.push(job);
		
		//hm, timerkeys... kommer ju få tillbaka alla på en ggn, eller iaf per device... (om ett addjob/device)
		//tänkte ändå alltid uppdatera alla (dvs ta bort/lägga till) för varje device, om den alls har ändrats...
		//man kunde klart koppla en key till en point, men isf måste man på ngt sätt lagra när den tas bort...
		//och jämför när man lägger till nya (iofs bara kolla om ngn key finns på pointen, annars är den ny)
		//ett jobb/punkt (med flera events, per dag)
		//lägga till alla jobb/device (för alla dagar) på en ggn (om device hasChanged), ta bort alla tidigare...
		//alla timers kommer ju att ha försvunnit vid avstängning, så det behöver man inte bry sig om...
		var timerkeys = com.telldus.scheduler.addJobs(jobs);
		*/
	//}
	
}
