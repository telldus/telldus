#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QObject
{
	Q_OBJECT
public:
	explicit Settings(const QString &extensionName);
	~Settings();

signals:

public slots:
	void setValue( const QString & key, const QVariant & value );
	QVariant value( const QString & key, const QVariant & defaultValue = QVariant() ) const;

private:
	class PrivateData;
	PrivateData *d;

};

#endif // SETTINGS_H
