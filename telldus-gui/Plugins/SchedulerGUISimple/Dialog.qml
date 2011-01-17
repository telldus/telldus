 import Qt 4.7

 Rectangle {
     id: container
     property ActionPoint actionPoint

     function show(actionPoint) {
         dialogText.text = "Nice dialog with possibility to set type of action, exact time, fuzziness, offset etc"
         container.opacity = 1;
		 container.border.color = "black"
		 container.border.width = 2
		 container.actionPoint = actionPoint
     }

     function hide() {
         container.opacity = 0;
		 container.border.width = 0
     }
     
	 smooth: true
	 radius: 5
     width: dialogText.width + 120
     height: dialogText.height + 220
     opacity: 0

     Text {
         id: dialogText
         anchors.centerIn: parent
         text: ""
     }
     
     Rectangle {  
		id: circleType //TODO only types this device has...
		height: 20
		width: 100
		anchors.verticalCenter: parent.verticalCenter
		anchors.verticalCenterOffset: 0

		property color buttonColor: "lightgrey"

		Text{
			text: "Toggle type"
			anchors.verticalCenter: parent.verticalCenter
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.margins: 2

		}
		MouseArea {
			id: buttonMouseAreaType
			anchors.fill: parent
			hoverEnabled: true
			//onEntered: parent.border.color = onHoverColor
			//onExited:  parent.border.color = borderColor
			onClicked: {
				container.actionPoint.toggleType();
			}
		}

		color: buttonMouseAreaType.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
	}
     
     Rectangle {  //TODO create common button-class
		id: closeButton
		height: 20
		width: 100
		anchors.horizontalCenter: circleType.right
		anchors.horizontalCenterOffset: 30

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
			hoverEnabled: true
			//onEntered: parent.border.color = onHoverColor
			//onExited:  parent.border.color = borderColor
			onClicked: {
				hide();
			}
		}

		color: buttonMouseAreaClose.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
	}
 } 
