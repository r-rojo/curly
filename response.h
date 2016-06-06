#pragma once
#include <string>
#include <map>
#include <vector>
#include <string>


class http_response
{
public:
    http_response();
    
    ~http_response();
    bool parse_status(const std::string &status);
    bool parse_header(const std::string &header);
    
    const std::string& version()
    {
        return version_;
    }
    int status_code()
    {
        return status_code_;
    }
    const std::string& reason_phrase()
    {
        return phrase_;
    }
    
    std::string header(const std::string &key);
    
    void SetBody(char * buffer, unsigned int size, bool append = false);
    const char* body();
    std::size_t body_size();
    
private:

    std::string version_;
    std::string phrase_;
    int status_code_;
    std::map<std::string, std::string> headers_;
    std::string body_;
    
};