#pragma once
#include "includes.h"


class MainWindow;

struct sigExeDetails
{
    bool sigMatch;
    std::string path;
    std::string hash = "";
};

class directoryTraversal : public QObject
{
    Q_OBJECT

public:
    directoryTraversal(MainWindow* mainWindow = nullptr);
    static bool hasEnding (std::string const &fullString, std::string const &ending);
    void traverse(LPCSTR path);
    std::vector<sigExeDetails> returnResult();
    MainWindow* mainWindow;

signals:
    void textChanged(QString);

private:
    LPCSTR startPath;
    std::vector<sigExeDetails> paths;
};


