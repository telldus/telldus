import Qt 4.7

Rectangle{
	id: pointRect
	//property variant xposition
	property string actionTypeColor: "blue" //TODO default value
	property int actionType: 1 //TODO default value
	property double actionTypeOpacity: 1
	property string isPoint: "true"
	property variant isLoaded
							 
	Component.onCompleted: {
		//TODO useless really, still gets Cannot anchor to a null item-warning...
		isLoaded = "true"
	}
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
	state: "on"
	//actionTypeColor: getColor()
	
	MouseArea {
		id: pointRectMouseArea
		onClicked: {
			dialog.show(pointRect) //TODO om inte redan i visandes läge....
		}
		//onPositionChange... maybe emit signal to change in row...
		anchors.fill: parent
		drag.target: pointRect
		drag.axis: Drag.XAxis
		drag.minimumX: 0
		drag.maximumX: 685 //TODO make relative!!
		//TODO make it impossible to overlap (on release)
		//onPositionChanged: { value = (maximum - minimum) * (handle.x-2) / slider.xMax + minimum; }
	}
	
	Column{
		spacing: 10
		Image {
			//opacity: 1
			id: actionImage
			width: 20; height: 20
			source: "/home/stefan/Projects/tellstick/trunk/telldus-gui/TelldusCenter/images/devices.png"
		}
		
		Rectangle{
			id: trigger
			
			state: "absolute"
			width: 20; height: 20
			
			//TODO state should move the point to correct place... (sunrisetime, sunsettime or absolute (stored value, the one that is dragged)
			//drag not permitted depending on state...
			states: [
				State {
					name: "sunrise"
					PropertyChanges { target: triggerImage; source: "/home/stefan/Downloads/sunrise.png" } //TODO: images!!
					PropertyChanges { target: pointRectMouseArea; drag.target: undefined }
				},
				State {
					name: "sunset"
					PropertyChanges { target: triggerImage; source: "/home/stefan/Downloads/sunset.png" } //TODO: images!!
					PropertyChanges { target: pointRectMouseArea; drag.target: undefined }
				},
				State {
					name: "absolute"
					PropertyChanges { target: triggerImage; source: "/home/stefan/Downloads/11949889941371111141clock_michael_breuer_01.svg.hi.png" } //TODO: images!!
					PropertyChanges { target: pointRectMouseArea; drag.target: parent }
				}
			]
			
			Image {
				//triggerImage, antingen sol upp, sol ned, eller inte bild utan text m. klockslag
				id: triggerImage
				anchors.fill: parent
				width: 20; height: 20
				source: "/home/stefan/Downloads/11949889941371111141clock_michael_breuer_01.svg.hi.png"
			}
		}
	}
	
	states: [
		State {
			name: "on"
			PropertyChanges { target: pointRect; actionTypeColor: "blue"; actionTypeOpacity: 1 } //TODO: images!!
			PropertyChanges { target: actionImage; source: "/home/stefan/Projects/tellstick/trunk/telldus-gui/TelldusCenter/images/devices.png" }
		},
		State{
			name: "off"
			PropertyChanges { target: pointRect; actionTypeColor: "gainsboro"; actionTypeOpacity: 0 }
			PropertyChanges { target: actionImage; source: "/home/stefan/Projects/tellstick/trunk/telldus-gui/TelldusCenter/images/devices-bw.png" }
		},
		State{
			name: "dim"
			PropertyChanges { target: pointRect; actionTypeColor: "green"; actionTypeOpacity: 1 }
			PropertyChanges { target: actionImage; source: "/home/stefan/Projects/tellstick/trunk/telldus-gui/TelldusCenter/images/TelldusCenter_128.png" }
			//something opacity = dim for example
		},
		State{
			name: "bell"
			PropertyChanges { target: pointRect; actionTypeColor: getLastPointColor() }
			PropertyChanges { target: actionImage; source: "icon.png" }
		}
	]
	
	function toggleType(){ //TODO other kind of selection method
		print(pointRect.state);
		if(pointRect.state == "on"){
			pointRect.state = "off"
		}
		else if(pointRect.state == "off"){
			pointRect.state = "dim"
		}
		else if(pointRect.state == "dim"){
			pointRect.state = "bell"
		}
		else if(pointRect.state == "bell"){
			pointRect.state = "on"
		}
	}
	
	function toggleTrigger(){ //TODO other kind of selection method
		print(trigger.state);
		if(trigger.state == "sunrise"){
			trigger.state = "sunset";
		}
		else if(trigger.state == "sunset"){
			trigger.state = "absolute";
		}
		else if(trigger.state == "absolute"){
			trigger.state = "sunrise";
		}
	}
	
	function getLastPointColor(){
		//get previous point:
		var prevPoint = null;
		var pointList = pointRect.parent.children;
		for(var i=1;i<pointList.length;i++){
			if (pointList[i].isPoint != undefined && pointList[i] != pointRect) {
				if(pointList[i].x < pointRect.x && (prevPoint == null || pointList[i].x > prevPoint.x)){
					prevPoint = pointList[i];
				}
			}
		}
		
		//TODO Binding loop here when moving transperent point over other point
		if(prevPoint == null || prevPoint.actionTypeOpacity == 0){
			//no point before, no bar after either
			actionTypeOpacity = 0
			return "papayawhip" //just return a color, will not be used
		}
		
		actionTypeOpacity = 1
		return prevPoint.actionTypeColor
	}
}
