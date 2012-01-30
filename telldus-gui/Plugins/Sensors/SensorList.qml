import Qt 4.7
import QtDesktop 0.1

Column {
	id: sensorList
	property int minimumWidth: main.state == "EDIT" ? 770 : 475
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
			text: ""
			anchors.right: sensorid.left
			visible: main.state == "EDIT"
			width: 50
		}
		HeaderTitle {
			id: sensorid
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
			horizontalAlignment: Text.AlignHCenter
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
		delegate: SensorView{ state: main.state == "EDIT" ? 'EDIT' : ''}
	}
	Row{
		spacing: 20
		Button {
			width: 50
			height: 20
			text: main.state == "VIEW" ? "Edit" : "View"
			onClicked: {
				if(main.state == "VIEW"){
					main.state = "EDIT"
				}
				else{
					main.state = "VIEW"
				}
			}
		}
	}
}
