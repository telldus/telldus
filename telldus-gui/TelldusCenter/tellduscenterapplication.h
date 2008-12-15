#ifndef TELLDUSCENTERAPPLICATION_H
#define TELLDUSCENTERAPPLICATION_H

#include <QApplication>
#include <QSystemTrayIcon>

class TelldusCenterApplication : public QApplication
{
	Q_OBJECT
public:
	TelldusCenterApplication(int &argc, char **argv);
	virtual ~TelldusCenterApplication();

	static TelldusCenterApplication *instance();

private:
	QSystemTrayIcon systrayIcon;
};

#endif // TELLDUSCENTERAPPLICATION_H
