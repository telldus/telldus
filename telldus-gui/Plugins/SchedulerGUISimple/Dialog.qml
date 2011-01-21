 import Qt 4.7

 Rectangle {
	id: container
	property ActionPoint actionPoint
	
	focus: true
	Keys.onPressed: {
		if (event.key == Qt.Key_Left) {
			actionPoint.x = actionPoint.x - 1
			event.accepted = true;
		}
		else if (event.key == Qt.Key_Right) {
			actionPoint.x = actionPoint.x + 1
			event.accepted = true;
		}
		tryme(event, "container");
	}
	 
	MouseArea{
		anchors.fill: parent
	}
	
	Component.onCompleted: {
		//create default actionPoint, to avoid null values and binding errors
		var component = Qt.createComponent("ActionPoint.qml")
		var dynamicPoint = component.createObject(container)
		dynamicPoint.opacity = 0  //TODO why not use "visible"? Test that...
		dynamicPoint.width = 0
		dynamicPoint.height = 0
		container.actionPoint = dynamicPoint
	}
	
	function show(actionPoint) {
		container.opacity = 0; //needed for fuzz/offset unbinding
		container.border.color = "black"
		container.border.width = 2
		inputFuzzyBeforeText.text = actionPoint.fuzzyBefore
		inputFuzzyAfterText.text = actionPoint.fuzzyAfter
		inputOffsetText.text = actionPoint.offset
		inputDimText.text = actionPoint.dimvalue
		container.actionPoint = actionPoint
		
		var rootCoordinates = actionPoint.mapToItem(null, actionPoint.x, actionPoint.y);
		container.y = rootCoordinates.y + actionPoint.height + 10;
		container.width = actionPoint.parent.width;
		container.x = (actionPoint.parent.parent.width - container.width)/2;
		container.opacity = 1;
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
				validator: IntValidator{bottom: 0; top: 10080;} //0 to a week...
				selectByMouse: true
				color: "#151515"; selectionColor: "mediumseagreen"
				//text: actionPoint.fuzzyBefore
			}
			
			Binding {
				target: actionPoint
				property: "fuzzyBefore"
				value: inputFuzzyBeforeText.text
				when: container.opacity == 1
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
				validator: IntValidator{bottom: 0; top: 10080;} //0 to a week...
				selectByMouse: true
				color: "#151515"; selectionColor: "mediumseagreen"
			}
			
			Binding {
				target: actionPoint
				property: "fuzzyAfter"
				value: inputFuzzyAfterText.text
				when: container.opacity == 1
			}
		}
		
		Image{
			//TODO turn into component?
			anchors.left: textFuzzyBeforeUnit.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textFuzzyBeforeUnit.verticalCenter
			
			source: imageInfo
			width: 15
			height: 15
			
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
			
			source: imageInfo
			width: 15
			height: 15
			
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
	
	Rectangle{
		id: offsetPanel
		height: 50
		width: 80
		
		opacity: actionPoint.triggerstate == "sunrise" || actionPoint.triggerstate == "sunset"
			
		anchors.left: removePoint.right
		anchors.leftMargin: 10
		anchors.top: fuzzyPanel.bottom
		anchors.topMargin: 10
		
		Text{
			id: textOffset
			anchors.left: parent.left
			anchors.leftMargin: 5
			anchors.top: parent.top
			anchors.topMargin: 5
			text: "Offset:"
		}
		
		Text{
			id: textOffsetUnit
			anchors.left: inputOffset.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textOffset.verticalCenter
			text: "minutes"
		}
		
		Rectangle{
			id: inputOffset
			property alias offsetText: inputOffsetText.text
			anchors.left: textOffset.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textOffset.verticalCenter
			width: 35
			height: textOffset.height
			border.width: 1
			
			TextInput{
				id: inputOffsetText
				anchors.fill: parent
				maximumLength: 4
				validator: IntValidator{bottom: getMinimumOffset(actionPoint.triggerstate); top: getMaximumOffset(actionPoint.triggerstate);} // +/- 12 hours
				selectByMouse: true
				color: "#151515"; selectionColor: "mediumseagreen"
			}
			
			Binding {
				target: actionPoint
				property: "offset"
				value: inputOffsetText.text
				when: container.opacity == 1
			}
		}
		
		Image{
			anchors.left: textOffsetUnit.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textOffsetUnit.verticalCenter
			
			source: imageInfo
			width: 15
			height: 15
			
			MouseArea{
				anchors.fill: parent
				hoverEnabled: true
				onEntered: {
					infobox.opacity = 1
					infobox.infoboxtext = "Enter a positive or negative value for how many minutes before or after sunset/sunrise this action will be executed."
				}
				onExited: {
					infobox.opacity = 0
				}
			}
		}
	}
	
	Rectangle{
		id: dimPanel
		height: 50
		width: 80
		
		opacity: actionPoint.state == "dim"
			
		anchors.left: removePoint.right
		anchors.leftMargin: 10
		anchors.top: offsetPanel.bottom
		anchors.topMargin: 10
		
		Text{
			id: textDim
			anchors.left: parent.left
			anchors.leftMargin: 5
			anchors.top: parent.top
			anchors.topMargin: 5
			text: "Dim value:"
		}
		
		Text{
			id: textDimUnit
			anchors.left: inputDim.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textDim.verticalCenter
			text: "%"
		}
		
		Rectangle{
			id: inputDim
			anchors.left: textDim.right
			anchors.leftMargin: 5
			anchors.verticalCenter: textDim.verticalCenter
			width: 35
			height: textDim.height
			border.width: 1
			
			TextInput{
				id: inputDimText
				anchors.fill: parent
				maximumLength: 4
				validator: IntValidator{bottom: 0; top: 100;}
				selectByMouse: true
				color: "#151515"; selectionColor: "mediumseagreen"
			}
			
			Binding {
				target: actionPoint
				property: "dimvalue"
				value: inputDimText.text
				when: container.opacity == 1
			}
		}
		
		Image{
			anchors.left: inputDim.right
			anchors.leftMargin: 5
			anchors.verticalCenter: inputDim.verticalCenter
			
			source: imageInfo
			width: 15
			height: 15
			
			MouseArea{
				anchors.fill: parent
				hoverEnabled: true
				onEntered: {
					infobox.opacity = 1
					infobox.infoboxtext = "Enter a dim percent value between 0 (no light) and 100 (full light)"
				}
				onExited: {
					infobox.opacity = 0
				}
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
		model: typeSelection
		delegate: typeSelectionRow
		highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
		snapMode: ListView.SnapToItem
		interactive: false
	
		//TODO can these paths be turned into some kind of constants? Import from common file or something?
		ListModel{
			id: typeSelection
			ListElement{
				name: "on"
				imagesource: "on.png" //TODO cannot use javascript properties here... do in some other way, maybe a list with names here?
			}
			ListElement{
				name: "off"
				imagesource: "off.png"
			}
			ListElement{
				name: "dim"
				imagesource: "dim.png"
			}
			ListElement{
				name: "bell"
				imagesource: "bell.png"
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
	
	function tryme(event, origin){
		print("KEY: " + event.key + " Från: " + origin);
	}
	
	function getMinimumOffset(state){
		//TODO this will not work if a value is set to the highest limit, then not reentering scheduler until the sunrise/set has changed so much that the device will be hidden anyway... Not common though
		var minutes = 0;
		var time;
		if(state == "sunrise"){
			time = main.sunData[0].split(':');
		}
		else if(state == "sunset"){
			time = main.sunData[1].split(':');
		}
		else{
			return 0 ;
		}
		
		return -1 * (parseInt(time[0], 10) * 60 + parseInt(time[1], 10));
	}
	
	function getMaximumOffset(state){
		var minutes = 0;
		var time;
		if(state == "sunrise"){
			time = main.sunData[0].split(':');
		}
		else if(state == "sunset"){
			time = main.sunData[1].split(':');
		}
		else{
			return 0 ;
		}
		
		return 24 * 60 - (parseInt(time[0], 10) * 60 + parseInt(time[1], 10));
	}
 } 
