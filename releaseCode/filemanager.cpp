#include "filemanager.h"
#include "mainwindow.h"
#include "threattype.h"

fileManager::fileManager(MainWindow* mainWnd)
{
    this->fillQuarantineFileList();
    mainWnd->appendQuarantinedItem(this->quarantineFileList);
}

bool fileManager::deleteFile(std::string originalPath,bool quarantined)
{
    if(quarantined)
    {
        std::ifstream fileStream(this->quarantinedFiles);
        std::ofstream outStream(this->quarantineFilePath + "temp.txt");
        if(fileStream.end == EOF)
        {
            return false;
        }
        if(fileStream.fail())
        {
            return false;
        }
        std::string line;
        std::string lineToReplace;
        bool found = false;
        while(std::getline(fileStream,line))
        {
            std::string filePath;
            int seperatorPos = line.find('|');
            std::string x = line.substr(0,seperatorPos);
            if(x == originalPath)
            {
                lineToReplace = line;
                found = true;
                continue;
            }
            outStream << '\n' + line;
        }
        fileStream.close();
        outStream.close();
        std::remove(this->quarantinedFiles.c_str());
        std::rename((this->quarantineFilePath + "temp.txt").c_str(),this->quarantinedFiles.c_str());
        if(found)
        {
            std::string baseFilename = originalPath.substr(originalPath.find_last_of("\\") + 1);
            QFile file(QString::fromStdString(this->quarantineFilePath + baseFilename));
            file.moveToTrash();
            return true;
        }
        else
        {
            logging.debug->logMessage("File manager: File not in quarantined list");
        }
    }
    else
    {
        QFile file(QString::fromStdString(originalPath));
        if(file.isOpen())
        {
            file.moveToTrash(); //WARNING, NUKED HARD DRIVES ARE NOT MY FAULT
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool fileManager::restoreFile(std::string originalPath)
{
    std::ifstream fileStream(this->quarantinedFiles);
    std::ofstream outStream(this->quarantineFilePath + "temp.txt");
    if(fileStream.end == EOF)
    {
        return false;
    }
    if(fileStream.fail())
    {
        return false;
    }
    std::string line;
    std::string lineToReplace;
    bool found = false;
    while(std::getline(fileStream,line))
    {
        std::string filePath;
        int seperatorPos = line.find('|');
        std::string x = line.substr(0,seperatorPos);
        if(x == originalPath)
        {
            lineToReplace = line;
            found = true;
            continue;
        }
        outStream << line + '\n';
    }
    fileStream.close();
    outStream.close();
    if(found)
    {
        std::string baseFilename = originalPath.substr(originalPath.find_last_of("\\") + 1);
        QFile file(QString::fromStdString(this->quarantineFilePath + baseFilename));
        bool x = file.copy(QString::fromStdString(originalPath));
        if(x)
        {
            std::remove(this->quarantinedFiles.c_str());
            std::rename((this->quarantineFilePath + "temp.txt").c_str(),this->quarantinedFiles.c_str());
            file.moveToTrash(); //kept here for safety
            return true;
        }
        else
        {
            std::remove((this->quarantineFilePath + "temp.txt").c_str());
            logging.debug->logMessage("File manager: File exists");
            return false;
        }
    }
    else
    {
        std::remove((this->quarantineFilePath + "temp.txt").c_str());
        logging.debug->logMessage("File manager: File not in quarantined list");
        return false;
    }
}

bool fileManager::quarantineFile(std::string originalPath, threats::threatType type)
{
    std::string baseFilename = originalPath.substr(originalPath.find_last_of("\\") + 1);
    QFile file(QString::fromStdString(originalPath));
    bool state = file.copy(QString::fromStdString(this->quarantineFilePath + baseFilename));
    if(state)
    {
        std::ofstream outputStream(this->quarantinedFiles, std::ios::app);
        if(outputStream.fail())
        {
            return false;
        }
        std::string writeString = originalPath + '|' + threats::threat::mapThreatTypeToStr(type);
        outputStream << writeString + '\n';
        outputStream.close();
        file.moveToTrash();
        return true;
    }
    else
    {
        return false;
    }
}

void fileManager::fillQuarantineFileList()
{
    std::ifstream fileStream(this->quarantinedFiles);
    if(fileStream.end == EOF)
    {
        return;
    }
    if(fileStream.fail())
    {
        return;
    }
    std::string line;
    while(std::getline(fileStream,line))
    {
        std::string filePath;
        int seperatorPos = line.find('|');
        quarantineInfo curInfo;
        curInfo.originalFilePath = line.substr(0,seperatorPos);
        curInfo.type = threats::threat::mapStrToThreatType(line.substr(seperatorPos+1,-1));
        this->quarantineFileList.emplace_back(curInfo);
    }
    fileStream.close();
}

std::vector<quarantineInfo> fileManager::getInfo()
{
    return this->quarantineFileList;
}
