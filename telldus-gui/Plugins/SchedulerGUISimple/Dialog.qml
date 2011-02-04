 import Qt 4.7
 import "schedulerscripts.js" as Scripts

 Rectangle {
	id: container
	property ActionPoint actionPoint
	property alias absoluteHour: inputAbsoluteHourText.text
	property alias absoluteMinute: inputAbsoluteMinuteText.text
	
	focus: true
	Keys.onPressed: {
		
		tryme(event, "container");
		
		var hours = 0;
		var minutes = 0;
		if (event.key == Qt.Key_Left) {
			//actionPoint.x = actionPoint.x - 1
			var minutes = parseInt(inputAbsoluteMinuteText.text, 10);
			var hours = parseInt(inputAbsoluteHourText.text, 10);
			if(minutes == 0){
				if(hours == 0){
					return; //do nothing, border reached
				}
				hours = hours - 1;
				minutes = 59;
			}
			else{
				minutes = minutes - 1;
			}
			event.accepted = true;
		}
		else if (event.key == Qt.Key_Right) {
			//actionPoint.x = actionPoint.x + 1
			var minutes = parseInt(inputAbsoluteMinuteText.text, 10);
			var hours = parseInt(inputAbsoluteHourText.text, 10);
			if(minutes == 59){
				if(hours == 23){
					return; //do nothing, border reached
				}
				hours = hours + 1;
				minutes = 0;
			}
			else{
				minutes = minutes + 1;
			}
			event.accepted = true;
		}
		else{
			return;
		}
		
		inputAbsoluteHourText.text = Scripts.pad(hours, 2);
		inputAbsoluteMinuteText.text = Scripts.pad(minutes, 2);
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
		inputAbsoluteHourText.text = Scripts.pad(actionPoint.absoluteHour, 2)
		inputAbsoluteMinuteText.text = Scripts.pad(actionPoint.absoluteMinute, 2)
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

	Button{
		id: circleType
		text: "Toggle Action Type"
		anchors.verticalCenter: parent.verticalCenter
		anchors.verticalCenterOffset: 0
		anchors.left: typeList.right
		anchors.leftMargin: 50
		onClicked: {
			container.actionPoint.toggleType();
		}
		
		Keys.onLeftPressed: {
			debug("Left pressed"); //TODO make it work
			
		}
	}
	
	Button{
		id: circleTrigger
		text: "Toggle Trigger Type"
		anchors.horizontalCenter: circleType.horizontalCenter
		y: circleType.y + 30
		onClicked: {
			container.actionPoint.toggleTrigger();
		}
	}
	
	Button{
		id: removePoint
		text: "Remove action"
		anchors.horizontalCenter: circleTrigger.horizontalCenter
		y: circleTrigger.y + 30
		onClicked: {
			container.actionPoint.remove();
		}
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
												  
				Keys.onTabPressed: {
					event.accepted = true;
					inputFuzzyAfterText.focus = true;
				}
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
				Keys.onTabPressed: {
					event.accepted = true;
					inputFuzzyBeforeText.focus = true;
				}
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
		id: absolutePanel
		height: 50
		width: 80
		
		opacity: actionPoint.triggerstate == "absolute"
			
		anchors.left: removePoint.right
		anchors.leftMargin: 10
		anchors.top: fuzzyPanel.bottom
		anchors.topMargin: 10
		//property alias absoluteHourText: inputAbsoluteHourText.text
		//property alias absoluteMinuteText: inputAbsoluteMinuteText.text
		
		Row{
			Text{
				/*id: textAbsolute
				anchors.left: parent.left
				anchors.leftMargin: 5
				anchors.top: parent.top
				anchors.topMargin: 5
				*/
				text: "Time:"
			}
			
			Rectangle{
				id: inputAbsoluteHour
				//property alias absoluteHour: inputAbsoluteHourText.text
				/*anchors.left: textOffset.right
				anchors.leftMargin: 5
				anchors.verticalCenter: textOffset.verticalCenter
				*/
				width: 35
				height: inputAbsoluteHourText.height
				border.width: 1
				
				TextInput{
					id: inputAbsoluteHourText
					anchors.fill: parent
					maximumLength: 4
					validator: IntValidator{bottom: 0; top: 23;}
					selectByMouse: true
					color: "#151515"; selectionColor: "mediumseagreen"
					Keys.onTabPressed: {
						event.accepted = true;
						inputAbsoluteMinuteText.focus = true;
					}
					//text: "0"
  					//onChanged:{
					//	print("DIALOG HOUR CHANGED");
					//}

				}
				
				Binding {
					target: actionPoint
					property: "absoluteHour"
					value: inputAbsoluteHourText.text
					when: container.opacity == 1
				}
			}
			
			Text{
				/*id: textOffsetUnit
				anchors.left: inputOffset.right
				anchors.leftMargin: 5
				anchors.verticalCenter: textOffset.verticalCenter
				*/
				text: ":"
			}
			
			Rectangle{
				id: inputAbsoluteMinute
				property alias absoluteMinute: inputAbsoluteMinuteText.text
				/*anchors.left: textOffset.right
				anchors.leftMargin: 5
				anchors.verticalCenter: textOffset.verticalCenter
				*/
				width: 35
				height: inputAbsoluteMinuteText.height
				border.width: 1
				
				TextInput{
					id: inputAbsoluteMinuteText
					anchors.fill: parent
					maximumLength: 4
					validator: IntValidator{bottom: 0; top: 59;}
					selectByMouse: true
					color: "#151515"; selectionColor: "mediumseagreen"
					Keys.onTabPressed: {
						event.accepted = true;
						inputAbsoluteHourText.focus = true;
					}
					text: "0"
				}
				
				Binding {
					target: actionPoint
					property: "absoluteMinute"
					value: inputAbsoluteMinuteText.text
					when: container.opacity == 1
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
	
	Rectangle{
		id: weekDayPanel
		anchors.left: currentType.left
		anchors.top: currentType.bottom
		anchors.topMargin: 40
		height: weekColumn.height
		width: weekColumn.width
		Column{
			id: "weekColumn"
			//anchors.verticalCenter: parent.verticalCenter	
			//anchors.fill: parent
			spacing: 2
			Repeater{
				id: weekRepeater
				model: 7 //actionPoint.daysOfWeek
				//delegate: weekDayDelegate
				Rectangle{
					width: 30
					height: 30
					Image{
						id: tickBox
						height: 14
						width: 14
						source: actionPoint.getTickedImageSource(index)
						MouseArea{
							anchors.fill: parent
							onClicked: {
								actionPoint.toggleTickedWeekDay(index)
							}
						}
					}
					Text{
						//anchors.fill: parent
						anchors.left: tickBox.right
						anchors.leftMargin: 10
						text: Scripts.weekday_name_array[index] //TODO start on monday
					}
				}
			}
		}
	}
	
	Component{
		id: weekDayDelegate
		Rectangle{
			width: 10
			height: 20
			color: "red"
		}
	}
	
	Button{
		id: closeButton
		text: "Close"
		anchors.top: weekDayPanel.bottom
		anchors.horizontalCenter: weekDayPanel.right //horizontalCenter
		//y: circleTrigger.y + 30
		//x: parent.x + 30
		onClicked: {
			hide();
		}
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
