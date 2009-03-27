
__postInit__ = function() {
	application.showMessage.connect( systrayMessage );
}

function systrayMessage(title, message, detailedMessage) {
	if (!application.isMainWindowShown()) {
		com.telldus.systray.showMessage(title, message, detailedMessage);
	}
}
