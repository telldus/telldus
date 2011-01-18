
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

function pad(number, length) {
   
    var str = '' + number;
    while (str.length < length) {
        str = '0' + str;
    }
   
    return str;
}

function isMidnightDark(){
	if(main.sunData == undefined){
		main.sunData = getSunData.call();
	}
	
	var sunrise = main.sunData[0].split(':');;
	var sunset = main.sunData[1].split(':');
	
	if(sunset[0] < sunrise[0] || (sunset[0] == sunrise[0] && sunset[1] < sunrise[1])){
		print("Rise: " + sunrise);
		print("Set: " + sunset);
		return false;
	}
	return true;
}
