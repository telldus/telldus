import QtQuick 1.0
import QtDesktop 0.1

Item {
	id: main
	state: initialViewMode

	ScrollArea {
		id: scrollArea
		anchors.fill: parent
		frame: false

		contentHeight: sensorList.height
		contentWidth: sensorList.width

		SensorList {
			id: sensorList
			property int calculatedWidth: main.width-scrollArea.verticalScrollBar.width
			width: (calculatedWidth < minimumWidth ? minimumWidth : calculatedWidth)
		}
	}
}
