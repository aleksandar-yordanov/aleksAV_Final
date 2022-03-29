#include "memscanner.h"
#include "stringrequest.h"
#include "httprequest.h"
#include "scanmanager.h"

using namespace scanning::memScanner;

Scanner::Scanner(scanning::scanManager* mainManager)
{
    this->mainManager = mainManager;
}

std::string Scanner::calculateFileHash(std::string fileLocation)
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

std::vector<DWORD> Scanner::tlhelp32EnumProcesses()
{
    HANDLE hProcSnap;
    PROCESSENTRY32 pr32;

    std::vector<DWORD> procIDList;

    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (hProcSnap == INVALID_HANDLE_VALUE)
    {
        std::cout << "invalid handle" << std::endl;
        return {};
    }
    pr32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcSnap, &pr32))
    {
        CloseHandle(hProcSnap);
        std::cout << "proc32iter failed" << std::endl;
        return {};
    }
    do {
        HANDLE bitTest = OpenProcess(PROCESS_VM_READ,FALSE,pr32.th32ProcessID);
        BOOL bitState;
        IsWow64Process(bitTest,&bitState);
        if(bitState)
        {
            std::cout << pr32.th32ProcessID << std::endl;
            procIDList.emplace_back(pr32.th32ProcessID);
        }
    } while (Process32Next(hProcSnap, &pr32));
    CloseHandle(hProcSnap);
    return procIDList;
}

void Scanner::getProcNameFromID(DWORD processID, std::vector<DWORD>& vecRef)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("none");

    HANDLE hProcess = OpenProcess(
        PROCESS_ALL_ACCESS,
        FALSE, processID);

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod),&cbNeeded,LIST_MODULES_ALL))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
    }
    if (strcmp(szProcessName, "none") != 0)
    {
        vecRef.emplace_back(processID);
    }
    CloseHandle(hProcess);
}

void Scanner::entWrapper(unsigned int index)
{
    std::lock_guard lock(Scanner::vecLock);
    double ent = Scanner::mainEntScanner.readByteEntropy(Scanner::details[index].path,false);
    if(ent > 6.7)
    {
        Scanner::details[index].highEntropy = true;
    }
    else
    {
        Scanner::details[index].highEntropy = false;
    }
}

std::vector<exeDetails> Scanner::scan()
{
    this->mainManager->setScanText("Import detection");
	DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return {};
	}
	std::vector<DWORD> procIDVec;
	cProcesses = cbNeeded / sizeof(DWORD);
	for (unsigned int i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			this->getProcNameFromID(aProcesses[i], procIDVec);
		}
	}
    int d = procIDVec.size();
    std::cout << d << std::endl;
	PEDumper x(procIDVec);
	std::vector<std::string> result = x.returnResults();
	std::set<std::string> s(result.begin(), result.end());
	result.assign(s.begin(), s.end());
	for (const auto& x : result)
	{
        //logging.debug->logMessage(x);
		int colonPos = x.find('?');
		bool mal = false;
		if (x.substr(colonPos + 1, -1) == "true")
		{
			mal = true;
		}
		exeDetails currentDetails = exeDetails{ x.substr(0,colonPos),mal,false,false };
		this->details.emplace_back(currentDetails);
	}
    this->mainManager->setScanText("Entropy checking");
    for(unsigned int i = 0; i < this->details.size(); i++)
    {
        this->futuresVec.emplace_back(std::async(std::launch::async, Scanner::entWrapper, i));
    }
    for(auto& future : this->futuresVec)
    {
        future.wait();
    }
    this->mainManager->setScanText("Hash comparison");

    std::vector<json> sigJsVec;
    unsigned int c = 0;
    unsigned int numJson = (this->details.size() / 10) + 1;
    for(unsigned int i = 0; i < numJson && c < this->details.size();i++)
    {
        json sigJs;
        for(unsigned int t = 0; t < 10 && c < this->details.size();t++)
        {
            std::string signum = "sig" + std::to_string(c);
            std::string hash = calculateFileHash(this->details[c].path);
            this->details[c].fileHash = hash;
            sigJs[signum] = hash;
            c++;
        }
        sigJsVec.emplace_back(sigJs);
    }
    for(unsigned int i = 0; i < sigJsVec.size(); i++)
    {
        net::Session mainSession(true);
        this->mainManager->setScanText("Querying hashes");
        mainSession.initialiseEndpoint("51.38.80.233", "http");
        net::StringRequest sReq(net::method::METHOD_POST,"aleks/sigcheck.php","51.38.80.233",sigJsVec[i].dump());
        mainSession.connectAndSend(sReq);
        auto retVec = net::parseResponseVec(mainSession.returnResponse());
        for(unsigned int t = 0; t < retVec.size();t++)
        {
            if(retVec[t] == 1)
            {
                this->details[t + i*10].sigMatch = true;
            }
            else
            {
                this->details[t + i*10].sigMatch = false;
            }
        }
    }
    this->mainManager->setScanText("");
	return this->details;
}

