import QtQuick 1.0
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
			text: qsTr("Name")
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
			text: qsTr("ID")
			anchors.right: modelTitle.left
			visible: main.state == "EDIT"
			width: 50
		}
		HeaderTitle {
			id: modelTitle
			text: qsTr("Model")
			anchors.right: visibleinlistTitle.left
			visible: main.state == "EDIT"
			width: 100
		}
		HeaderTitle {
			id: visibleinlistTitle
			text: qsTr("Visible in list")
			anchors.right: sendtoliveTitle.left
			horizontalAlignment: Text.AlignHCenter
			visible: main.state == "EDIT"
			width: 100
		}
		HeaderTitle {
			id: sendtoliveTitle
			text: qsTr("Send to Telldus Live!")
			anchors.right: sensorinformationTitle.left
			horizontalAlignment: Text.AlignHCenter
			visible: main.state == "EDIT"
			width: 150
		}
		HeaderTitle {
			id: sensorinformationTitle
			text: qsTr("Sensor information")
			width: 150
			anchors.right: timestampTitle.left
		}
		HeaderTitle {
			id: timestampTitle
			text: qsTr("Last updated")
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
			height: 20
			text: main.state == "VIEW" ? qsTr("Edit") : qsTr("View")
			onClicked: {
				if(main.state == "VIEW"){
					main.state = "EDIT"
					sendSensorReport.callWith(1);
				}
				else{
					main.state = "VIEW"
					sendSensorReport.callWith(2);
				}
			}
		}
	}
}
