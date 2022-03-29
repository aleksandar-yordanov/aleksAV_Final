#pragma once
#include "includes.h"

namespace threats
{
    enum class threatType;
}

class MainWindow;

struct quarantineInfo
{
    std::string originalFilePath;
    threats::threatType type;
};

class fileManager
{
public:
    fileManager(MainWindow* mainWnd); //get the quarantined files in constructor
    bool restoreFile(std::string originalFile);
    bool quarantineFile(std::string originalFile, threats::threatType type);
    bool deleteFile(std::string originalFile, bool quarantined);
    std::vector<quarantineInfo> getInfo();
private:
    void fillQuarantineFileList();
    std::vector<quarantineInfo> quarantineFileList;
    std::string currentFile;
    const std::string quarantineFilePath = "C:\\Users\\alexs\\aleksAvQuarantine\\";
    const std::string quarantinedFiles = "C:\\Users\\alexs\\aleksAvQuarantine\\quarantineFile.txt";
    MainWindow* mainWnd;
};
