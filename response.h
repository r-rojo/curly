#pragma once
#include <string>
#include <map>
#include <vector>
#include <string>


class HttpResponse
{
public:
    HttpResponse();
    
    ~HttpResponse();
    bool ParseStatus(const std::string &status);
    bool ParseHeader(const std::string &header);
    
    const std::string& Version()
    {
        return version_;
    }
    int StatusCode()
    {
        return status_code_;
    }
    const std::string& ReasonPhrase()
    {
        return phrase_;
    }
    
    std::string Header(const std::string &key);
    
    void SetBody(char * buffer, unsigned int size, bool append = false);
    const char* GetBody();
    
private:

    std::string version_;
    std::string phrase_;
    int status_code_;
    std::map<std::string, std::string> headers_;
    std::string body_;
    
};