import Qt 4.7
import "schedulerscripts.js" as Scripts

Rectangle{
	id: barRectangle
	property variant hangOnToPoint
	property color prevDayColor: "white" //default
	property double prevDayOpacity: 0 //default
	property int prevDayWidth: 0 //default
	property string firstBar: "false"
	
	height: constBarHeight
	z: 110

	color: barRectangle.prevDayColor
	opacity: barRectangle.prevDayOpacity
	width: barRectangle.prevDayWidth
			
	onStateChanged: {
		//cannot bind these values as state change properties for some reason (will generate anchor-to-null-item warnings), do it here instead
		if(state == "pointLoaded"){
			anchors.verticalCenter = hangOnToPoint.verticalCenter;
			anchors.left = hangOnToPoint.horizontalCenter;
		}
		else if(state == "continuingWithLimitedWidth"){
			anchors.verticalCenter = hangOnToPoint.verticalCenter;
			anchors.left = hangOnToPoint.left;
		}
		else if(state == "continuing"){
			anchors.verticalCenter = hangOnToPoint.verticalCenter
			anchors.left = hangOnToPoint.left
		}
	}
	
	states: [
		State{
			name: "continuingWithLimitedWidth"
			PropertyChanges {
				target: barRectangle
				width: Scripts.getFirstPointWidth(hangOnToPoint)
			}
		},
		State{
			name: "pointLoaded"
			PropertyChanges {
				target: barRectangle
				color: hangOnToPoint.actionTypeColor
				opacity: hangOnToPoint.actionTypeOpacity
				width: hangOnToPoint.parent == null ? 0 : Scripts.getBarWidth(barRectangle, hangOnToPoint, hangOnToPoint.parent.children)	
			}
		}
	]  
} 
