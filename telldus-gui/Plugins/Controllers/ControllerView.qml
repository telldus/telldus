import QtQuick 1.1
import QtDesktop 0.1

BorderImage {
	source: "row_bg.png"
	border.left: 5; border.top: 5
	border.right: 5; border.bottom: 5
	width: parent.width
	height: content.height + content.anchors.margins*2

	Item {
		id: content
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.margins: 5
		height: childrenRect.height
		width: childrenRect.width

		Row {
			spacing: 10
			Image {
				source: icon(controller.type)
				width: 50
				smooth: true
				fillMode: Image.PreserveAspectFit
				opacity: controller.available ? 1 : 0.5
			}

			Column {
				Text {
					color: "#004275"
					text: productName(controller.type)
					font.pixelSize: 15
				}
				TextField {
					//id: nameEdit
					text: controller.name;
					placeholderText: 'Enter a name for this controller'
					onTextChanged: controller.name = text
				}
			}
			Loader {
				sourceComponent: tellstick
			}
			Image {
				source: "btn_action_remove.png"
				visible: !controller.available
				MouseArea {
					anchors.fill: parent
					onClicked: controller.tryRemove();
				}
			}
		}
	}

	Component {
		id: tellstick
		Grid {
			spacing: 3
			columns: 2
			Text {
				color: "#004275"
				text: "Serial:"
			}
			Text {
				color: "#004275"
				text: controller.serial
			}
			Text {
				color: "#004275"
				text: "Firmware version:"
			}
			Text {
				color: "#004275"
				text: controller.firmware
			}
		}
	}

	function icon(type) {
		if (type == 1) {
			return "tellstick.png";
		} else if (type == 2) {
			return "tellstick_duo.png";
		}
		return "tellstick.png";
	}
	function productName(type) {
		if (type == 1) {
			return "TellStick";
		} else if (type == 2) {
			return "TellStick Duo";
		}
		return "";
	}
}
