#ifndef QMLVIEW_H
#define QMLVIEW_H

#include <QDeclarativeView>
#include <QScriptValue>
#include <QDir>

class QMLView : public QDeclarativeView
{
	Q_OBJECT
public:
	QMLView(const QDir &dir, const QScriptValue &object);
	virtual ~QMLView();

signals:

public slots:
	void load(const QString &url);
	void setProperty( const QString & name, const QScriptValue &value );
	void sizeRootObjectToView(bool enable);

private:
	class PrivateData;
	PrivateData *d;

};

#endif // QMLVIEW_H
