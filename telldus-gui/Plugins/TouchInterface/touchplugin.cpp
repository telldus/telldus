#include <QtCore>
#include <QLabel>
#include "touchplugin.h"
#include "panel.h"

QIcon TouchPlugin::iconForPage( const QString &page ) const {
	Q_UNUSED(page);
	return QIcon(":/images/touchinterface.png");
}

void TouchPlugin::init() {
}

QString TouchPlugin::pluginName() const {
	return "TouchInterface";
}

QWidget *TouchPlugin::widget( const QString &page, QWidget *parent ) const {
	Q_UNUSED(page)
	return new Panel(parent);
}

QStringList TouchPlugin::widgets() const {
	return QStringList() << "touch.default";
}

Q_EXPORT_PLUGIN2(TouchInterface, TouchPlugin)
