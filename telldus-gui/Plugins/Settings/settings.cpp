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
	d->s.remove(key);

	if (value.type() == QVariant::List) {
		QVariantList list = value.toList();
		d->s.beginGroup(key);
		d->s.setValue("size", list.size());
		d->s.setValue("type", "array");
		for (int i = 0; i < list.size(); ++i) {
			d->s.setArrayIndex(i);
			this->setValue(QString::number(i), list.at(i));
		}
		d->s.endGroup();

	} else if (value.type() == QVariant::Map) {
		QVariantMap list = value.toMap();
		d->s.beginGroup(key);
		d->s.setValue("type", "dict");
		d->s.beginGroup("values");
		QVariantMap::const_iterator it = list.constBegin();
		for (; it != list.constEnd(); ++it) {
			this->setValue(it.key(), it.value());
		}
		d->s.endGroup();
		d->s.endGroup();

	} else {
		d->s.setValue(key, value);
	}
}

QVariant Settings::value( const QString &key, const QVariant &defaultValue ) const {
	//First, find out if it is an array
	if (!d->s.childGroups().contains(key)) {
		//Normal value
		return d->s.value(key, defaultValue);
	}

	QVariant retval = QVariant::Invalid;

	//Find out the type
	d->s.beginGroup(key);
	QString type = d->s.value("type").toString();
	if (type == "dict") {
		QVariantMap dict;
		d->s.beginGroup("values");
		QStringList keys;
		keys << d->s.childGroups() << d->s.childKeys();
		foreach(QString key, keys) {
			dict[key] = this->value(key);
		}

		d->s.endGroup();
		retval = dict;
	} else {
		QVariantList list;
		int size = d->s.value("size", 0).toInt();
		for (int i = 0; i < size; ++i) {
			list.append(this->value(QString::number(i)));
		}
		retval = list;
	}
	d->s.endGroup();

	return retval;
}
