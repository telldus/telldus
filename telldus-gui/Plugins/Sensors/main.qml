import Qt 4.7

Item {
	id: main
	state: "VIEW"

	Flickable {
		anchors.fill: parent

		contentHeight: sensorList.height

		SensorList {
			width: parent.width
			id: sensorList
		}
	}
}
