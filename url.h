#pragma once
#include <string>

class url
{
public:
    url();
    
    std::string to_string()
    {
        return urlstring_;
    }
    
    std::string scheme()
    {
        return scheme_;
    }
    void scheme(const std::string &scheme)
    {
        scheme_ = scheme;
    }
    
    std::string host()
    {
        return host_;
    }
    void host(const std::string &host)
    {
        host_ = host;
    }
    
    int port()
    {
        return port_;
    }
    void port(int port)
    {
        port_ = port;
    }
    
    std::string resource()
    {
        return resource_;
    }
    void resource(const std::string &resource)
    {
        resource_ = resource;
    }
    
    static url parse(const std::string &url);
private:
    std::string urlstring_;
    std::string scheme_;
    std::string host_;
    int port_;
    std::string resource_;
    
};