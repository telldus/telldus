import Qt 4.7

Item {
	id: main

	Column {
		Repeater {
			model: sensorModel
			delegate: Row {
				spacing: 10
				Text { text: modelData.name }
				Text { text: modelData.protocol }
				Text { text: modelData.model }
				Text { text: modelData.temperature + '°C'; visible: modelData.hasTemperature }
				Text { text: modelData.humidity + "%"; visible: modelData.hasHumidity }
			}
		}
		anchors.fill: parent
	}
}
