import Qt 4.7

Rectangle{
	id: navButton
	property string arrowText: ""
	signal clicked()
	height: 20
	width: 20
	border.color: "red"
	Text{
		anchors.centerIn: parent
		text: arrowText
	}
	MouseArea{
		anchors.fill: parent
		onClicked: {
			//step to prev weekday
			dialog.hide()
			main.updateEndsWith()
			navButton.clicked()
			main.updateCurrentDay()
		}
	}
}
