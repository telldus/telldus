import Qt 4.7

Item {
	id: main

	Component {
		id: sensorView
		BorderImage {
			source: "row_bg.png"
			border.left: 5; border.top: 5
			border.right: 5; border.bottom: 5
			height: sensorInfo.height
			width: parent.width

			Text {
				anchors.left: parent.left
				anchors.leftMargin: 15
				height: 40
				verticalAlignment: Text.AlignVCenter
				text: modelData.name;
				color: "#004275"
			}
			Column {
				id: sensorInfo
				anchors.right: parent.right
				width: 250
				SensorValue {
					visible: modelData.hasTemperature
					text: visible ? modelData.sensorValue(1).value + '°C' : ''
					icon: "icon_temp.png"
					lastUpdated: visible ? modelData.sensorValue(1).lastUpdated : new Date()
				}
				SensorValue {
					visible: modelData.hasHumidity
					text: visible ? modelData.sensorValue(2).value + '%' : ''
					icon: "icon_humidity.png"
					lastUpdated: visible ? modelData.sensorValue(2).lastUpdated : new Date()
				}
			}
		}
	}

	Column {
		spacing: 1
		BorderImage {
			id: header
			source: "header_bg.png"
			width: parent.width; height: 40
			border.left: 5; border.top: 5
			border.right: 5; border.bottom: 5

			HeaderTitle {
				text: "Name"
				anchors.left: parent.left
				anchors.leftMargin: 15
			}
			HeaderTitle {
				text: "Sensor information"
				width: 150
				anchors.right: timestampTitle.left
			}
			HeaderTitle {
				id: timestampTitle
				text: "Last updated"
				width: 100
				anchors.right: parent.right
			}
		}
		Repeater {
			model: sensorModel
			delegate: sensorView
		}
		anchors.fill: parent
	}
}
