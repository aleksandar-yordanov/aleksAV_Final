#pragma once
#include "includes.h"
#include "threattype.h"

class MainWindow;

struct exeDetails;

struct sigExeDetails;

namespace scanning
{
    enum class scanType
    {
        static_scan,
        memory_scan
    };

    class scanManager
    {
    public:
        scanManager(MainWindow* mainWnd = nullptr,scanType type = scanType::memory_scan);
        std::vector<threats::threat> returnResults();
        void setScanText(std::string inStr);
    private:
        bool staticScan;
        float scanProgrss = 0;
        std::vector<threats::threat> threatVec;
        threats::threat mapDetailsToThreat(const exeDetails& details);
        threats::threat mapSigDetailsToThreat(const sigExeDetails& details);
        MainWindow* mainWindow;
    };

}
