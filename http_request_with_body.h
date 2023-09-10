#ifndef __UNIRESTPP_HTTP_REQUEST_WITH_BODY_H__
#define __UNIRESTPP_HTTP_REQUEST_WITH_BODY_H__

#include "http_request.h"

namespace unirestpp {
    class HTTPRequestWithBody : public HTTPRequest<HTTPRequestWithBody> {
    public:

        HTTPRequestWithBody(SessionHandleType session, HTTPMethod method, const std::string &url)
        : HTTPRequest(session, method, url) {}

        HTTPRequestWithBody(SessionHandleType session, HTTPMethod method, const char* url)
        : HTTPRequest(session, method, url) {}

        virtual ~HTTPRequestWithBody() = default; 

        virtual HTTPRequestWithBody& Body(const std::string &body)
        {
            body_ = body;
            return *this;
        }

        virtual HTTPRequestWithBody& Body(const nlohmann::json &body)
        {
            body_ = body.dump();
            return *this;
        }

        virtual HTTPResponse<std::string> AsString()
        {
            curl_easy_setopt(session_, CURLOPT_POSTFIELDS, body_.c_str());
            return HTTPRequest::AsString();
        }

        virtual HTTPResponse<nlohmann::json> AsJson()
        {
            curl_easy_setopt(session_, CURLOPT_POSTFIELDS, body_.c_str());
            return HTTPRequest::AsJson();
        }
    protected:
        std::string body_{};

    };
}

#endif //__UNIRESTPP_HTTP_REQUEST_WITH_BODY_H__