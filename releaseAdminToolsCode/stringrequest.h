#pragma once
#include "includes.h"

namespace net
{
    enum class method
    {
        METHOD_GET,
        METHOD_POST,
        METHOD_PUT,
        METHOD_HEAD,
        METHOD_DELETE,
        METHOD_TRACE,
        METHOD_CONNECT,
        METHOD_OPTIONS
    };

    class StringRequest{
        std::string HTMLRequestString;
        std::string ipAddress;
    public:
        StringRequest(net::method method, std::string resource, std::string ipAddress,std::string json);

        operator std::string() const;

        std::string returnString();
        std::string returnIpAddress();
    };
}
