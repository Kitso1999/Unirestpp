#ifndef __UNIRESTPP_UNIRESTPP_H__
#define __UNIRESTPP_UNIRESTPP_H__

#include "get_request.h"
#include "http_request_with_body.h"

#include <curl/curl.h>

#include <cassert>
#include <string>

namespace unirestpp 
{

    class Unirestpp
    {
    public:
        using SessionHandleType = CURL*;

        Unirestpp(const Unirestpp&) = delete;
        Unirestpp(Unirestpp&&) = delete;
        Unirestpp& operator=(const Unirestpp&) = delete;
        Unirestpp& operator=(Unirestpp&&) = delete;

        static Unirestpp& GetInstance()
        {
            static Unirestpp instance;
            return instance;
        }

        GetRequest Get(const std::string &url) const {return GetRequest(session_, url);}
        GetRequest Get(const char *url) const {return GetRequest(session_, url);}

        HTTPRequestWithBody Post(const std::string &url) const 
        {
            return HTTPRequestWithBody(session_, HTTPMethod::POST, url);
        }
        HTTPRequestWithBody Post(const char *url) const
        {
            return HTTPRequestWithBody(session_, HTTPMethod::POST, url);
        }

        HTTPRequestWithBody Put(const std::string &url) const 
        {
            return HTTPRequestWithBody(session_, HTTPMethod::PUT, url);
        }
        HTTPRequestWithBody Put(const char *url) const
        {
            return HTTPRequestWithBody(session_, HTTPMethod::PUT, url);
        }

        HTTPRequestWithBody Patch(const std::string &url) const 
        {
            return HTTPRequestWithBody(session_, HTTPMethod::PATCH, url);
        }
        HTTPRequestWithBody Patch(const char *url) const
        {
            return HTTPRequestWithBody(session_, HTTPMethod::PATCH, url);
        }

        HTTPRequestWithBody Delete(const std::string &url) const 
        {
            return HTTPRequestWithBody(session_, HTTPMethod::DELETE, url);
        }
        HTTPRequestWithBody Delete(const char *url) const
        {
            return HTTPRequestWithBody(session_, HTTPMethod::DELETE, url);
        }

        HTTPRequestWithBody Head(const std::string &url) const 
        {
            return HTTPRequestWithBody(session_, HTTPMethod::HEAD, url);
        }
        HTTPRequestWithBody Head(const char *url) const
        {
            return HTTPRequestWithBody(session_, HTTPMethod::HEAD, url);
        }

        HTTPRequestWithBody Options(const std::string &url) const 
        {
            return HTTPRequestWithBody(session_, HTTPMethod::OPTIONS, url);
        }
        HTTPRequestWithBody Options(const char *url) const
        {
            return HTTPRequestWithBody(session_, HTTPMethod::OPTIONS, url);
        }

    private:
        Unirestpp()
        {
            CURLcode status = curl_global_init(CURL_GLOBAL_ALL);
            if(status != CURLE_OK) assert("Failed to globally initialize curl");

            session_ = curl_easy_init();
            if(!session_) assert("Failed to initialize curl");
        }

        ~Unirestpp()
        {
            curl_easy_cleanup(session_);
            session_ = nullptr;

            curl_global_cleanup();
        }

        SessionHandleType		session_        {nullptr};

    };
}

#endif //__UNIRESTPP_UNIRESTPP_H__
