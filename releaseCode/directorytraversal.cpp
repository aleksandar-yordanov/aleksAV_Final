#include "directorytraversal.h"
#include "filescanner.h"
#include "mainwindow.h"
//depth first search algorithm for the windows file structure

directoryTraversal::directoryTraversal(MainWindow* mainWindow)
{
    directoryTraversal::mainWindow = mainWindow;
    connect(this, SIGNAL(textChanged(QString)),mainWindow,SLOT(setScanText(QString)));
}

bool directoryTraversal::hasEnding (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

void directoryTraversal::traverse(LPCSTR path)
{
    std::string searchPath = std::string(path) + "*";
    //std::cout << "path: " << search_path << std::endl;
    WIN32_FIND_DATA mainStruct;
    HANDLE hFindStruct = FindFirstFile(searchPath.c_str(),&mainStruct);
    //std::cout << mainStruct.cFileName << std::endl;
    if(hFindStruct == INVALID_HANDLE_VALUE)
    {
        std::cout << GetLastError() << std::endl;
    }
    do
    {
        std::string x = std::string(mainStruct.cFileName);
        if(x != "." && x != "..")
        {
            if(mainStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                std::string passStr = std::string(path) + x + "\\";
                this->traverse(passStr.c_str());
            }
            else
            {
                if(directoryTraversal::hasEnding((path+x),".exe"))
                {
                    emit textChanged("Currently scanning: " + QString::fromStdString(x));
                    this->paths.emplace_back(sigExeDetails{false,path+x});
                }
            }

        }
    }while(FindNextFile(hFindStruct,&mainStruct));
    FindClose(hFindStruct);
}

std::vector<sigExeDetails> directoryTraversal::returnResult()
{
    return this->paths;
}
