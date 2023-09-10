#ifndef __UNIRESTPP_HTTP_REQUEST_INTERFACE_H__
#define __UNIRESTPP_HTTP_REQUEST_INTERFACE_H__

#include "http_response.h"

#include <string>
#include <unordered_map>

namespace unirestpp {
    enum class HTTPMethod {
        GET = 0,
        POST,
        PUT,
        PATCH,
        HEAD,
        DELETE,
        OPTIONS,
    };

    template<class Derived, class JsonType>
    class HTTPRequestInterface {
    public:
        virtual ~HTTPRequestInterface() = default;

        virtual Derived& BasicAuth(const char* username, const char* password) = 0;
        virtual Derived& BasicAuth(const std::string &username, const std::string &password) = 0;
        virtual Derived& Header(const char* name, const char* value) = 0;
        virtual Derived& Header(const std::string &name, const std::string &value) = 0;
        virtual Derived& Headers(const std::unordered_map<std::string, std::string> &headers) = 0;
        virtual Derived& RouteParam(std::string name, std::string value) = 0;

        virtual HTTPResponse<std::string> AsString() = 0;
        virtual HTTPResponse<JsonType> AsJson() = 0;
        virtual HTTPResponse<void> AsEmpty() = 0;

        virtual HTTPMethod GetHTTPMethod() = 0;
        virtual std::string GetUrl() = 0;

        //TODO query strings methods

    private:
    };
}

#endif //__UNIRESTPP_HTTP_REQUEST_INTERFACE_H__