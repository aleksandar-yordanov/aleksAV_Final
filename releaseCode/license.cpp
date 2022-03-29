#include "license.h"
#include "httprequest.h"
#include "stringrequest.h"

license::t_licenses license::mapLicenseResponseToLicense(net::HTTPLicenseResponse response)
{
    switch (response) {
    case net::HTTPLicenseResponse::databaseDown:
        return license::t_licenses::license_none;
        break;
    case net::HTTPLicenseResponse::license_none:
        return license::t_licenses::license_none;
        break;
    case net::HTTPLicenseResponse::license_standard:
        return license::t_licenses::license_standard;
        break;
    case net::HTTPLicenseResponse::license_premium:
        return license::t_licenses::license_premium;
        break;
    default:
        return license::t_licenses::license_none;
        break;
    }
}

license::t_licenses license::getLicenseStatic(std::string username)
{
    if(username == "bypass") return license::mapLicenseResponseToLicense(net::licenseMapIntToEnum(1)); //DEBUG, REMOVE IN FINAL BUILD
    json licenseJs;
    licenseJs["Username"] = username;
    net::StringRequest sReq(net::method::METHOD_POST, "aleks/license.php","51.38.80.233", licenseJs.dump());
    net::Session s1;
    s1.initialiseEndpoint(sReq, "http");
    s1.connectAndSend(sReq);
    int licenseState = net::parseResponse(s1.returnResponse());
    return license::mapLicenseResponseToLicense(net::licenseMapIntToEnum(licenseState));
}

std::string license::mapLicenseToString(license::t_licenses license)
{
    switch (license)
    {
    case license::t_licenses::license_none:
        return "license_none";
        break;
    case license::t_licenses::license_standard:
        return "license_standard";
        break;
    case license::t_licenses::license_premium:
        return "license_premium";
        break;
    default:
        return "license_none";
        break;
    }
}

std::string license::mapLicenseToStringFormatted(license::t_licenses license)
{
    switch (license)
    {
    case license::t_licenses::license_none:
        return "No License";
        break;
    case license::t_licenses::license_standard:
        return "Standard License";
        break;
    case license::t_licenses::license_premium:
        return "Premium License";
        break;
    default:
        return "No License";
        break;
    }
}
