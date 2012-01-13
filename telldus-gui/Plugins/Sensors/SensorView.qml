import Qt 4.7
import QtDesktop 0.1

Component {
	id: sensorView
	Item{
		id: sensorViewItem
		visible: main.state == "EDIT" || modelData.showInList
		height: childrenRect.height
		width: parent.width

		property string state: main.state
		onStateChanged: {
			if (state != "EDIT") {
				modelData.setName(nameEdit.text)
			}
		}

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
				text: modelData.name == '' ? '<unnamed>' : modelData.name;
				color: "#004275"
			}
			TextField {
				id: nameEdit
				visible: main.state == "EDIT"
				anchors.left: parent.left
				anchors.leftMargin: 15
				anchors.top: parent.top
				anchors.topMargin: (40-nameEdit.height)/2
				text: modelData.name;
				placeholderText: 'Enter a name'
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
			Item  {
				id: visibleinlistcheckbox
				height: 40
				width: 100
				anchors.right: sensorInfo.left
				CheckBox {
					id: checkBox
					anchors.centerIn: parent
					width: checkBox.height
					visible: main.state == "EDIT"
					checked: modelData.showInList
					onClicked: modelData.setShowInList(!modelData.showInList)
				}
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
