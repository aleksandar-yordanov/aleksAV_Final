#include "networkingmanager.h"

using namespace std::chrono_literals;

networkingManager::networkingManager(std::string adminUsername, std::string adminPassword)
{
    this->adminUsername = adminUsername;
    this->adminPassword = adminPassword;
}

bool networkingManager::login()
{
    json loginJs;
    loginJs["Username"] = this->adminUsername;
    loginJs["Password"] = this->adminPassword;
    net::StringRequest sReq(net::method::METHOD_POST,"aleks/adminLogin.php","51.38.80.233",loginJs.dump());
    net::Session loginSession;
    loginSession.initialiseEndpoint("51.38.80.233","http");
    loginSession.connectAndSend(sReq);
    net::HTTPAdminResponses response = net::mapAdminIntToEnum(net::parseResponse(loginSession.returnResponse()));
    switch(response)
    {
    case net::HTTPAdminResponses::loginSuccess:
        std::cout << "-- Login successful --" << std::endl;
        return true;
        break;
    case net::HTTPAdminResponses::notAdmin:
        std::cout << "> You are not an administrator, please log in with an administrator account" << std::endl;
        return false;
        break;
    case net::HTTPAdminResponses::loginFailure:
        std::cout << "> Your password is incorrect" << std::endl;
        return false;
        break;
    case net::HTTPAdminResponses::databaseDown:
        std::cout << "> Database down, restart DB" << std::endl;
        return false;
        break;
    default:
        return false;
        break;
    }
}

bool networkingManager::changeLicense(t_licenses license, std::string userToChange)
{
    json changeJs;
    changeJs["Username"] = this->adminUsername;
    changeJs["Password"] = this->adminPassword;
    changeJs["UserToChange"] = userToChange;
    changeJs["License"] = this->mapLicenseToStr(license);
    net::StringRequest sReq(net::method::METHOD_POST,"aleks/adminUpgradeLicense.php","51.38.80.233",changeJs.dump());
    net::Session updateSession;
    updateSession.initialiseEndpoint("51.38.80.233","http");
    updateSession.connectAndSend(sReq);
    net::HTTPAdminUpgradeResponses response = net::mapAdminUpgradeIntToEnum(net::parseResponse(updateSession.returnResponse()));
    switch(response)
    {
    case net::HTTPAdminUpgradeResponses::changeSuccess:
        std::cout << "-- Change successful --" << std::endl;
        return true;
        break;
    case net::HTTPAdminUpgradeResponses::notAdmin:
        std::cout << "> You are not an administrator, please log in with an administrator account" << std::endl;
        return false;
        break;
    case net::HTTPAdminUpgradeResponses::invalidAdminPassword:
        std::cout << "> Your password is incorrect" << std::endl;
        return false;
        break;
    case net::HTTPAdminUpgradeResponses::databaseDown:
        std::cout << "> Database down, restart DB" << std::endl;
        return false;
        break;
    case net::HTTPAdminUpgradeResponses::invalidUsernameGiven:
        std::cout << "> Invalid username given" << std::endl;
        return false;
        break;
    default:
        std::cout << "> Request failed" << std::endl;
        return false;
        break;
    }
}

bool networkingManager::changeUserPassword(std::string username, std::string password)
{
    json changeJs;
    changeJs["Username"] = this->adminUsername;
    changeJs["Password"] = this->adminPassword;
    changeJs["UserToChange"] = username;
    changeJs["PasswordToChange"] = password;
    net::StringRequest sReq(net::method::METHOD_POST,"aleks/adminChangePassword.php","51.38.80.233",changeJs.dump());
    net::Session updateSession;
    updateSession.initialiseEndpoint("51.38.80.233","http");
    updateSession.connectAndSend(sReq);
    net::HTTPAdminResetResponses response = net::mapAdminResetIntToEnum(net::parseResponse(updateSession.returnResponse()));
    switch(response)
    {
    case net::HTTPAdminResetResponses::changeSuccess:
        std::cout << "-- Change successful --" << std::endl;
        return true;
        break;
    case net::HTTPAdminResetResponses::notAdmin:
        std::cout << "> You are not an administrator, please log in with an administrator account" << std::endl;
        return false;
        break;
    case net::HTTPAdminResetResponses::invalidAdminPassword:
        std::cout << "> Your password is incorrect" << std::endl;
        return false;
        break;
    case net::HTTPAdminResetResponses::databaseDown:
        std::cout << "> Database down, restart DB" << std::endl;
        return false;
        break;
    case net::HTTPAdminResetResponses::invalidUsernameGiven:
        std::cout << "> Invalid username" << std::endl;
        return false;
    case net::HTTPAdminResetResponses::passwordLengthInvalid:
        std::cout << "> Invalid password length, Passwords must be more than 6 characters or less than 64 characters";
        return false;
    case net::HTTPAdminResetResponses::unknownResponse:
        std::cout << "> Request Failed" << std::endl;
        break;
    default:
        std::cout << "> Request Failed" << std::endl;
        return false;
        break;
    }
}

std::string networkingManager::calculateFileHash(std::string fileLocation)
{
    QFile file(QString::fromStdString(fileLocation));
    std::string fileHash = "";
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = file.readAll();

        QByteArray hashData = QCryptographicHash::hash(fileData,QCryptographicHash::Md5); // or QCryptographicHash::Sha1
        fileHash = hashData.toHex(0).toStdString();
    }
    else
    {
        fileHash = "0";
    }
    return fileHash;
}

bool networkingManager::uploadHash(std::string path)
{
    json loginJs;
    loginJs["Username"] = this->adminUsername;
    loginJs["Password"] = this->adminPassword;
    loginJs["Sig"] = this->calculateFileHash(path);
    net::StringRequest sReq(net::method::METHOD_POST,"aleks/sigUpload.php","51.38.80.233",loginJs.dump());
    net::Session loginSession;
    loginSession.initialiseEndpoint("51.38.80.233","http");
    loginSession.connectAndSend(sReq);
    net::HTTPAdminResponses response = net::mapAdminIntToEnum(net::parseResponse(loginSession.returnResponse()));
    switch(response)
    {
    case net::HTTPAdminResponses::loginSuccess:
        std::cout << "-- Upload successful --" << std::endl;
        return true;
        break;
    case net::HTTPAdminResponses::notAdmin:
        std::cout << "> You are not an administrator, please log in with an administrator account" << std::endl;
        return false;
        break;
    case net::HTTPAdminResponses::loginFailure:
        std::cout << "> Your password is incorrect" << std::endl;
        return false;
        break;
    case net::HTTPAdminResponses::databaseDown:
        std::cout << "> Database down, restart DB" << std::endl;
        return false;
        break;
    default:
        std::cout << "> Request failed" << std::endl;
        return false;
        break;
    }
}

t_licenses networkingManager::mapIntToLicence(int input)
{
    switch(input)
    {
    case 1:
        return t_licenses::license_none;
        break;
    case 2:
        return t_licenses::license_standard;
        break;
    case 3:
        return t_licenses::license_premium;
        break;
    default:
        std::cout << "Invalid license selected" << std::endl;
        exit(1);
    }
}

std::string networkingManager::mapLicenseToStr(t_licenses license)
{
    switch(license)
    {
    case t_licenses::license_none:
        return "license_none";
        break;
    case t_licenses::license_standard:
        return "license_standard";
        break;
    case t_licenses::license_premium:
        return "license_premium";
        break;
    default:
        return "license_standard";
        break;
    }
}
