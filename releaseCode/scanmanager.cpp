#include "scanmanager.h"
#include "PEdump.h"
#include "memscanner.h"
#include "mainwindow.h"
#include "filescanner.h"
#include "directorytraversal.h"

using namespace scanning;
scanManager::scanManager(MainWindow* mainWnd, scanType type)
{
    this->mainWindow = mainWnd;
    if(type == scanType::memory_scan)
    {
        scanning::memScanner::Scanner scanner = scanning::memScanner::Scanner(this);
        std::vector<exeDetails> list = scanner.scan();
        for(auto& detail: list)
        {
            if(detail.highEntropy == false && detail.maliciousImports == false && detail.sigMatch == false)
            {
                continue;
            }
            else
            {
               this->threatVec.emplace_back(this->mapDetailsToThreat(detail));
            }

        }
    }
    else if(type == scanType::static_scan)
    {
        std::string path = mainWnd->getSearchDir();
        if(path != "\\")
        {
            scanning::fileScanning::fileScanner scanner = scanning::fileScanning::fileScanner(mainWnd);
            std::vector<sigExeDetails> list = scanner.scan(path.c_str());
            for(auto& detail : list)
            {
                if(detail.sigMatch == true)
                {
                    this->threatVec.emplace_back(this->mapSigDetailsToThreat(detail));
                }
            }
        }
    }
    else
    {
        pass;
    }
}

void scanManager::setScanText(std::string inStr)
{
    this->mainWindow->setScanText(QString::fromStdString(inStr));
}

std::vector<threats::threat> scanManager::returnResults()
{
    return this->threatVec;
}

threats::threat scanManager::mapSigDetailsToThreat(const sigExeDetails& details)
{
    threats::threatType type;
    if(details.sigMatch == true)
    {
        type = threats::threatType::Type_Malicious;
        logging.debug->logMessage("Threat detected, Flags: sigMatch");
    }
    else
    {
        type = threats::threatType::Type_None;
    }
    threats::threat retThreat(details.path,type,details.hash);
    return retThreat;
}

threats::threat scanManager::mapDetailsToThreat(const exeDetails& details)
{
    threats::threatType type;
    if(details.sigMatch == true)
    {
        type = threats::threatType::Type_Malicious;
        logging.debug->logMessage("Threat detected, Flags: sigMatch");
    }
    else if(details.highEntropy == true && details.maliciousImports == true)
    {
        type = threats::threatType::Type_PUP;
        logging.debug->logMessage("Threat detected, Flags: highEntropy, maliciousImports");
    }
    else if(details.highEntropy == true)
    {
        type = threats::threatType::Type_Unknown;
        logging.debug->logMessage("Threat detected, Flags: highEntropy");
    }
    else if(details.maliciousImports == true)
    {
        type = threats::threatType::Type_Unknown;
        logging.debug->logMessage("Threat detected, Flags: maliciousImports");
    }
    else
    {
        type = threats::threatType::Type_None;
    }
    threats::threat retThreat(details.path,type,details.fileHash);
    return retThreat;
}
