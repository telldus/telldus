 import Qt 4.7

 Rectangle {
     id: container

     function show(text) {
         dialogText.text = text;
         container.opacity = 1;
		 container.border.color = "black"
		 container.border.width = 2
     }

     function hide() {
         container.opacity = 0;
		 container.border.width = 0
     }

	 smooth: true
	 radius: 5
     width: dialogText.width + 120
     height: dialogText.height + 120
     opacity: 0

     Text {
         id: dialogText
         anchors.centerIn: parent
         text: ""
     }

     MouseArea {
         anchors.fill: parent
         onClicked: hide();
     }
 } 
