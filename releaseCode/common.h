#pragma once
#include "includes.h"

//Generic utilities header

class debugWindow;

struct LOGGER_STRUCT
{
    debugWindow* debug;
};

extern LOGGER_STRUCT logging;

namespace license
{
    enum class t_licenses;
}

#define pass (void)0

struct userDetails
{
    std::string username;
    std::string password;
    license::t_licenses licenseType;
};


namespace utils
{

    template<class T> void safeDelete(T* ptr); //for raw unmanaged, non QPointers.

    template<class T> T safeMalloc(T* ptr); //for raw unmanaged, non QPointers.

    std::string boolToStr(bool state);

    bool checkStrToInt(std::string str);

    bool strToBool(std::string state);

    std::string getHomeDir();

    bool is_empty(std::ifstream& pFile);

    std::string intToStr(int x);
}
