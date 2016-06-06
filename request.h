#pragma once
#include <string>
#include <vector>
#include "url.h"

class http_request
{
public:
    http_request(const std::string & method, const std::string & url);
    url get_url() const
    {
        return url_;
    }
    url get_url() 
    {
        return url_;
    }
    std::string buffer();
    

private:
    std::string buffer_;
    std::string method_;
    url url_;
    
};