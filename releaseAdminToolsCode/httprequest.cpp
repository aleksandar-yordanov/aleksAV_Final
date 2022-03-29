#include "httprequest.h"

using namespace std::chrono_literals;
using namespace net;

HTTPRequest::HTTPRequest()
{
    this->idleWork = std::make_unique<asio::io_context::work>(this->context);
    this->threadContext = std::thread([&](){context.run();});
    this->socket = std::make_unique<asio::ip::tcp::socket>(this->context);
    std::vector<char> vBuffer(1024*20);
    this->vBuffer = vBuffer;
}

HTTPRequest::~HTTPRequest()
{
    this->context.stop();
    if(this->threadContext.joinable()) this->threadContext.join();
}

void HTTPRequest::initialiseEndpoint(StringRequest req, std::string service)
{
    asio::ip::tcp::resolver resolver(this->context);
    asio::ip::tcp::resolver::query query(req.returnIpAddress(),service);
    asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    //std::cout << "initialised endpoints" << std::endl;
    this->socketEndpoint = iter->endpoint();
}

void HTTPRequest::initialiseEndpoint(std::string ipAddress, std::string service)
{
    asio::ip::tcp::resolver resolver(this->context);
    asio::ip::tcp::resolver::query query(ipAddress,service);
    asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    this->socketEndpoint = iter->endpoint();
}

void Session::grabdata()
{
    //std::lock_guard lock(this->mainMtx);
    //std::cout << "data read" << std::endl;
    this->socket->async_read_some(asio::buffer(vBuffer.data(),vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if(!ec){
                //std::cout << "\nRead " << length << " bytes\n";
                for(int i = 0; i < static_cast<int>(length); i++){
                    //std::cout << vBuffer[i];
                    this->response.push_back(vBuffer[i]);
                }
                grabdata();
            }
            else
            {
                //std::cout << ec;
            }
        }
    );
}

void Session::connectAndSend(StringRequest req)
{
    if(this->socket == nullptr)
    {
        std::cout << "nullptr:socket";
    }
    this->socket->connect(this->socketEndpoint,this->ec);
    if(!this->ec)
    {
        //std::cout<<"Service resolved" << '\n';
    }
    else if(this->ec)
    {
        std::cout << this->ec.message() << '\n';
        socket->close();
        return;
    }
    if(this->socket->is_open())
    {
        //std::cout << "Socket open" << '\n';
        this->grabdata();

        std::string sReq = std::string(req);
        this->socket->write_some(asio::buffer(sReq.data(),sReq.size()),this->ec);
        this->socket->wait(this->socket->wait_read,this->ec);
        this->grabdata();

        size_t bytes = this->socket->available();
        //std::cout << "bytes available: " << bytes << '\n';
        if(bytes > 0)
        {
            std::vector<char> vBuffer(bytes);
            this->socket->read_some(asio::buffer(vBuffer.data(),vBuffer.size()),this->ec);
        }

        this->context.stop();

        while(!this->threadContext.joinable())
        {
            void(0);
        }
        this->threadContext.join();
        //if(this->threadContext.joinable()) this->threadContext.join();
        vBuffer.clear();
        this->closeConnection();
    }
    else if(this->socket->is_open() == false)
    {
        std::cout << "socket closed" << std::endl;
    }
}

std::string Session::returnResponse()
{
    return this->response;
}

void Session::closeConnection()
{
    this->socket->close(this->ec);
}


HTTPResponses net::mapIntToEnum(int value)
{
    switch(value)
    {
        case 0:
            return net::HTTPResponses::loginFailure;
            break;
        case 1:
            return net::HTTPResponses::loginSuccess;
            break;
        case 2:
            return net::HTTPResponses::databaseDown;
            break;
        default:
            return net::HTTPResponses::unknownResponse;
            break;
    }
}

HTTPAdminResponses net::mapAdminIntToEnum(int value)
{
    switch(value)
    {
        case 0:
            return net::HTTPAdminResponses::loginFailure;
            break;
        case 1:
            return net::HTTPAdminResponses::loginSuccess;
            break;
        case 2:
            return net::HTTPAdminResponses::databaseDown;
            break;
        case 3:
            return net::HTTPAdminResponses::notAdmin;
            break;
        case 10:
            return net::HTTPAdminResponses::unknownResponse;
            break;
        default:
            return net::HTTPAdminResponses::unknownResponse;
            break;
    }
}

HTTPAdminUpgradeResponses net::mapAdminUpgradeIntToEnum(int value)
{
    switch(value)
    {
        case 0:
            return net::HTTPAdminUpgradeResponses::invalidAdminPassword;
            break;
        case 1:
            return net::HTTPAdminUpgradeResponses::changeSuccess;
            break;
        case 2:
            return net::HTTPAdminUpgradeResponses::invalidUsernameGiven;
            break;
        case 3:
            return net::HTTPAdminUpgradeResponses::notAdmin;
            break;
        case 4:
            return net::HTTPAdminUpgradeResponses::databaseDown;
            break;
        default:
            return net::HTTPAdminUpgradeResponses::unknownResponse;
            break;
    }
}

HTTPAdminResetResponses net::mapAdminResetIntToEnum(int value)
{
    switch(value)
    {
        case 0:
            return net::HTTPAdminResetResponses::invalidAdminPassword;
            break;
        case 1:
            return net::HTTPAdminResetResponses::changeSuccess;
            break;
        case 2:
            return net::HTTPAdminResetResponses::passwordLengthInvalid;
            break;
        case 3:
            return net::HTTPAdminResetResponses::notAdmin;
            break;
        case 4:
            return net::HTTPAdminResetResponses::databaseDown;
            break;
        case 5:
            return net::HTTPAdminResetResponses::invalidUsernameGiven;
            break;
        default:
            return net::HTTPAdminResetResponses::unknownResponse;
            break;
    }
}


int net::parseResponse(std::string inputStr)
{
    int initPos = inputStr.find("$");
    std::string curStr = "";
    if(initPos != static_cast<int>(std::string::npos))
    {
        for(unsigned long i = 1; i < (inputStr.length()-1)-initPos;i++)
        {
            if(inputStr[initPos+i] == '?'){
                break;
            }
            else{
                curStr += inputStr[initPos+i];
            }
        }
    }
    try {
        return std::stoi(curStr);
    } catch (std::invalid_argument) {
        return std::stoi("10");
    }
}
