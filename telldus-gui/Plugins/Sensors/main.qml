import Qt 4.7

Item {
	id: main
	state: "VIEW"

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
				text: "ID"
				anchors.right: modelTitle.left
				visible: main.state == "EDIT"
				width: 50
			}
			HeaderTitle {
				id: modelTitle
				text: "Model"
				anchors.right: visibleinlistTitle.left
				visible: main.state == "EDIT"
				width: 100
			}
			HeaderTitle {
				id: visibleinlistTitle
				text: "Visible in list"
				anchors.right: sensorinformationTitle.left
				visible: main.state == "EDIT"
				width: 100
			}
			HeaderTitle {
				id: sensorinformationTitle
				text: "Sensor information"
				width: 150
				anchors.right: timestampTitle.left
			}
			HeaderTitle {
				id: timestampTitle
				text: "Last updated"
				width: 100
				anchors.right: parent.right
				//horizontalAlignment: Text.AlignRight
			}
		}
		Repeater {
			model: sensorModel
			delegate: sensorView
		}
		Row{
			spacing: 20
			Rectangle {
				width: 50
				height: 20
				Text{
					anchors.centerIn: parent
					text: main.state == "VIEW" ? "Edit" : "View"
				}
				MouseArea{
					anchors.fill: parent
					onClicked: {
						if(main.state == "VIEW"){
							main.state = "EDIT"
						}
						else{
							main.state  ="VIEW"
						}
					}
				}
			}
			/*
			Rectangle {
				//TODO should this button exist at all, or always save?
				width: 50
				height: 20
				visible: main.state == "EDIT"
				Text{
					anchors.centerIn: parent
					text: "Cancel"
				}
				MouseArea{
					anchors.fill: parent
					onClicked: {
						main.state  ="VIEW"
					}
				}
			}
			*/
		}
		anchors.fill: parent
	}
}
