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
					width: parent.width
					height: constDeviceRowHeight
					border.color: "green"
					Text{
						anchors.centerIn: parent
						text: modelData.name
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
				
				MouseArea {
					id: deviceMouseArea
					anchors.fill: parent
					//hoverEnabled: true

					//TODO ändra muspekaren kanske?
					//onEntered: parent.border.color = onHoverColor
					//onExited:  parent.border.color = borderColor
					onClicked: {
						print("ENDPOINT: " + deviceRow.endPoint);
						
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
						
						dialog.show(dynamicPoint) 
					}
				}
				
				ActionBar{
					id: "continuingBar"
					hangOnToPoint: deviceRow
						
					state: "continuing"
				}
				
				Item{
				//TODO eller ska detta VARA början-rektanglen?
					id: previousDay
					states: State{
						name: "isFirst"; when: daydate > getPreviousDayDate()
						PropertyChanges{ target: dayListView; endsWith: "NÅGOT" }
					}
				}

				ListView.onAdd: SequentialAnimation {
					PropertyAction { target: mainRow; property: "height"; value: 0 }
					NumberAnimation { target: mainRow; property: "height"; to: 50; duration: 250; easing.type: Easing.InOutQuad }
				}
				
				function hasPoints(){
					return Scripts.hasPoints(deviceRow.children);
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
	//opacity vid dimning?
	//linjens färg etc (state) beror ju på närmaste punkt föres sort... Punkten kan finnas osynlig (tidigare dag) också...
	//kan man liksom göra hela linjen (från en vecka tillbaka) men inte visa den? Om det är vettigt... Då hade man tom kunnat zooma en vacker dag
	//properties, ställa in dem...
	//fuzziness
}
