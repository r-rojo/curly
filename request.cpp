#include "request.h"
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

HttpRequest::HttpRequest(const std::string & method, const std::string & urlstring)
: method_(method)
, buffer_("")
{
    url_ = Url::Parse(urlstring);
}

string HttpRequest::GetBuffer()
{
    if (buffer_ == "")
    {
        stringstream ss;
        ss << method_ << " " << url_.Resource() << " HTTP/1.1\r\n";
        ss << "Host: " << url_.Host() << "\r\n";
        ss << "Accept: */*\r\n";
        ss << "User-Agent: Curly\r\n\r\n";
        
        const string & str = ss.str();
        buffer_ = ss.str();
    }

    return buffer_;
}

