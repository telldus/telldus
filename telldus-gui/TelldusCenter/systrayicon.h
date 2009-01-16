#ifndef SYSTRAYICON_H
#define SYSTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>

class SystrayIcon : public QSystemTrayIcon
{
	Q_OBJECT
public:
	SystrayIcon( QObject * parent = 0 );

signals:
	void showEventMessage( const QString &title, const QString &message, const QString &detailedMessage );

public slots:
	void eventTriggered( const QString &name, const QString &title );

private slots:
//	void activated( QSystemTrayIcon::ActivationReason reason );
	void updateSystrayMenu();
	void addDevice( int id );

private:
	QMenu systrayMenu;
};

#endif // SYSTRAYICON_H
