#pragma once
#include <string>
#include <sys/socket.h>
#include "request.h"
#include "response.h"

class HttpClient
{
public:
    
    HttpClient();
    
    HttpResponse Send(HttpRequest & request);
    HttpResponse Get(const std::string & url);
    
    
private:
    int sockfd_;
    bool Connect(const std::string &host, int port);
    bool Disconnect();
    bool SendRequest(HttpRequest & request);
    bool ReceiveResponse(HttpResponse & response);
    bool ReadLine(std::string &line);
    std::size_t ReadBytes(char* buffer, std::size_t length);
    
    
};