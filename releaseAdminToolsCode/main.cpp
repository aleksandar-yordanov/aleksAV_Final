#include "includes.h"
#include "input.h"
#include "networkingmanager.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Aleks-AV Administration Panel:");
    QCoreApplication::setApplicationVersion("Beta 0.8");

    QCommandLineParser parser;
    parser.setApplicationDescription("Command Line management utility for managing users and uploading signatures");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption username(QStringList() << "u" << "username",
                                QCoreApplication::translate("main","Administrator Username"),
                                QCoreApplication::translate("main","username"));
    QCommandLineOption password(QStringList() << "p" << "password",
                                QCoreApplication::translate("main","Administrator Password"),
                                QCoreApplication::translate("main","password"));
    QCommandLineOption upload("upload-file",
                              QCoreApplication::translate("main","Threat to upload to database"),
                              QCoreApplication::translate("main","file"));

    parser.addOption(username);
    parser.addOption(password);
    parser.addOption(upload);

    parser.process(a);
    if(!(parser.isSet(username) && parser.isSet(password)))
    {
        std::cout << "Args: Login arguments not set, Please enter a username and password with -u and -p" << std::endl;
        std::this_thread::sleep_for(1000ms);
        return 0;
    }

    networkingManager mainManager(parser.value(username).toStdString(),parser.value(password).toStdString());
    input mainInput;
    if(!(parser.isSet(upload)))
    {
        bool loginState = mainManager.login();
        if(loginState)
        {
#ifndef _WIN32
            std::cout << "\e[1;1H\e[2J";
#else
            system("cls");
#endif
            int x = mainInput.mainInput();
            if(x == 1)
            {
                std::string username;
                std::string password;
                std::cout << "Enter username for password to change:";
                std::cin >> username;
                std::cout << "Enter password to change:";
                std::cin >> password;
                mainManager.changeUserPassword(username,password);
            }
            else if(x == 2)
            {
                std::string username;
                std::cout << "Enter users licence to change:";
                std::cin >> username;
                int license = mainInput.licenseInput();
                mainManager.changeLicense(networkingManager::mapIntToLicence(license),username);
            }
            else if(x == 0)
            {
                return 0;
            }
        }
    }
    else
    {
        bool uploadState = mainManager.uploadHash(parser.value(upload).toStdString());
        if(uploadState)
        {
            std::cout << "-- File uploaded successfully --" << std::endl;
        }
        else
        {
            std::cout << "-- File upload failed --" << std::endl;
        }
    }

    return 0;
    //return a.exec();
}
