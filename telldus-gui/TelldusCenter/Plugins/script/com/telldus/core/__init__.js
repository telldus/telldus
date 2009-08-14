/** TELLDUS CORE **/
__setupPackage__( __extension__ );

__postInit__ = function() {
	com.telldus.core.errorOccurred.connect( errorOccurred );
}

function errorOccurred( deviceId, errorId, errorString ) {
	application.showMessage("", errorString, "");
}
