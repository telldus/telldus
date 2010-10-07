#include "TelldusWinService_win.h"
//#include <QCoreApplication>

#include <windows.h>
#include <Dbt.h>


int main(int argc, char **argv) {
	g_argc = argc;
	g_argv = argv;

	SERVICE_TABLE_ENTRY serviceTable[] = {
		{serviceName, TelldusWinService::serviceMain },
        { 0, 0 }
    };

    StartServiceCtrlDispatcher( serviceTable );

	return 0;
}
