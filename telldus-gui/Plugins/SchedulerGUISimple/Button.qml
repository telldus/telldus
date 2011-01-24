import Qt 4.7

Rectangle {
	id: buttonRect
	height: 20
	width: buttonText.width + 20
	
	property color buttonColor: "lightgrey"
	property alias text: buttonText.text
	signal clicked()
								
	Text{
		id: buttonText
		anchors.verticalCenter: parent.verticalCenter
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.margins: 2

	}
	MouseArea {
		id: buttonMouseArea
		anchors.fill: parent
		onClicked: buttonRect.clicked()
	}

	color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
} 
