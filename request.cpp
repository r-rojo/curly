#include "request.h"
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

http_request::http_request(const std::string & method, const std::string & urlstring)
: method_(method)
, buffer_("")
{
    url_ = url::parse(urlstring);
}

string http_request::buffer()
{
    if (buffer_ == "")
    {
        stringstream ss;
        ss << method_ << " " << url_.resource() << " HTTP/1.1\r\n";
        ss << "host: " << url_.host() << "\r\n";
        ss << "Accept: */*\r\n";
        ss << "User-Agent: Curly\r\n\r\n";
        
        const string & str = ss.str();
        buffer_ = ss.str();
    }

    return buffer_;
}

