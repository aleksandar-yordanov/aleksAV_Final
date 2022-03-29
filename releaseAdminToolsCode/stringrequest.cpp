#include "stringrequest.h"
using namespace net;

StringRequest::StringRequest(net::method method, std::string resource, std::string ipAddress, std::string json = "")
{
    std::string jLength = std::to_string(json.length());
    switch (method)
    {
        case net::method::METHOD_POST:
            this->HTMLRequestString += "POST /"+resource+" HTTP/1.1\r\n";
            this->HTMLRequestString += "Host: "+ipAddress+"\r\n";
            this->HTMLRequestString += "Content-Type: application/x-www-form-urlencoded\r\n";
            this->HTMLRequestString += "Content-Length: " + jLength + "\r\n";
            this->HTMLRequestString += "Connection: close\r\n\r\n";  //Always double line feed for post content
            this->HTMLRequestString += json;
            break;

        case net::method::METHOD_GET:
            this->HTMLRequestString += "GET /" + resource +'?'+ json + " HTTP/1.1\r\n";
            this->HTMLRequestString += "Host: " + ipAddress + "\r\n";
            this->HTMLRequestString += "Connection: close\r\n\r\n";
            break;

        case net::method::METHOD_HEAD:
            this->HTMLRequestString += "HEAD /" + resource +" HTTP/1.1\r\n";
            this->HTMLRequestString += "Host: " + ipAddress + "\r\n";
            this->HTMLRequestString += "Connection: close\r\n\r\n";
            break;

        case net::method::METHOD_PUT:
            this->HTMLRequestString += "PUT /"+resource+" HTTP/1.1\r\n";
            this->HTMLRequestString += "Host: "+ipAddress+"\r\n";
            this->HTMLRequestString += "Content-Type: application/x-www-form-urlencoded\r\n";
            this->HTMLRequestString += "Content-Length: " + jLength + "\r\n";
            this->HTMLRequestString += "Connection: close\r\n\r\n";  //Always double line feed for post content
            this->HTMLRequestString += json;
            break;

        case net::method::METHOD_DELETE:
            this->HTMLRequestString += "DELETE /" + resource +" HTTP/1.1\r\n";
            this->HTMLRequestString += "Host: " + ipAddress + "\r\n";
            this->HTMLRequestString += "Connection: close\r\n\r\n";
            break;

        case net::method::METHOD_CONNECT:
            this->HTMLRequestString += "CONNECT /" + ipAddress +" HTTP/1.1\r\n";
            break;

        case net::method::METHOD_TRACE:
            this->HTMLRequestString += "TRACE /" + resource +" HTTP/1.1\r\n";
            this->HTMLRequestString += "Host: " + ipAddress + "\r\n";
            break;

        case net::method::METHOD_OPTIONS:
            this->HTMLRequestString += "OPTIONS / HTTP/1.1\r\n";
            break;
    }
    this->ipAddress = ipAddress;
}

std::string StringRequest::returnString()
{
    return this->HTMLRequestString;
}

StringRequest::operator std::string() const
{
    return this->HTMLRequestString;
}

std::string StringRequest::returnIpAddress()
{
    return this->ipAddress;
}
