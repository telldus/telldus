#include "settings.h"
#include <QSettings>
#include <QStringList>

class Settings::PrivateData {
public:
	QSettings s;
};

Settings::Settings(const QString &extensionName) :
	QObject()
{
	d = new PrivateData;
	d->s.beginGroup(extensionName);
}

Settings::~Settings() {
	delete d;
}

void Settings::setValue( const QString & key, const QVariant & value ) {
	if (value.type() == QVariant::List) {
		QVariantList list = value.toList();
		d->s.beginWriteArray(key);
		for (int i = 0; i < list.size(); ++i) {
			d->s.setArrayIndex(i);
			this->setValue(QString::number(i), list.at(i));
		}
		d->s.endArray();

	} else {
		d->s.setValue(key, value);
	}
}

QVariant Settings::value( const QString &key, const QVariant &defaultValue ) const {
	//First, find out if it is an array
	if (d->s.childGroups().contains(key)) {
		//Array or dict
		QVariantList list;
		int size = d->s.beginReadArray(key);
		for (int i = 0; i < size; ++i) {
			d->s.setArrayIndex(i);
			list.append(this->value(QString::number(i)));
		}
		d->s.endArray();
		return list;
	}
	return d->s.value(key, defaultValue);
}
