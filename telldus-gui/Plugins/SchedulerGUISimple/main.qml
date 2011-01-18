//import QtQuick 1.0 TODO not until 4.7.1
import Qt 4.7
import "schedulerscripts.js" as Scripts

 Item{
	id: main
	width: 800 //TODO how?
	height: 600 //TODO how?

	Component{
		id: listRow
		
		Row{
			id: row1
			
			width: parent.width; //TODO relative
			height: 50
			//color: "red"
			Rectangle { border.color: "red"; width: 100; height:parent.height;
				Text{
					anchors.verticalCenter: parent.verticalCenter
					anchors.horizontalCenter: parent.horizontalCenter
					//text: "Device " + (index + 1) + "Name: " + name

					text: modelData.name + height
				}
			}
			Rectangle { id: "deviceRow"; border.color: "blue"; width: parent.width-100; height:parent.height;

				MouseArea {
					id: deviceMouseArea
					anchors.fill: parent
					hoverEnabled: true

					//TODO ändra muspekaren kanske?
					//onEntered: parent.border.color = onHoverColor
					//onExited:  parent.border.color = borderColor
					onClicked: {
						//ny point här

						var component = Qt.createComponent("ActionPoint.qml")
						var dynamicPoint = component.createObject(deviceRow)
						//dynamicPoint.x = mouseX (blir inte kvar)
					//dynamicPoint.
						dynamicPoint.x = mouseX - dynamicPoint.width/2 //xposition
						//dynamicPoint.width = mouseX-
						dynamicPoint.border.color = "blue"

						var dynamicBar = actionBar.createObject(deviceRow)
						dynamicBar.hangOnToPoint = dynamicPoint
						
						dialog.show(dynamicPoint) 
						//dynamicBar.width = Scripts.getBarWidth(dynamicPoint, deviceRow.children)
						//dynamicBar.color = "blue" //TODO dependent of point type
						//dynamicBar.anchors.left = dynamicPoint.right
						//dynamicBar.width = 100 //TODO dependent of this and next point position

						//deviceRow.add(point)
						//TODO destroy? (only to remove them if needed)
						//TODO komponenter med stor bokstav kanske?
						//om detta inte fungerar, testa med pathview...
					}
				}

			}

			ListView.onAdd: SequentialAnimation {
				PropertyAction { target: row1; property: "height"; value: 0 }
				NumberAnimation { target: row1; property: "height"; to: 50; duration: 250; easing.type: Easing.InOutQuad }
			}
		}
	 }

	ListView {
		id: myListView
		anchors.fill: parent
		model: deviceModel
		delegate: listRow
		//focus: true
		footer: addButtonComponent
		header: timeline
	 }

	 Component{
		id: timeline
		Item{
			width: parent.width
			height: 15
			Rectangle{
				width: parent.width
				border.color: "red"

				Row{
					//x: 100
					anchors.horizontalCenter: parent.horizontalCenter
					anchors.horizontalCenterOffset: parent.width/24

					width: listRow.width
					height: listRow.height
					spacing: (parent.width-124)/24
					Repeater{
						model:24
						Rectangle{
							width: 1
							height: 15
							color: "green"
						}
					}
				}
			}
		}
	 }

	 Component{
		 id: addButtonComponent
		 //anchors { left: myListView.left; top: myListView.top; margins: 20 }
		Row {
			id: addButton
			//anchors.fill: parent
			//spacing: 10

			Rectangle {
				height: 20
				width: 100
				anchors.verticalCenter: parent.verticalCenter
				anchors.verticalCenterOffset: 30

				property color buttonColor: "lightgrey"

				Text{
					text: "Add an item"
					anchors.verticalCenter: parent.verticalCenter
					anchors.horizontalCenter: parent.horizontalCenter
					anchors.margins: 2

				}
				MouseArea {
					id: buttonMouseArea
					anchors.fill: parent
					hoverEnabled: true
					//onEntered: parent.border.color = onHoverColor
					//onExited:  parent.border.color = borderColor
					onClicked: {
						addDevice.call()
					}
				}

				color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor

			}
		}
	 }

	Component{
		id: actionBar
		Rectangle{
			id: barRectangle
			property variant hangOnToPoint
			//width: 100 //TODO dependent on hangOnToPoint and next point positions

			height: 10
			/*
			color: hangOnToPoint.actionType //"blue" //TODO, dependent on hangOnToPoint
			anchors.verticalCenter: hangOnToPoint.verticalCenter
			anchors.left: hangOnToPoint.horizontalCenter
			*/
			z: 110

			//state actionType
			// couldnt get this to work:
			// (if it works later on, try to set opacity for actionPoint in this way too)
			states: State {
				name: "myState"; when: hangOnToPoint.isLoaded != undefined && hangOnToPoint.verticalCenter != undefined  //TODO might aswell use hangOnToPoint != undefined, still get null item warning
				PropertyChanges {
					target: barRectangle
					anchors.verticalCenter: hangOnToPoint.verticalCenter
					anchors.left: hangOnToPoint.horizontalCenter
					color: hangOnToPoint.actionTypeColor
					opacity: hangOnToPoint.actionTypeOpacity
					width: Scripts.getBarWidth(actionBar, hangOnToPoint, hangOnToPoint.parent.children)
				}
				//anchors.verticalCenter: hangOnToPoint.verticalCenter
				//anchors.left: hangOnToPoint.horizontalCenter
			}

		}
	}

	Dialog {
		id: dialog
		anchors.centerIn: parent
		z: 150
	}

	//opacity vid dimning?
	//linjens färg etc (state) beror ju på närmaste punkt föres sort... Punkten kan finnas osynlig (tidigare dag) också...
	//kan man liksom göra hela linjen (från en vecka tillbaka) men inte visa den? Om det är vettigt... Då hade man tom kunnat zooma en vacker dag
	//properties, ställa in dem...
	//fuzziness
	/*
	 *  Dialog {
		 id: dialog
		 anchors.centerIn: parent
		 z: 100
	 }
	 */
}
