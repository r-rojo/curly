#pragma once
#include <string>
#include <sys/socket.h>
#include "request.h"
#include "response.h"

class http_client
{
public:
    
    http_client();
    
    http_response send_request(http_request & request);
    http_response get(const std::string & url);
    
    
private:
    int sockfd_;
    bool connect_socket(const std::string &host, int port);
    bool disconnect_socket();
    bool send_request_buffer(http_request & request);
    bool receive_response(http_response & response);
    bool socket_readline(std::string &line);
    std::size_t socket_readbytes(char* buffer, std::size_t length);
    
    
};