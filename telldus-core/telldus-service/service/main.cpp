
#include <QtCore/QCoreApplication>
#include <QDebug>
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
	TelldusCore::logMessage("Contruct");
}

TelldusService::~TelldusService()
{
	TelldusCore::logMessage("Destruct");
}

void TelldusService::start() {
	QCoreApplication *app = application();

	tc = new TelldusCore();

	logMessage("TService started", QtServiceBase::Error);
	TelldusCore::logMessage("Started");
	//app->quit();

}

void TelldusService::stop() {
	delete tc;
	tc = 0;
	TelldusCore::logMessage("Stopped");
}

void TelldusService::pause() {
	TelldusCore::logMessage("Pause");
}

void TelldusService::resume() {
	TelldusCore::logMessage("Resume");
}

void TelldusService::processCommand(int code) {
}

int main(int argc, char **argv) {
    TelldusService service(argc, argv);
	TelldusCore::logMessage("Main");
    return service.exec();
}
