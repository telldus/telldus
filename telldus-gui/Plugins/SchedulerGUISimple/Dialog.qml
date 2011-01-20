 import Qt 4.7

 Rectangle {
	id: container
	property ActionPoint actionPoint
	 
	Component.onCompleted: {
		//create default actionPoint, to avoid null values and binding errors
		var component = Qt.createComponent("ActionPoint.qml")
		var dynamicPoint = component.createObject(container)
		dynamicPoint.opacity = 0
		dynamicPoint.width = 0
		dynamicPoint.height = 0
		container.actionPoint = dynamicPoint
	}
	
	/*
	states: State{
		name: "visible"; when: actionPoint.fuzzyAfter != undefined
		PropertyChanges{
			target: container; fuzzyAfter: actionPoint.fuzzyAfter
		}
	}
	*/
     
	function show(actionPoint) {
		container.opacity = 1;
		container.border.color = "black"
		container.border.width = 2
		container.actionPoint = actionPoint
		
		var rootCoordinates = actionPoint.mapToItem(null, actionPoint.x, actionPoint.y);
		//container.x = rootCoordinates.x + actionPoint.parent.width/2 - container.width/2;
		container.y = rootCoordinates.y + actionPoint.height + 10;
		container.width = actionPoint.parent.width;
		container.x = (actionPoint.parent.parent.width - container.width)/2;
	}

     function hide() {
         container.opacity = 0;
		 container.border.width = 0
     }
     
	 smooth: true
	 radius: 5
     //width: 500 //TODO
     height: 500 // typelist.height * 2 + 50
     opacity: 0

     Rectangle {  
		id: circleType //TODO only types this device has...
		height: 20
		width: 100
		anchors.verticalCenter: parent.verticalCenter
		anchors.verticalCenterOffset: 0
		anchors.left: typeList.right
		anchors.leftMargin: 50

		property color buttonColor: "lightgrey"

		Keys.onLeftPressed: {
			debug("Left pressed"); //TODO make it work
			
		}							
		
		Text{
			text: "Toggle Action Type"
			anchors.verticalCenter: parent.verticalCenter
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.margins: 2
		}
		
		MouseArea {
			id: buttonMouseAreaType
			anchors.fill: parent
			//hoverEnabled: true
			//onEntered: parent.border.color = onHoverColor
			//onExited:  parent.border.color = borderColor
			onClicked: {
				container.actionPoint.toggleType();
			}
		}

		color: buttonMouseAreaType.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
	}
	
	Rectangle {  
		id: circleTrigger
		height: 20
		width: 100
		anchors.horizontalCenter: circleType.horizontalCenter
		y: circleType.y + 30
		
		property color buttonColor: "lightgrey"

		Text{
			text: "Toggle Trigger Type"
			anchors.verticalCenter: parent.verticalCenter
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.margins: 2

		}
		MouseArea {
			id: buttonMouseAreaTrigger
			anchors.fill: parent
			//hoverEnabled: true
			//onEntered: parent.border.color = onHoverColor
			//onExited:  parent.border.color = borderColor
			onClicked: {
				container.actionPoint.toggleTrigger();
			}
		}

		color: buttonMouseAreaTrigger.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
	}
	
	Rectangle {  
		id: removePoint
		height: 20
		width: 100
		anchors.horizontalCenter: circleTrigger.horizontalCenter
		y: circleTrigger.y + 30
		
		property color buttonColor: "lightgrey"

		Text{
			text: "Remove action"
			anchors.verticalCenter: parent.verticalCenter
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.margins: 2

		}
		MouseArea {
			id: buttonMouseAreaRemovePoint
			anchors.fill: parent
			//hoverEnabled: true
			//onEntered: parent.border.color = onHoverColor
			//onExited:  parent.border.color = borderColor
			onClicked: {
				container.actionPoint.remove();
			}
		}

		color: buttonMouseAreaRemovePoint.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
	}
	
	Rectangle{
		id: fuzzyPanel
		height: 100
		width: 80
		
		anchors.left: removePoint.right
		anchors.leftMargin: 10
		anchors.top: parent.top
		anchors.topMargin: 10
		
		Text{
			id: textFuzzyBefore
			anchors.left: parent.left
			anchors.leftMargin: 5
			anchors.top: parent.top
			anchors.topMargin: 5
			text: "Fuzzy before:"
		}
		
		Text{
			id: textFuzzyBeforeUnit
			anchors.left: inputFuzzyBefore.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textFuzzyBefore.verticalCenter
			text: "minutes"
		}
		
		Text{
			id: textFuzzyAfter
			anchors.left: parent.left
			anchors.leftMargin: 5
			anchors.top: textFuzzyBefore.bottom
			anchors.topMargin: 5
			text: "Fuzzy after:"
		}
		
		Text{
			id: textFuzzyAfterUnit
			anchors.left: inputFuzzyAfter.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textFuzzyAfter.verticalCenter
			text: "minutes"
		}
		
		Rectangle{
			id: inputFuzzyBefore
			anchors.left: textFuzzyBefore.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textFuzzyBefore.verticalCenter
			width: 35
			height: textFuzzyBefore.height
			border.width: 1
			
			TextInput{
				id: inputFuzzyBeforeText
				anchors.fill: parent
				maximumLength: 5
				selectByMouse: true
				color: "#151515"; selectionColor: "mediumseagreen"
				text: "0" //container.actionPoint.fuzzyBefore
			}
		}
		
		Rectangle{
			id: inputFuzzyAfter
			anchors.left: textFuzzyAfter.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textFuzzyAfter.verticalCenter
			width: 35
			height: textFuzzyAfter.height
			border.width: 1
			
			TextInput{
				id: inputFuzzyAfterText
				anchors.fill: parent
				maximumLength: 5
				selectByMouse: true
				color: "#151515"; selectionColor: "mediumseagreen"
				text: actionPoint.fuzzyAfter
			}
			
			Binding {
				target: actionPoint
				property: "fuzzyAfter"
				value: inputFuzzyAfterText.text
			}
		}
		
		Image{
			//TODO turn into component?
			anchors.left: textFuzzyBeforeUnit.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textFuzzyBeforeUnit.verticalCenter
			
			source: "icon.png" //TODO info-icon
			
			MouseArea{
				anchors.fill: parent
				hoverEnabled: true
				onEntered: {
					infobox.opacity = 1
					infobox.infoboxtext = "Enter a value indicating how many minutes before the set value that the action may be executed. The action will be executed at a random time within the interval."
				}
				onExited: {
					infobox.opacity = 0
				}
			}
		}
		
		Image{
			//TODO turn into component?
			anchors.left: textFuzzyAfterUnit.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textFuzzyAfterUnit.verticalCenter
			
			source: "icon.png" //TODO info-icon
			
			MouseArea{
				anchors.fill: parent
				hoverEnabled: true
				onEntered: {
					infobox.opacity = 1
					infobox.infoboxtext = "Enter a value indicating how many minutes after the set value that the action may be executed. The action will be executed at a random time within the interval."
				}
				onExited: {
					infobox.opacity = 0
				}
			}
		}
		
		Rectangle{
			id: infobox
			property alias infoboxtext: infoboxtext.text
			opacity: 0
			color: "antiquewhite"
			width: infoboxtext.width + 4
			height: infoboxtext.height + 4
			border.color: "black"
			border.width: 1
			Text{
				id: infoboxtext
				anchors.centerIn: parent
				width: 200
				wrapMode: Text.WordWrap
				text: ""
			}
		}
	}
	
	Rectangle {  //TODO create common button-class (but how to differentiate action?)
		id: closeButton
		height: 20
		width: 100
		//anchors.horizontalCenter: circleTrigger.right
		//anchors.horizontalCenterOffset: 30
		y: circleTrigger.y + 30

		property color buttonColor: "lightgrey"

		Text{
			text: "Close"
			anchors.verticalCenter: parent.verticalCenter
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.margins: 2

		}
		MouseArea {
			id: buttonMouseAreaClose
			anchors.fill: parent
			//hoverEnabled: true
			//onEntered: parent.border.color = onHoverColor
			//onExited:  parent.border.color = borderColor
			onClicked: {
				hide();
			}
		}

		color: buttonMouseAreaClose.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
	}
	
	ListView{
		id: typeList
		anchors.top: parent.top
		anchors.topMargin: 20
		anchors.left: currentType.right
		anchors.leftMargin: 10
		width: 100 //TODO relative
		height: 100 //TODO relative
		//anchors.fill: parent
		model: typeSelection
		delegate: typeSelectionRow
		highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
	
		//TODO can these paths be turned into some kind of constants? Import from common file or something?
		ListModel{
			id: typeSelection
			ListElement{
				name: "on"
				imagesource: "/home/stefan/Projects/tellstick/trunk/telldus-gui/TelldusCenter/images/devices.png"
			}
			ListElement{
				name: "off"
				imagesource: "/home/stefan/Projects/tellstick/trunk/telldus-gui/TelldusCenter/images/devices-bw.png"
			}
			ListElement{
				name: "dim"
				imagesource: "/home/stefan/Projects/tellstick/trunk/telldus-gui/TelldusCenter/images/TelldusCenter_128.png"
			}
			ListElement{
				name: "bell"
				imagesource: "icon.png"
			}
		}
	}
	
	Rectangle{
		id: currentType
		anchors.top: parent.top
		anchors.topMargin: 20
		anchors.left: parent.left
		anchors.leftMargin: 10
		border.color: "grey"
		border.width: 1
		width: 120
		height: 120
		Image{
			anchors.fill: parent
			id: mainImage
			source: actionPoint.actionTypeImage
			/*states: State {
				name: "typeLoaded"; when: actionPoint.actionTypeImage != undefined
				PropertyChanges { target: mainImage
					source: actionPoint.actionTypeImage
				}
			}
			*/
		}
	}
	
	Component{
		id: typeSelectionRow
		Row{
			
			Rectangle{
				border.color: "grey"
				border.width: 2
				anchors.leftMargin: 10
				width: 30; height: 30
				Image{
					anchors.fill: parent
					//anchors.left: typeSelectionText.right
					//anchors.leftMargin: 10
					//anchors.centerIn: parent
					source: imagesource
					
					MouseArea{
						anchors.fill: parent
						onClicked: {
							//pointRect.focus = true
							container.actionPoint.setType(name)
							//typeList.highlight = name
						}
					}
				}
			}
		}
	}
	
 } 
