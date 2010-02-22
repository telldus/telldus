
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QFile>
#include "qtservice.h"

#include "TelldusCore.h"

class TelldusService : public QtService<QCoreApplication> {
public:
    TelldusService(int argc, char **argv);
    ~TelldusService();

protected:

    void start();
    void stop();
    void pause();
    void resume();
    void processCommand(int code);

private:
	TelldusCore *tc;
};

TelldusService::TelldusService(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "TelldusCore"),
	tc(0)
{
    setServiceDescription("A Telldus service for managing TellStick Duo.");
}

TelldusService::~TelldusService()
{
}

void TelldusService::start() {
	QCoreApplication *app = application();

	QFile file("C:/log.txt");
	file.open(QIODevice::Text | QIODevice::WriteOnly);
	file.close();

	tc = new TelldusCore();

	logMessage("TService started", QtServiceBase::Error);
	//app->quit();

}

void TelldusService::stop() {
	delete tc;
	tc = 0;
}

void TelldusService::pause() {
}

void TelldusService::resume() {
}

void TelldusService::processCommand(int code) {
}

int main(int argc, char **argv) {
    TelldusService service(argc, argv);
    return service.exec();
}
