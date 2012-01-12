import Qt 4.7

Component {
	id: sensorView
	Item{
		id: sensorViewItem
		visible: main.state == "EDIT" || modelData.showInList
		height: childrenRect.height
		width: parent.width

		BorderImage {
			source: "row_bg.png"
			border.left: 5; border.top: 5
			border.right: 5; border.bottom: 5
			height: sensorInfo.height
			width: parent.width

			Text {
				visible: main.state == "VIEW"
				anchors.left: parent.left
				anchors.leftMargin: 15
				height: 40
				verticalAlignment: Text.AlignVCenter
				text: modelData.name;
				color: "#004275"
			}
			Rectangle{
				color: "white"
				visible: main.state == "EDIT"
				anchors.left: parent.left
				anchors.leftMargin: 15
				width: nameEdit.width + 4
				height: 22
				TextInput{
					id: nameEdit
					anchors.centerIn: parent
					text: modelData.name;
					color: "#004275"

					onActiveFocusChanged: {
						if(!activeFocus){
							//todo other way?
							modelData.setName(nameEdit.text);
						}
					}
					onAccepted: {
						modelData.setName(nameEdit.text);
					}
				}
			}
			Text{
				anchors.right: model.left
				visible: main.state == "EDIT"
				height: 40
				verticalAlignment: Text.AlignVCenter
				text: modelData.id
				color: "#004275"
				width: 50
			}
			Text{
				id: model
				anchors.right: visibleinlistcheckbox.left
				visible: main.state == "EDIT"
				height: 40
				verticalAlignment: Text.AlignVCenter
				text: modelData.model
				color: "#004275"
				width: 100
			}
			Item{
				id: visibleinlistcheckbox
				anchors.right: sensorInfo.left
				visible: main.state == "EDIT"
				height: 40
				Rectangle{
					anchors.centerIn: parent
					height: 10
					width: 10
					color: "white"
					Text{
						anchors.centerIn: parent
						color: "#004275"
						text: modelData.showInList ? "X" : ""
					}
					MouseArea{
						anchors.fill: parent
						onClicked: {
							modelData.setShowInList(!modelData.showInList);
						}
					}
				}
				width: 100
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
}
