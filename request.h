#pragma once
#include <string>
#include <vector>
#include "url.h"

class HttpRequest
{
public:
    HttpRequest(const std::string & method, const std::string & url);
    Url GetUrl() const
    {
        return url_;
    }
    Url GetUrl() 
    {
        return url_;
    }
    std::string GetBuffer();
    

private:
    std::string buffer_;
    std::string method_;
    Url url_;
    
};