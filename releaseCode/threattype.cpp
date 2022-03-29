#include "threattype.h"
using namespace threats;

threat::threat(std::string inFileLocation, threats::threatType inType, std::string hash)
{
    this->fileLocation = inFileLocation;
    this->type = inType;
    this->fileHash = hash;
}

std::string threat::returnFileLocation()
{
    return this->fileLocation;
}
threatType threat::returnThreatType()
{
    return this->type;
}
std::string threat::returnFileHash()
{
    return this->fileHash;
}
void threat::setFileLocation(std::string inFileLocation)
{
    this->fileLocation = inFileLocation;
}
void threat::setThreatType(threatType inType)
{
    this->type = inType;
}

threatType threat::mapStrToThreatType(std::string inString)
{
    if(inString == "Type_PUP")
    {
        return threatType::Type_PUP;
    }
    else if(inString == "Type_Unknown")
    {
        return threatType::Type_Unknown;
    }
    else if(inString == "Type_Malicious")
    {
        return threatType::Type_Malicious;
    }
    else
    {
        return threatType::Type_Unknown;
    }
}

std::string threat::mapThreatTypeToStr(threatType inThreat)
{
    switch(inThreat)
    {
    case threatType::Type_Malicious:
        return "Type_Malicious";
        break;
    case threatType::Type_PUP:
        return "Type_PUP";
        break;
    case threatType::Type_Unknown:
        return "Type_Unknown";
        break;
    case threatType::Type_None:
        return "Type_None";
        break;
    default:
        return "Type_Unknown";
        break;
    }
}

std::string threat::mapThreatTypeToShortStr(threatType inThreat)
{
    switch(inThreat)
    {
    case threatType::Type_Malicious:
        return "Mal";
        break;
    case threatType::Type_PUP:
        return "PUP";
        break;
    case threatType::Type_Unknown:
        return "Unk";
        break;
    case threatType::Type_None:
        return "None";
        break;
    default:
        return "Unk";
        break;
    }
}

void threat::setFileHash(std::string hash)
{
    this->fileHash = hash;
}

