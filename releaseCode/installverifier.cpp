#include "installverifier.h"

bool installVerifier::verifyInstall()
{
    bool dirVerified = this->verifyDir();
    if(!dirVerified)
    {
        return false;
    }
    bool filesVerified = this->verifyFiles();
    if(!filesVerified)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void installVerifier::repairInstall()
{
    for(const components& comp : this->missingComponents)
    {
        if(comp == components::DataDir)
        {
            QDir().mkdir(QString::fromStdString(utils::getHomeDir() + "/AleksAvData/"));
        }
        else if(comp == components::QuarantineDir)
        {
            QDir().mkdir(QString::fromStdString(utils::getHomeDir() + "/aleksAvQuarantine"));
        }
    }
    for(const components& comp : this->missingComponents)
    {
        switch(comp)
        {
            case components::AdvertImage:
            {
                logging.debug->logMessage("Check github repo for advertising image (ad0.jpg) and move to data directory");
                break;
            }
            case components::CountDump:
            {
                std::ofstream CountDumpCreator(std::string(utils::getHomeDir() + "/AleksAvData/CountDump.txt"));
                CountDumpCreator << "0,0,0,0";
                CountDumpCreator.close();
                break;
            }
            case components::QuarantineFile:
            {
                std::ofstream QuarantineFileCreator(utils::getHomeDir() + "/AleksAvData/CountDump.txt");
                QuarantineFileCreator.clear();
                QuarantineFileCreator.close();
                break;
            }
            case components::QuarantineDir:
                break;
            case components::DataDir:
                break;
            case components::RequiredDLLs:
            {
                logging.debug->logMessage("Check github repo for DLL-DumpPEHeader.dll and move to data directory");
                break;
            }
            case components::StatsDump:
            {
                std::ofstream StatsDumpCreator(utils::getHomeDir() + "/AleksAvData/StatsDump.txt");
                StatsDumpCreator << "0,0,0,0,0,0,0";
                StatsDumpCreator.close();
                break;
            }
            case components::pwCache:
            {
                std::ofstream pwCacheCreator(utils::getHomeDir() + "/AleksAvData/cache.txt");
                pwCacheCreator << '\n';
                pwCacheCreator.close();
                break;
            }
        }
    }
}

bool installVerifier::verifyFiles()
{
    QFile statsDump(QString::fromStdString(utils::getHomeDir() + "/AleksAvData/StatsDump.txt"));
    QFile countDump(QString::fromStdString(utils::getHomeDir() + "/AleksAvData/CountDump.txt"));
    QFile peDumpDLL(QString::fromStdString(utils::getHomeDir() + "/AleksAvData/DLL-DumpPEHeaders.dll"));
    QFile adImage(QString::fromStdString(utils::getHomeDir() + "/AleksAvData/ad0.jpg"));
    QFile pwCache(QString::fromStdString(utils::getHomeDir() + "/AleksAvData/cache.txt"));
    QFile quarantineFile(QString::fromStdString(utils::getHomeDir()+ "/aleksAvQuarantine/quarantineFile.txt"));
    bool statsDumpExists = statsDump.exists();
    bool countDumpExists = countDump.exists();
    bool peDumpDLLExists = peDumpDLL.exists();
    bool adImageExists = adImage.exists();
    bool pwCacheExists = pwCache.exists();
    bool quarantineFileExists = quarantineFile.exists();
    if(statsDumpExists && countDumpExists && peDumpDLLExists && adImageExists && pwCacheExists && quarantineFileExists)
    {
        statsDump.close();
        countDump.close();
        peDumpDLL.close();
        adImage.close();
        pwCache.close();
        quarantineFile.close();
        return true;
    }
    if(!statsDumpExists)
    {
        this->missingComponents.emplace_back(components::StatsDump);
        logging.debug->logMessage("Stats dump file missing");
    }
    else if(!countDumpExists)
    {
        this->missingComponents.emplace_back(components::CountDump);
        logging.debug->logMessage("Count dump file missing");
    }
    else if(!peDumpDLLExists)
    {
        this->missingComponents.emplace_back(components::RequiredDLLs);
        logging.debug->logMessage("PE Dump DLL missing");
    }
    else if(!adImageExists)
    {
        this->missingComponents.emplace_back(components::AdvertImage);
        logging.debug->logMessage("Advertising image missing");
    }
    else if(!pwCacheExists)
    {
        this->missingComponents.emplace_back(components::pwCache);
        logging.debug->logMessage("Username cache missing");
    }
    else if(!quarantineFileExists)
    {
        this->missingComponents.emplace_back(components::QuarantineFile);
        logging.debug->logMessage("Quarantine database missing");
    }
    statsDump.close();
    countDump.close();
    peDumpDLL.close();
    adImage.close();
    pwCache.close();
    quarantineFile.close();
    return false;
}

bool installVerifier::verifyDir()
{
    QDir checkDir(QString::fromStdString(utils::getHomeDir() + "/AleksAvData/"));
    QDir checkQuarantineDir(QString::fromStdString(utils::getHomeDir() + "/aleksAvQuarantine"));
    bool checkDirExists = checkDir.exists();
    bool checkQuarantineDirExists = checkQuarantineDir.exists();
    if(checkDirExists && checkQuarantineDirExists)
    {
        return true;
    }
    if(!checkDirExists)
    {
        this->missingComponents.emplace_back(components::DataDir);
        logging.debug->logMessage("Data directory missing");
    }
    if(!checkQuarantineDirExists)
    {
        this->missingComponents.emplace_back(components::QuarantineDir);
        logging.debug->logMessage("Quarantine directory missing");
    }
    return false;
}

