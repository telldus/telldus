#include "settings.h"
#include <QSettings>
#include <QDebug>

class Settings::PrivateData {
public:
	QString extension;
};

Settings::Settings(const QString &extensionName) :
	QObject()
{
	d = new PrivateData;
	d->extension = extensionName;
}

Settings::~Settings() {
	delete d;
}

void Settings::setValue( const QString & key, const QVariant & value ) {
	QSettings s;
	s.beginGroup(d->extension);
	s.setValue(key, value);
}

QVariant Settings::value( const QString &key, const QVariant &defaultValue ) const {
	QSettings s;
	s.beginGroup(d->extension);
	return s.value(key, defaultValue);
}
