//import QtQuick 1.0 TODO not until 4.7.1
import Qt 4.7


 
 Item{
	id: main
	width: 800 //TODO how?
	height: 600 //TODO how?
	
		//height: 200
	
	/*
	Rectangle {
		width:200
		height: 200
		id: testRectangle
		border.color: "red"
		//anchors.fill: parent
		color: "blue"
		MouseArea {
			anchors.fill: parent
			//onClicked: container.clicked(testGrid.)
		}
	} 
	 
	 
	Text{ 
		id: testItem
		text: "Hello world!"
		//anchors.left: main.left
		anchors.centerIn: parent
		anchors.verticalCenter: main.verticalCenter
	}
	
	Column {
         id: testColumn
         width: parent.width
		 height: parent.height
         //anchors.bottom: page.bottom; anchors.bottomMargin: 4
         //rows: 6; columns: 1; spacing: 3

		Repeater{
			model: 3
			Row{
				id: row1
				width: parent.width;
				height: 50
				//color: "red"
				Rectangle { border.color: "blue"; width: 100; height:parent.height; 
					Text{
						anchors.verticalCenter: parent.verticalCenter
						anchors.horizontalCenter: parent.horizontalCenter
						text: "Device " + index
					}
				}
				Rectangle { border.color: "blue"; width: parent.width-100; height:parent.height; }
			}
		}
	
        Rectangle { color: "green"; width: parent.width; height:50; id: row2 }
         Rectangle { color: "blue";  }
         Rectangle { color: "yellow";  }
         Rectangle { color: "steelblue";  }
         Rectangle { color: "black";  }
     }
     */
     
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
				
					text: devicename + height
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
  
  ListModel {
		id: myModel
		ListElement {
			devicename: "Storsalen"
		}
		ListElement {
			devicename: "Förstugan"
		}
		ListElement {
			devicename: "Västra uthuset"
		}
	}
  
	ListView {
		id: myListView
		anchors.fill: parent
		model: myModel
		delegate: listRow
		//snapMode: ListView.SnapToItem
		//highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
		focus: true
		footer: addButtonComponent
		header: timeline
     }
     
     Component{
		id: timeline
		Item{
			width: parent.width
			Rectangle{
				width: parent.width
				height: 30
				border.color: "red"
				anchors.verticalCenter: parent.verticalCenter
				
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
						myModel.append({
							"devicename": "Stället"
						})
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
			width: 100 //TODO dependent on hangOnToPoint and next point positions
			height: 10
			color: "blue" //TODO, dependent on hangOnToPoint
			anchors.verticalCenter: hangOnToPoint.verticalCenter
			anchors.left: hangOnToPoint.horizontalCenter
			
			z: 110	   
			/* couldnt get this to work:   
			 * (if it works later on, try to set opacity for actionPoint in this way too)
			states: State {
				name: "myState"; when: hangOnToPoint != undefined
				PropertyChanges { target: barRectangle; anchors.verticalCenter: hangOnToPoint.verticalCenter; anchors.left: hangOnToPoint.horizontalCenter } 
				//anchors.verticalCenter: hangOnToPoint.verticalCenter
				//anchors.left: hangOnToPoint.horizontalCenter
			}
			*/
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
