import Qt 4.7

Image{
	property string infotext: ""
	anchors.leftMargin: 5
	
	source: imageInfo
	width: 15
	height: 15
	z: 200
	
	MouseArea{
		anchors.fill: parent
		hoverEnabled: true
		onEntered: {
			infobox.opacity = 1
			infobox.infoboxtext = parent.infotext
			infobox.anchors.top = parent.bottom
			infobox.anchors.horizontalCenter = parent.horizontalCenter
		}
		onExited: {
			infobox.opacity = 0
		}
	}

	Rectangle{
		id: infobox
		property alias infoboxtext: infoboxtext.text
		opacity: 0
		color: "antiquewhite"
		width: infoboxtext.width + 4
		height: infoboxtext.height + 4
		border.color: "black"
		border.width: 1
		z: 200
		Text{
			id: infoboxtext
			anchors.centerIn: parent
			width: 200
			wrapMode: Text.WordWrap
			text: ""
		}
	}
}