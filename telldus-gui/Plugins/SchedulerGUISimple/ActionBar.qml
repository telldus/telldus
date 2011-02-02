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
	width: barRectangle.prevDayWidth //TODO Scripts.getBarWidth(barRectangle, hangOnToPoint, hangOnToPoint.parent.children); //TEST getNextAndPrevBarWidth before
			

	states: [
		State{
			name: "continuing"
			PropertyChanges {
				target: barRectangle
				anchors.verticalCenter: hangOnToPoint.verticalCenter
				anchors.left: hangOnToPoint.left
				/*color: barRectangle.prevDayColor
				//getPreviousDayColor(barRectangle.currentDay, barRectangle.days, hangOnToPoint.deviceId, barRectangle.deviceIndex)
				opacity: barRectangle.prevDayOpacity
				width: barRectangle.prevDayWidth //TODO Scripts.getBarWidth(barRectangle, hangOnToPoint, hangOnToPoint.parent.children);
				*/
			}
		},
		State{
			name: "continuingWithLimitedWidth"
			PropertyChanges {
				target: barRectangle
				anchors.verticalCenter: hangOnToPoint.verticalCenter
				anchors.left: hangOnToPoint.left
				width: Scripts.getFirstPointWidth(hangOnToPoint)
			}
		},
		State{
			name: "pointLoaded" //; when: hangOnToPoint != undefined && hangOnToPoint.isLoaded != undefined && hangOnToPoint.parent != null && hangOnToPoint.parent != undefined && hangOnToPoint.verticalCenter != undefined  //TODO might aswell use hangOnToPoint != undefined, still get null item warning, used hangOnToPoint.isLoaded too before, remove this?
			PropertyChanges {
				target: barRectangle
				anchors.verticalCenter: hangOnToPoint.verticalCenter
				anchors.left: hangOnToPoint.horizontalCenter
				color: hangOnToPoint.actionTypeColor
				opacity: hangOnToPoint.actionTypeOpacity
				width: Scripts.getBarWidth(barRectangle, hangOnToPoint, hangOnToPoint.parent.children)	
			}
		}
	]  
} 
