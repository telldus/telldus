import Qt 4.7
import QtDesktop 0.1

Item{
	id: sensorViewItem
	visible: state == "EDIT" || modelData.showInList
	height: childrenRect.height
	width: parent ? parent.width : 0

	states:[
		State {
			name: "EDIT"
		}
	]

	BorderImage {
		source: "row_bg.png"
		border.left: 5; border.top: 5
		border.right: 5; border.bottom: 5
		height: sensorInfo.height
		width: parent.width

		Text {
			visible: sensorViewItem.state != "EDIT"
			anchors.left: parent.left
			anchors.leftMargin: 15
			height: 40
			verticalAlignment: Text.AlignVCenter
			text: modelData.name == '' ? qsTr('<unnamed>') : modelData.name;
			color: "#004275"
		}
		TextField {
			id: nameEdit
			visible: sensorViewItem.state == "EDIT"
			anchors.left: parent.left
			anchors.leftMargin: 15
			anchors.top: parent.top
			anchors.topMargin: (40-nameEdit.height)/2
			text: modelData.name;
			placeholderText: qsTr('Enter a name')
			onTextChanged: modelData.name = text
		}
		Item {
			height: 40
			width: deleteImg.width
			anchors.right: sensorid.left
			anchors.rightMargin: 15
			visible: sensorViewItem.state == "EDIT"
			Image {
				id: deleteImg
				anchors.centerIn: parent
				source: "btn_action_remove.png"
				MouseArea{
					anchors.fill: parent
					onClicked: {
						main.focus = true
						confirmDeletion.visible = true;
					}
				}
			}
		}
		Text{
			id: sensorid
			anchors.right: model.left
			visible: sensorViewItem.state == "EDIT"
			height: 40
			verticalAlignment: Text.AlignVCenter
			text: modelData.id
			color: "#004275"
			width: 50
		}
		Text{
			id: model
			anchors.right: visibleinlistcheckbox.left
			visible: sensorViewItem.state == "EDIT"
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
			anchors.right: sendtolivecheckbox.left
			CheckBox {
				id: checkBox
				anchors.centerIn: parent
				width: checkBox.height
				visible: sensorViewItem.state == "EDIT"
				checked: modelData.showInList
				onClicked: modelData.setShowInList(!modelData.showInList)
			}
		}
		Item  {
			id: sendtolivecheckbox
			height: 40
			width:  showLiveOptions ? 150 : 0
			anchors.right: sensorInfo.left
			CheckBox {
				id: checkBoxSTL
				anchors.centerIn: parent
				width: checkBoxSTL.height
				visible: sensorViewItem.state == "EDIT" && showLiveOptions
				checked: modelData.sendToLive
				onClicked: modelData.setSendToLive(!modelData.sendToLive)
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
			SensorValue {
				visible: modelData.hasRainRate
				text: visible ? modelData.sensorValue(4).value + ' mm/h' : ''
				icon: "icon_rain.png"
				lastUpdated: visible ? modelData.sensorValue(4).lastUpdated : new Date()
			}
			SensorValue {
				visible: modelData.hasRainTotal
				text: visible ? modelData.sensorValue(8).value + ' mm' : ''
				icon: "icon_rain.png"
				lastUpdated: visible ? modelData.sensorValue(8).lastUpdated : new Date()
			}
			SensorValue {
				visible: modelData.hasWindDirection
				text: visible ? getWindDirection.callWith(modelData.sensorValue(16).value): ''
				icon: "icon_wind.png"
				lastUpdated: visible ? modelData.sensorValue(16).lastUpdated : new Date()
			}
			SensorValue {
				visible: modelData.hasWindAverage
				text: visible ? modelData.sensorValue(32).value + ' m/s' : ''
				icon: "icon_wind.png"
				lastUpdated: visible ? modelData.sensorValue(32).lastUpdated : new Date()
			}
			SensorValue {
				visible: modelData.hasWindGust
				text: visible ? '(' + modelData.sensorValue(64).value + ' m/s)' : ''
				icon: "icon_wind.png"
				lastUpdated: visible ? modelData.sensorValue(64).lastUpdated : new Date()
			}
		}
	}

	Dialog{
		id: confirmDeletion
		modal: true
		title: qsTr("Confirm deletion")
		Text{
			id: descriptionHeadline
			text: qsTr("Delete this sensor?")
			font.bold: true
		}
		Text{
			id: descriptionText
			anchors.top: descriptionHeadline.bottom
			anchors.topMargin: 10
			width: parent.width - 20
			anchors.left: parent.left
			anchors.leftMargin: 10
			text: qsTr("Please note that a sensor that is still transmitting will reappear here again, but it will be hidden in the list by default.")
			wrapMode: Text.Wrap
		}

		onAccepted: {
			deleteSensor.callWith(modelData.protocol, modelData.model, modelData.id);
		}
	}
}
