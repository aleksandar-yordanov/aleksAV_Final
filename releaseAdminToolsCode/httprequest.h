#pragma once
#include "includes.h"
#include "stringrequest.h"

namespace net{

    enum class HTTPResponses
    {
        loginSuccess,
        loginFailure,
        databaseDown,
        unknownResponse,
    };

    enum class HTTPAdminResponses
    {
        loginSuccess,
        notAdmin,
        loginFailure,
        databaseDown,
        unknownResponse
    };

    enum class HTTPAdminUpgradeResponses
    {
        changeSuccess,
        notAdmin,
        invalidUsernameGiven,
        invalidAdminPassword,
        databaseDown,
        unknownResponse
    };

    enum class HTTPAdminResetResponses
    {
        changeSuccess,
        notAdmin,
        invalidUsernameGiven,
        passwordLengthInvalid,
        invalidAdminPassword,
        databaseDown,
        unknownResponse
    };

    HTTPResponses mapIntToEnum(int value);
    HTTPAdminResponses mapAdminIntToEnum(int value);
    HTTPAdminUpgradeResponses mapAdminUpgradeIntToEnum(int value);
    HTTPAdminResetResponses mapAdminResetIntToEnum(int value);
    int parseResponse(std::string inputStr);
}

namespace net
{
class HTTPRequest { //abstract HTTPRequest Class.
protected:
    std::thread threadContext;
    asio::error_code ec;
    asio::io_context context;
    std::unique_ptr<asio::io_context::work> idleWork;
    std::unique_ptr<asio::ip::tcp::socket> socket;
    asio::ip::tcp::endpoint socketEndpoint;

public:
    explicit HTTPRequest(); //Preps context and keeps it from destruction with idle work on a seperate thread.
    ~HTTPRequest();
public:
    void initialiseEndpoint(StringRequest req, std::string service);
    void initialiseEndpoint(std::string ipAddress, std::string service);
    virtual void grabdata() = 0;
    virtual void connectAndSend(StringRequest req) = 0;
    virtual void closeConnection() = 0;
    virtual std::string returnResponse() = 0;
    std::vector<char> vBuffer;
};

class Session : public HTTPRequest
{
    std::string response;
public:
    void grabdata();
    void connectAndSend(StringRequest req);
    void closeConnection();
    std::string returnResponse();
    std::mutex mainMtx;
};
}
