import QtQuick 1.1
import QtDesktop 0.1

Window {
	id: controllerUpgradeDialog
	width: 400
	height: 200

	modal: true
	title: "Upgrade TellStick"
	visible: false
	deleteOnClose: false

	property int step: controller.upgradeStep
	property variant controller: undefined
	Connections {
		target: controller
		onUpgradeDone: state = 'upgradeDone'
	}

	Item {
		id: confirm
		anchors.top: parent.top
		anchors.left: parent.left
		width: parent.width
		height: parent.height
		clip: true
		Item {
			anchors.fill: parent
			anchors.margins: 16

			Column {
				Text { text: "This will upgrade your TellStick to the latest version.\n\n" }
				Text { text: "Please do not unplug the TellStick or end the application during upgrade." }
				Text { text: "Doing so might render the TellStick unusable." }
			}
			Row {
				anchors.bottom: parent.bottom
				anchors.right: parent.right
				spacing: 8
				Button {
					text: "Upgrade"
					onClicked: controllerUpgradeDialog.state = 'upgrade'
				}
				Button {
					text: "Close"
					onClicked: controllerUpgradeDialog.visible = false
				}
			}
		}
	}

	Item {
		id: content
		anchors.topMargin:16
		anchors.margins: 16
		anchors.top: parent.top
		anchors.left: parent.right
		width: parent.width - 32
		height: parent.height - 32
		anchors.right: undefined
		clip: true
		Column {
			StateLabel { text: "1. Disconnecting TellStick"; currentState: controllerUpgradeDialog.step; state: 0 }
			StateLabel { text: "2. Aquiring TellStick"; currentState: controllerUpgradeDialog.step; state: 1 }
			StateLabel { text: "3. Entering bootloader"; currentState: controllerUpgradeDialog.step; state: 2 }
			StateLabel { text: "4. Uploading firmware"; currentState: controllerUpgradeDialog.step; state: 3 }
			StateLabel { text: "5. Rebooting TellStick"; currentState: controllerUpgradeDialog.step; state: 4 }
			StateLabel { text: "6. Connecting TellStick"; currentState: controllerUpgradeDialog.step; state: 5 }
		}
		ProgressBar {
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			minimumValue: 0
			maximumValue: 100
			value: controller.upgradeProgress
		}
	}
	Item {
		id: done
		anchors.top: parent.top
		anchors.left: parent.right
		width: parent.width
		height: parent.height
		clip: true
		Item {
			anchors.fill: parent
			anchors.margins: 16

			Column {
				Text { text: "Upgrade done.\n\n" }
				Text { text: "Your TellStick has now been flashed with the latest firmware." }
			}
			Button {
				anchors.bottom: parent.bottom
				anchors.right: parent.right
				text: "Close"
				onClicked: controllerUpgradeDialog.visible = false
			}
		}
	}

	states: [
		State {
			name: 'upgrade'
			AnchorChanges { target: confirm; anchors.left: undefined; anchors.right: controllerUpgradeDialog.left }
			AnchorChanges { target: content; anchors.left: controllerUpgradeDialog.left; anchors.right: undefined }
			StateChangeScript { script: controller.upgrade() }
		},
		State {
			name: 'upgradeDone'
			AnchorChanges { target: confirm; anchors.left: undefined; anchors.right: controllerUpgradeDialog.left }
			AnchorChanges { target: content; anchors.left: undefined; anchors.right: controllerUpgradeDialog.left }
			AnchorChanges { target: done; anchors.left: controllerUpgradeDialog.left; anchors.right: undefined }
		}
	]

	transitions: [
		Transition {
			AnchorAnimation { duration: 500; easing.type: Easing.InOutQuad }
		}
	]
}
