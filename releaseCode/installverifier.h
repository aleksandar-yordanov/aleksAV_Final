#include "includes.h"

enum class components
{
    StatsDump,
    CountDump,
    AdvertImage,
    DataDir,
    QuarantineDir,
    QuarantineFile,
    RequiredDLLs,
    pwCache
};

class installVerifier
{
public:
    installVerifier() = default;
    bool verifyInstall();
    void repairInstall();

private:
    std::string installDir = utils::getHomeDir() + "/AleksAvData/";
    std::vector<components> missingComponents;
    bool verifyFiles();
    bool verifyDir();
};
