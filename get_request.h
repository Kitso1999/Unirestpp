#ifndef __UNIRESTPP_GET_REQUEST_H__
#define __UNIRESTPP_GET_REQUEST_H__

#include "http_request.h"

namespace unirestpp {
    class GetRequest : public HTTPRequest<GetRequest> {
    public:
        GetRequest(SessionHandleType session, std::string url)
        : HTTPRequest(session, HTTPMethod::GET, url) {}
        GetRequest(SessionHandleType session, const char* url)
        : HTTPRequest(session, HTTPMethod::GET, url) {}

        GetRequest(const GetRequest&) = delete;
        GetRequest& operator=(const GetRequest&) = delete;

        virtual ~GetRequest(){}
    };
}
#endif //__UNIRESTPP_GET_REQUEST_H__