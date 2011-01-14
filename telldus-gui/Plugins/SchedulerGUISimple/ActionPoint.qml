import Qt 4.7

Rectangle{
	id: pointRect
	property variant xposition
	//x: xposition
	
	/*
	states: State {
		name: "myState"; when: xposition != undefined
		PropertyChanges { target: pointRect; x: myListView.width - xposition } 
	}
	*/

	width: 30
	height: 50
	border.color: "black"
	opacity: 0.8
	z: 100
	MouseArea {
		onClicked: {
			//pointRect.border.color: "red"
			dialog.show("Id: " + myListView.children[0].width)
		}
		//onPositionChange... maybe emit signal to change in row...
		anchors.fill: parent
		drag.target: pointRect
		drag.axis: Drag.XAxis
		drag.minimumX: 0
		drag.maximumX: actionPoint.width - pointRect.width //TODO this doesn't work
		//TODO make it impossible to overlap (on release)
	}
	
	Column{
		spacing: 10
		Image {
				//opacity: 1
				id: actionImage
				width: 20; height: 20
				source: "/home/stefan/Projects/tellstick/trunk/telldus-gui/TelldusCenter/images/devices.png"
			}
			
			Image {
				//triggerImage, antingen sol upp, sol ned, eller inte bild utan text m. klockslag
				id: triggerImage
				width: 20; height: 20
				source: "/home/stefan/Downloads/11949889941371111141clock_michael_breuer_01.svg.hi.png"
			}
	}
}
