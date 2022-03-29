#include "includes.h"
#include "loginmanager.h"
#include "mainwindow.h"
#include "loginwindow.h"
#include "resetwindow.h"
#include "httprequest.h"
#include "debugwindow.h"
#include "privilegeescalation.h"
#include "installverifier.h"
#include "errorbox.h"

LOGGER_STRUCT logging;

int main(int argc, char *argv[])
{
    //FreeConsole(); //Uncomment in release
    PrivilegeEscalation(INVALID_HANDLE_VALUE,"SeDebugPrivilege",TRUE);
    QApplication a(argc, argv);
    logging.debug = new debugWindow;
    std::stringstream mainStream;
    mainStream << &main;
    logging.debug->logMessage("Entry point: " + mainStream.str());
    installVerifier mainVerifier;
    bool installed = mainVerifier.verifyInstall();
    if(!installed)
    {
        ErrorBox("Install corrupt, some components are missing, check debug log for more information, Components will be attempted to be installed");
        mainVerifier.repairInstall();
    }
    loginTransfer* login = new loginTransfer();
    return a.exec();
}
