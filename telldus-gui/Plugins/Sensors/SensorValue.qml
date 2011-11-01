import Qt 4.7

Item {
	property alias text: text.text
	property alias icon: icon.source
	property date lastUpdated: new Date()

	id: sensorValue
	width: parent.width
	height: 40
	Image {
		id: icon
		anchors.verticalCenter: parent.verticalCenter
	}
	Text {
		id: text
		font.weight: Font.Bold
		anchors.left: icon.right
		anchors.leftMargin: 10
		anchors.verticalCenter: parent.verticalCenter
		color: "#004275"
	}
	Text {
		id: timestamp
		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.verticalCenter: parent.verticalCenter
		text: Qt.formatDateTime(lastUpdated, Qt.DefaultLocaleShortDate);
		color: "#004275"
		font.pointSize: text.font.pointSize - 1
		font.italic: true
	}
}
