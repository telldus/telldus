import QtQuick 1.1

//import QtDesktop 0.1

Item {
	width: 500 //Minimum width

	Column {
		spacing: 1
		anchors.fill: parent

		BorderImage {
			id: header
			source: "header_bg.png"
			width: parent.width; height: 40
			border.left: 5; border.top: 5
			border.right: 5; border.bottom: 5

			HeaderTitle {
				text: "Controllers"
				anchors.left: parent.left
				anchors.leftMargin: 15
			}
		}
		Repeater {
			model: controllerModel
			delegate: ControllerView {}
		}
	}
}
