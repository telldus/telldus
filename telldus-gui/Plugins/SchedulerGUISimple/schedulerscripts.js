
function getBarWidth(currentBar, currentPointRect, pointList){
	
	var maxWidth = currentPointRect.parent.width;
	var nextX = maxWidth
	var halfPointWidth = currentPointRect.width / 2
	for(var i=1;i<pointList.length;i++){
		if (pointList[i].isPoint != undefined) {
			if((pointList[i].x + halfPointWidth) < nextX && (pointList[i].x - halfPointWidth) > (currentPointRect.x + halfPointWidth)){
				nextX = pointList[i].x + halfPointWidth;
			}
		}
	}
	
	return nextX - (currentPointRect.x + halfPointWidth);
}
