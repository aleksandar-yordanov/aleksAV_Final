#pragma once
#include "includes.h"

namespace net
{
    enum class HTTPLicenseResponse;
}

namespace license{

    enum class t_licenses
    {
        license_none,
        license_standard,
        license_premium
    };
    license::t_licenses getLicenseStatic(std::string username);
    std::string mapLicenseToString(license::t_licenses license);
    std::string mapLicenseToStringFormatted(license::t_licenses license);
    t_licenses mapLicenseResponseToLicense(net::HTTPLicenseResponse response);

}
