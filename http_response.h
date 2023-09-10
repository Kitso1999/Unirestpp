#ifndef __UNIRESTPP_HTTP_RESPONSE_H__
#define __UNIRESTPP_HTTP_RESPONSE_H__

#include "http_response_base.h"

#include <nlohmann/json.hpp>

#include <string>

namespace unirestpp {
    template<class T>
    class HTTPResponse : public HTTPResponseBase {
    public:

        HTTPResponse(StatusCodeType status_code, std::string raw_body)
        : HTTPResponseBase{status_code, raw_body}{}

        T GetBody() const;
    };

    template<>
    class HTTPResponse<nlohmann::json> : public HTTPResponseBase {
    public:

        HTTPResponse(StatusCodeType status_code, std::string raw_body)
        : HTTPResponseBase{status_code, raw_body}{}

        nlohmann::json GetBody() const {
            return nlohmann::json::parse(GetRawBody());
        }
    };

    template<>
    class HTTPResponse<std::string> : public HTTPResponseBase {
    public:

        HTTPResponse(StatusCodeType status_code, std::string raw_body)
        : HTTPResponseBase{status_code, raw_body}{}

        std::string GetBody() const {
            return GetRawBody();
        }
    };

    template<>
    class HTTPResponse<void> : public HTTPResponseBase {
    public:

        HTTPResponse(StatusCodeType status_code)
        : HTTPResponseBase{status_code}{}

        void GetBody() const = delete;
        std::string GetRawBody() const = delete;
    };
}

#endif //__UNIRESTPP_HTTP_RESPONSE_H__