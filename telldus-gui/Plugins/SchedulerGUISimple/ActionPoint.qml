import Qt 4.7

Rectangle{
	id: pointRect
	//property variant xposition
	property string actionType: "blue"
	property string isPoint: "true"
	//x: xposition
	
	/*
	states: State {
		name: "myState"; when: xposition != undefined
		PropertyChanges { target: pointRect; x: myListView.width - xposition } 
	}
	*/
	
	//use item instead of rectangle (no border then though) to make it invisible (opacity: 0)
	width: 30
	height: 50
	border.color: "black"
	opacity: 0.8
	z: 100
	MouseArea {
		onClicked: {
			if(actionType == "red"){
				actionType = "blue"
			}
			else{
				actionType = "red"
			}
			//pointRect.border.color: "red"
			//Fungerar inte: for(var child in myListView.children){
			//	dialog.show("hej?")
			//}
				//listmodel har iaf en count-property man kan testa om vi gör om detta till en listmodel...		
			/*var continue = true;
			while(continue){
				dialog.show("Count: " + parent.parent.children[2]) //myListView.children[0].children[0].x), parent.x = punktens x, parent.parent.children = siblings... starting from 1
				continue = false;
			}
			*/
			//dialog.show("Width: " + Scripts.getBarWidth(pointRect, parent.parent.children));

			dialog.show("Nice dialog with possibility to set type of action, exact time, fuzziness, offset etc") //myListView.children[0].children[0].x), parent.x = punktens x, parent.parent.children = siblings... starting from 1
		}
		//onPositionChange... maybe emit signal to change in row...
		anchors.fill: parent
		drag.target: pointRect
		drag.axis: Drag.XAxis
		drag.minimumX: 0
		drag.maximumX: 685 //TODO make relative!!
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
