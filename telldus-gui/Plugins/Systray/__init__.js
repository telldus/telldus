/** SYSTRAY **/

__setupPackage__( __extension__ );

__postInit__ = function() {
	application.showMessage.connect( systrayMessage );
	com.telldus.systray.triggered.connect( triggered );
	application.allDoneLoading.connect( allDone );

}

function allDone() {
	var exit = com.telldus.systray.addMenuItem( qsTr("&Quit") );
	com.telldus.systray.menuItem(exit).triggered.connect(application.quit);
}

function systrayMessage(title, message, detailedMessage) {
	if (!application.isMainWindowShown()) {
		com.telldus.systray.showMessage(title, message, detailedMessage);
	}
}

function triggered() {
	application.showMainWindow();
}

