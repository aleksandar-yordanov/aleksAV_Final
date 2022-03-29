#include "filescanner.h"
#include "httprequest.h"
#include "stringrequest.h"
#include "scanmanager.h"
#include "mainwindow.h"

using namespace scanning::fileScanning;

fileScanner::fileScanner(MainWindow* mainWindow)
{
    this->mainWindow = mainWindow;
    directoryTraversal* dirPassThrough = new directoryTraversal(mainWindow);
    this->mainTraversal = dirPassThrough;
}

void fileScanner::hashWrapper(sigExeDetails* detail)
{
    std::lock_guard lock(fileScanner::hashLock);
    std::string hash = fileScanner::calculateFileHash(detail->path);
    detail->hash = hash;
}

std::vector<sigExeDetails> fileScanner::scan(LPCSTR path)
{
    this->mainTraversal->traverse(path);
    this->filePaths = mainTraversal->returnResult();
    this->mainWindow->setScanText("Reading hashes for files");
    for(sigExeDetails& detail : this->filePaths)
    {
        this->futuresVec.emplace_back(std::async(std::launch::async,fileScanner::hashWrapper,&detail));
    }
    for(const auto& future: this->futuresVec)
    {
        future.wait();
    }
    unsigned int c = 0;
    int numJson = (this->filePaths.size() / 10) + 1;
    //Fix json array, JSONS can have a max size of 10, or from 0 to 9. Last part, everything else works fine
    std::vector<json> sigJsVec;
    for(unsigned int i = 0; i < numJson && c < this->filePaths.size(); i++)
    {
        json sigJs;
        for(unsigned int t = 0; t < 10 && c < this->filePaths.size();t++)
        {
            std::string signum = "sig" + std::to_string(c);
            sigJs[signum] = this->filePaths[c].hash;
            c++;
        }
        sigJsVec.emplace_back(sigJs);
    }
    //std::string str2 = sigJs.dump();
    //std::cout << str2 << std::endl;
    for(unsigned int i = 0; i < sigJsVec.size(); i++)
    {
        net::Session mainSession(true);
        this->mainWindow->setScanText("Querying hashes");
        mainSession.initialiseEndpoint("51.38.80.233", "http");
        net::StringRequest sReq(net::method::METHOD_POST,"aleks/sigcheck.php","51.38.80.233",sigJsVec[i].dump());
        mainSession.connectAndSend(sReq);
        auto retVec = net::parseResponseVec(mainSession.returnResponse());
        for(unsigned int t = 0; t < retVec.size();t++)
        {
            if(retVec[t] == 1)
            {
                this->filePaths[t + i*10].sigMatch = true;
            }
        }
    }
    this->mainWindow->setScanText("");
    return this->filePaths;
}

std::string fileScanner::calculateFileHash(std::string fileLocation)
{
    QFile file(QString::fromStdString(fileLocation));
    std::string fileHash = "";
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = file.readAll();

        QByteArray hashData = QCryptographicHash::hash(fileData,QCryptographicHash::Md5); // or QCryptographicHash::Sha1
        fileHash = hashData.toHex(0).toStdString();
    }
    else
    {
        fileHash = "0";
    }
    return fileHash;
}
