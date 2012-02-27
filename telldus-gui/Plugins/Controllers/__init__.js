/** Sensors **/
__setupPackage__( __extension__ );

__postInit__ = function() {
	application.allDoneLoading.connect( com.telldus.controllers.init );
}

com.telldus.controllers = function() {
	var view = null;

	function init() {
		view = new com.telldus.qml.view({});
		view.setProperty('controllerModel', com.telldus.controllers.list);
		view.load("main.qml");
		view.sizeRootObjectToView(true);
		application.configuration.addPage('Controllers', view, 'icon.png');
		com.telldus.controllers.list.changed.connect(application.configuration.valueChanged)
		application.configuration.save.connect(com.telldus.controllers.list.save)
	}

	return { //Public functions
		init:init
	}

}();
