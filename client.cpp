#include "client.h"
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include "event.h"


http_client::http_client()
: sockfd_(0)
{
    
}

bool http_client::connect_socket(const std::string &hostname, int port)
{
    if (sockfd_ != 0)
    {
        disconnect_socket();
    }
    
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *host;
    
    host = gethostbyname(hostname.c_str());
    
    struct sockaddr_in server;
    
    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    bcopy((char *)host->h_addr, (char *)&server.sin_addr.s_addr, host->h_length);
    if (connect(sockfd_, (struct sockaddr *) &server,sizeof(server)) != 0)
    {
        // Log error, use stdout for now
        std::cout << "Error connecting to host: " << hostname << "(" << strerror(errno) << ")" << std::endl;
        return false;
    }
    return true;
}

bool http_client::disconnect_socket()
{
    if (close(sockfd_) != 0)
    {
        // log error
        std::cout << "Error closing socket" << std::endl;
        return false;
    }
    sockfd_ = 0;
    return true;
}

http_response http_client::send_request(http_request & request)
{
    http_response response;
    url url = request.get_url();
    if (connect_socket(url.host(), url.port()) && send_request_buffer(request))
    {
        receive_response(response);
        disconnect_socket();
    }
    
    return response;
}

http_response http_client::get(const std::string &url)
{
    http_request request("GET", url);
    return send_request(request);
}

bool http_client::send_request_buffer(http_request &request)
{
    auto buffer = request.buffer();
    if (write(sockfd_, buffer.data(), buffer.size()) == -1)
    {
        // Log error
        std::cout << "Error sending request buffer: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool http_client::receive_response(http_response & response)
{
    bool result = false;
    event ev(sockfd_);
    
    std::string statusline = "", line = "";
    if (!socket_readline(statusline))
    {
        std::cout << "Error getting status line" << std::endl;
        return false;
    }
    response.parse_status(statusline);
    

    while(true)
    {
        if (ev.wait(100))
        {
            // get headers
            while (socket_readline(line) )
            {
                
                if (line == "\r\n" || !response.parse_header(line))
                {
                    break;
                }
            }
            
            // now get the body
            auto content_length = response.header("Content-Length");
            auto xfer_encoding = response.header("Transfer-Encoding");
            
            if (content_length != "")
            {
                std::size_t length = std::stol(content_length);
                char buffer[length];
                socket_readbytes(buffer, length);
                response.SetBody(buffer, length);
                result = true;
                break;
            }
            else if (xfer_encoding == "chunked")
            {
                socket_readline(line);
                std::size_t chunk_size = std::stol(line, 0, 16);
                while (chunk_size > 0)
                {
                    char buffer[chunk_size];
                    socket_readbytes(buffer, chunk_size);
                    response.SetBody(buffer, chunk_size, true); // append to the body
                    socket_readline(line); // eat the next crlf
                    socket_readline(line);
                    chunk_size = std::stol(line, 0, 16);
                }
                result = true;
                break;
            }
            
            
        }
        usleep(10);
    }
    
    close(kq);
    return result;
}

std::size_t http_client::socket_readbytes(char* buffer, std::size_t length)
{
    std::size_t bytes_read = read(sockfd_, buffer, length);
    while (bytes_read < length)
    {
        bytes_read += read(sockfd_, buffer + bytes_read, length - bytes_read);
    }
    return bytes_read;
}

bool http_client::socket_readline(std::string & line)
{
    char buf[2];
    bool cr(false), lf(false);
    line = "";
    while(read(sockfd_, buf, 1) > 0)
    {
        if (!cr && !lf)
        {
            if (buf[0] == '\r')
            {
                cr = true;
            }
        }
        else if (cr && !lf)
        {
            if (buf[0] == '\n')
            {
                lf = true;
            }
            else
            {
                cr = false;
            }
        }
        line += buf[0];
        
        if (cr && lf)
        {
            return true;
            
        }
        
    }
    return false;
    
}
