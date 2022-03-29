#pragma once
#include "includes.h"

namespace threats{

#define TYPE_COUNT 4 //change when adding more types

enum class threatType
{
    Type_PUP,
    Type_Unknown,
    Type_Malicious,
    Type_None
};

class threat
{
public:
    threat(std::string fileLocation, threats::threatType type,std::string hash = "");
    std::string returnFileLocation();
    std::string returnFileHash();
    threatType returnThreatType();
    void setFileLocation(std::string inFileLocation);
    void setThreatType(threatType inType);
    void setFileHash(std::string hash);
    static std::string mapThreatTypeToStr(threatType inThreat);
    static threatType mapStrToThreatType(std::string inString);
    static std::string mapThreatTypeToShortStr(threatType inThreat);
private:
    threatType type;
    std::string fileHash;
    std::string fileLocation;
};

}
