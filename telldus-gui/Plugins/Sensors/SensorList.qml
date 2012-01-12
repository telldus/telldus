import Qt 4.7

Column {
	id: sensorList
	spacing: 1

	SensorView {
		id: sensorView
	}

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
