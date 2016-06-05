#include "client.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/event.h>
#include <netdb.h>
#include <unistd.h>


HttpClient::HttpClient()
: sockfd_(0)
{
    
}

bool HttpClient::Connect(const std::string &hostname, int port)
{
    if (sockfd_ != 0)
    {
        Disconnect();
    }
    
    sockfd_ = socket(PF_INET, SOCK_STREAM, 0);
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

bool HttpClient::Disconnect()
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

HttpResponse HttpClient::Send(HttpRequest & request)
{
    HttpResponse response;
    Url url = request.GetUrl();
    if (Connect(url.Host(), url.Port()) && SendRequest(request))
    {
        ReceiveResponse(response);
        Disconnect();
    }
    
    return response;
}

HttpResponse HttpClient::Get(const std::string &url)
{
    HttpRequest request("GET", url);
    return Send(request);
}

bool HttpClient::SendRequest(HttpRequest &request)
{
    auto buffer = request.GetBuffer();
    if (write(sockfd_, buffer.data(), buffer.size()) == -1)
    {
        // Log error
        std::cout << "Error sending request buffer: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool HttpClient::ReceiveResponse(HttpResponse & response)
{
    bool result = false;
    int kq, event_count(0);
    // allocate kqueue to get notifications of socket events
    kq = kqueue();
    if (kq == -1)
    {
        std::cout << "Error initializing kqueue: " << strerror(errno) << std::endl;
        return false;
    }
    struct kevent monitor[1];   // to monitor
    struct kevent events[2];    // to trigger
    EV_SET(&monitor[0], sockfd_, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    
    
    std::string statusline = "", line = "";
    if (!ReadLine(statusline))
    {
        std::cout << "Error getting status line" << std::endl;
        return false;
    }
    response.ParseStatus(statusline);
    

    while(true)
    {
        event_count = kevent(kq, monitor, 1, events, 1, NULL);
        if (event_count < 0)
        {
            std::cout << "Error waiting for events: " << strerror(errno) << std::endl;
            break;
        }
        
        if (events[0].flags & EV_EOF)
        {
            // EOF reached on socket
            result = true;
            break;
        }
        
        if (events[0].flags & EV_ERROR)
        {
            std::cout << "Event Error: " << strerror(events[0].data) << std::endl;
            break;
        }
        
        if (events[0].ident == sockfd_)
        {
            // get headers
            while (ReadLine(line) )
            {
                
                if (line == "\r\n" || !response.ParseHeader(line))
                {
                    break;
                }
            }
            
            // now get the body
            auto content_length = response.Header("Content-Length");
            auto xfer_encoding = response.Header("Transfer-Encoding");
            
            if (content_length != "")
            {
                std::size_t length = std::stol(content_length);
                char buffer[length];
                ReadBytes(buffer, length);
                response.SetBody(buffer, length);
                result = true;
                break;
            }
            else if (xfer_encoding == "chunked")
            {
                ReadLine(line);
                std::size_t chunk_size = std::stol(line, 0, 16);
                while (chunk_size > 0)
                {
                    char buffer[chunk_size];
                    ReadBytes(buffer, chunk_size);
                    response.SetBody(buffer, chunk_size, true); // append to the body
                    ReadLine(line); // eat the next crlf
                    ReadLine(line);
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

std::size_t HttpClient::ReadBytes(char* buffer, std::size_t length)
{
    std::size_t bytes_read = read(sockfd_, buffer, length);
    while (bytes_read < length)
    {
        bytes_read += read(sockfd_, buffer + bytes_read, length - bytes_read);
    }
    return bytes_read;
}

bool HttpClient::ReadLine(std::string & line)
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