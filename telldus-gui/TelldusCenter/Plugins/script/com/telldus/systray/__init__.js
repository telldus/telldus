
__postInit__ = function() {
	application.showMessage.connect( systrayMessage );
	com.telldus.systray.triggered.connect( triggered );
}

function systrayMessage(title, message, detailedMessage) {
	if (!application.isMainWindowShown()) {
		com.telldus.systray.showMessage(title, message, detailedMessage);
	}
}

function triggered() {
	application.showMainWindow();
}