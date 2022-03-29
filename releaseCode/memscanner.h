#pragma once
#include "Entropy.h"
#include "PEdump.h"
#include "includes.h"

namespace scanning
{

    class scanManager;

    namespace memScanner
    {

        struct entWrapperStruct
        {
            std::string& path;
            bool highEnt;
        };

        class Scanner
        {
        public:
            Scanner(scanning::scanManager* mainManager);
            std::vector<exeDetails> scan();
            static void entWrapper(unsigned int index);
        private:
            scanning::scanManager* mainManager;
            inline static std::mutex vecLock;
            inline static Entropy mainEntScanner;
            std::string calculateFileHash(std::string fileLocation);
            std::vector<DWORD> tlhelp32EnumProcesses();
            static void getProcNameFromID(DWORD processID, std::vector<DWORD>& vecRef);
            inline static std::vector<exeDetails> details;
            std::vector<std::future<void>> futuresVec;
        };
    }
}
