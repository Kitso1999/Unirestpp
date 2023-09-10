#ifndef __UNIRESTPP_HTTP_RESPONSE_BASE_H__
#define __UNIRESTPP_HTTP_RESPONSE_BASE_H__

#include <string>

namespace unirestpp {
    class HTTPResponseBase {
    public:
        using StatusCodeType = long;

        explicit HTTPResponseBase(StatusCodeType status_code)
        : status_code_{status_code} {}
        HTTPResponseBase(StatusCodeType status_code, std::string raw_body)
        : status_code_{status_code}, raw_body_{raw_body} {}
        virtual ~HTTPResponseBase() = default;

        inline std::string GetRawBody() const { return raw_body_; };
        inline StatusCodeType GetStatus() const { return status_code_; };

    protected:
        StatusCodeType status_code_{200};
        std::string raw_body_{};
    };
}
#endif //__UNIRESTPP_HTTP_RESPONSE_BASE_H__