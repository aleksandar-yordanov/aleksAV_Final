#pragma once
#include "includes.h"
#include "stringrequest.h"
#include "httprequest.h"

enum class t_licenses
{
    license_none,
    license_standard,
    license_premium
};

class networkingManager
{
public:
    networkingManager(std::string adminUsername, std::string adminPassword);
    bool login();
    bool uploadHash(std::string path);
    bool changeLicense(t_licenses license, std::string userToChange);
    bool changeUserPassword(std::string username, std::string password);
    std::string calculateFileHash(std::string fileLocation);
    static t_licenses mapIntToLicence(int input);
    static std::string mapLicenseToStr(t_licenses license);
private:
    std::string adminUsername;
    std::string adminPassword;
};
