#pragma once
#include <string>

class Url
{
public:
    Url();
    
    std::string ToString()
    {
        return urlstring_;
    }
    
    std::string Scheme()
    {
        return scheme_;
    }
    void Scheme(const std::string &scheme)
    {
        scheme_ = scheme;
    }
    
    std::string Host()
    {
        return host_;
    }
    void Host(const std::string &host)
    {
        host_ = host;
    }
    
    int Port()
    {
        return port_;
    }
    void Port(int port)
    {
        port_ = port;
    }
    
    std::string Resource()
    {
        return resource_;
    }
    void Resource(const std::string &resource)
    {
        resource_ = resource;
    }
    
    static Url Parse(const std::string &url);
private:
    std::string urlstring_;
    std::string scheme_;
    std::string host_;
    int port_;
    std::string resource_;
    
};