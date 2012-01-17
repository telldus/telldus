import Qt 4.7
import QtDesktop 0.1

Item {
	id: main
	state: "VIEW"

	ScrollArea {
		id: scrollArea
		anchors.fill: parent
		frame: false

		contentHeight: sensorList.height
		contentWidth: sensorList.width

		SensorList {
			id: sensorList
			width: main.width-scrollArea.verticalScrollBar.width
		}
	}
}
