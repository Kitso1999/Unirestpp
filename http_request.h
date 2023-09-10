#ifndef __UNIRESTPP_HTTP_REQUEST_H__
#define __UNIRESTPP_HTTP_REQUEST_H__

#include "http_request_interface.h"

#include <string>
#include <unordered_map>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace unirestpp {

    static std::size_t HTTPWriteCallback(void *contents, std::size_t size, std::size_t nmemb, void *userp)
    {
        static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
        return size * nmemb;
    }

    template<class Derived>
    class HTTPRequest : public HTTPRequestInterface<Derived, nlohmann::json>{
    public:

        using SessionHandleType = CURL*;
        using HeaderListType = curl_slist;

        HTTPRequest(SessionHandleType session, HTTPMethod method, const std::string &url)
        : session_{session}, method_{method}, url_{url}
        { 
            Reset();
        }

        HTTPRequest(SessionHandleType session, HTTPMethod method, const char* url)
        : session_{session}, method_{method}, url_{url}
        {
            Reset();
        }

        HTTPRequest(const HTTPRequest&) = delete;
        HTTPRequest& operator=(const HTTPRequest&) = delete;

        virtual ~HTTPRequest()
        {
            Reset();
        }

        virtual Derived& BasicAuth(const char* username, const char* password) override
        {
            curl_easy_setopt(session_, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
            curl_easy_setopt(session_, CURLOPT_USERNAME, username);
            curl_easy_setopt(session_, CURLOPT_PASSWORD, password);
            
            return *static_cast<Derived*>(this);
        }

        virtual Derived& BasicAuth(const std::string &username, const std::string &password) override
        {
            curl_easy_setopt(session_, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
            curl_easy_setopt(session_, CURLOPT_USERNAME, username);
            curl_easy_setopt(session_, CURLOPT_PASSWORD, password);

            return *static_cast<Derived*>(this);
        }

        virtual Derived& Header(const char* name, const char* value) override
        {

            std::string header_string{name};
            header_string.append(": ");
            header_string.append(value);

            header_list_ = curl_slist_append(header_list_, header_string.c_str());

            return *static_cast<Derived*>(this);
        }

        virtual Derived& Header(const std::string &name, const std::string &value) override
        {
            std::string header_string{name};
            header_string.append(": ");
            header_string.append(value);

            header_list_ = curl_slist_append(header_list_, header_string.c_str());

            return *static_cast<Derived*>(this);
        }

        virtual Derived& Headers(const std::unordered_map<std::string, std::string> &headers) override
        {
            for(auto& header : headers) {
                Header(header.first, header.second);
            }

            return *static_cast<Derived*>(this);
        }

        virtual Derived& RouteParam(std::string name, std::string value) override
        {
            size_t left{}, right{};
            left = url_.find_first_of('{');
            right = url_.find_first_of('}');

            while(left != std::string::npos && right != std::string::npos) {
                std::string_view param{&url_[left + 1], right - left - 1};
                if(param == name) {
                    url_.erase(left, right - left + 1);
                    url_.insert(left, value);
                    left = right = left + value.size() - 1;
                }

                left = url_.find_first_of('{', left);
                right = url_.find_first_of('}', right);
            }

            return *static_cast<Derived*>(this);
        }

        virtual HTTPResponse<std::string> AsString() override
        {
            std::string response_string{};

            curl_easy_setopt(session_, CURLOPT_URL, url_.c_str());
            curl_easy_setopt(session_, CURLOPT_WRITEFUNCTION, HTTPWriteCallback);
            curl_easy_setopt(session_, CURLOPT_WRITEDATA, &response_string);

            if(header_list_) {
                curl_easy_setopt(session_, CURLOPT_HTTPHEADER, header_list_);
            }

            curl_easy_perform(session_);

            HTTPResponseBase::StatusCodeType status;

            curl_easy_getinfo(session_, CURLINFO_RESPONSE_CODE, &status);
            
            return HTTPResponse<std::string>(status, response_string);
        }

        virtual HTTPResponse<nlohmann::json> AsJson() override
        {
            std::string response_string{};

            curl_easy_setopt(session_, CURLOPT_URL, url_.c_str());
            curl_easy_setopt(session_, CURLOPT_WRITEFUNCTION, HTTPWriteCallback);
            curl_easy_setopt(session_, CURLOPT_WRITEDATA, &response_string);

            if(header_list_) {
                curl_easy_setopt(session_, CURLOPT_HTTPHEADER, header_list_);
            }

            curl_easy_perform(session_);

            HTTPResponseBase::StatusCodeType status;

            curl_easy_getinfo(session_, CURLINFO_RESPONSE_CODE, &status);
            
            return HTTPResponse<nlohmann::json>(status, response_string);
        }

        virtual HTTPResponse<void> AsEmpty() override
        {
            curl_easy_setopt(session_, CURLOPT_URL, url_.c_str());

            if(header_list_) {
                curl_easy_setopt(session_, CURLOPT_HTTPHEADER, header_list_);
            }

            curl_easy_perform(session_);

            HTTPResponseBase::StatusCodeType status;

            curl_easy_getinfo(session_, CURLINFO_RESPONSE_CODE, &status);
            
            return HTTPResponse<void>(status);
        }

        virtual HTTPMethod GetHTTPMethod() override { return method_;}
        virtual std::string GetUrl() override {return url_;}

        void Reset()
        {
            if(header_list_) {
                curl_slist_free_all(header_list_);
                header_list_ = nullptr;
            }

            curl_easy_reset(session_);
            ApplyMethodType();
        }

        //TODO query strings methods
    protected:

        void ApplyMethodType()
        {
            switch(method_) {
                case HTTPMethod::GET:
                    curl_easy_setopt(session_, CURLOPT_CUSTOMREQUEST, "GET");
                    break;
                case HTTPMethod::POST:
                    curl_easy_setopt(session_, CURLOPT_CUSTOMREQUEST, "POST");
                    break;
                case HTTPMethod::PUT: 
                    curl_easy_setopt(session_, CURLOPT_CUSTOMREQUEST, "PUT");
                    break;
                case HTTPMethod::PATCH:
                    curl_easy_setopt(session_, CURLOPT_CUSTOMREQUEST, "PATCH");
                    break;
                case HTTPMethod::DELETE:
                    curl_easy_setopt(session_, CURLOPT_CUSTOMREQUEST, "DELETE");
                    break;
                case HTTPMethod::HEAD:
                    curl_easy_setopt(session_, CURLOPT_CUSTOMREQUEST, "HEAD");
                    break;
                case HTTPMethod::OPTIONS:
                    curl_easy_setopt(session_, CURLOPT_CUSTOMREQUEST, "OPTIONS");
                    break;
            }
        }

        SessionHandleType session_{};
        HTTPMethod method_{};
        std::string url_{};
        HeaderListType* header_list_{};
    };
}

#endif // __UNIRESTPP_HTTP_REQUEST_H__
