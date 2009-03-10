#include <QtCore>
#include <QLabel>
#include "touchplugin.h"

QIcon TouchPlugin::iconForPage( const QString &page ) const {
	return QIcon(":/images/touchinterface.png");
}

QString TouchPlugin::pluginName() const {
	return "TouchInterface";
}

QWidget *TouchPlugin::widget( const QString &page, QWidget *parent ) const {
	return new QLabel("Hello world", parent);
}

QStringList TouchPlugin::widgets() const {
	return QStringList() << "touch.default";
}

Q_EXPORT_PLUGIN2(TouchInterface, TouchPlugin)
