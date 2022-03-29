#include "includes.h"
#include "directorytraversal.h"

class MainWindow;

namespace scanning
{

    namespace fileScanning
    {
        class fileScanner
        {           
        public:
            fileScanner(MainWindow* mainWindow = nullptr);
            static std::string calculateFileHash(std::string fileLocation);
            std::vector<sigExeDetails> scan(LPCSTR path);
        private:
            directoryTraversal* mainTraversal;
            MainWindow* mainWindow;
            static void hashWrapper(sigExeDetails* detail);
            inline static std::vector<sigExeDetails> filePaths;
            std::vector<std::future<void>> futuresVec;
            static inline std::mutex hashLock;

        };

    }

}
