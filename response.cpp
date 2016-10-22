#include "response.h"
#include <boost/regex.hpp>
#include <iostream>
#include <algorithm>

using namespace boost;
using namespace std;
const char* STATUSLINE_PATTERN = "(.*?)\\s(.*?)\\s(.*?)\\r\\n";
const char* HEADER_PATTERN = "(.*?):\\s(.*?)\\r\\n";


bool ci_compare(char lhs, char rhs)
{
    return ::tolower(lhs) == ::tolower(rhs);
}

http_response::http_response() :
    version_(""),
    phrase_(""),
    status_code_(0),
    headers_(),
    body_("")
{
    
}

http_response::~http_response()
{

}

bool http_response::parse_status(const std::string & status)
{
    regex rex(STATUSLINE_PATTERN);
    smatch match;
    if (regex_match(status, match, rex))
    {
        version_ = match.str(1);
        status_code_ = stoi(match.str(2));
        phrase_ = match.str(3);
        return true;
    }
    cout << "could not parse status: " << status << endl;
    return false;
}

bool http_response::parse_header(const std::string & header)
{
    regex rex(HEADER_PATTERN);
    smatch match;
    if (regex_match(header, match, rex))
    {
        headers_[match.str(1)] = match.str(2);
        return true;
    }
    cout << "[Cound not parse header] " << header << endl;

    return false;
}

std::string http_response::header(const string &key)
{
    auto it = headers_.find(key);
    if (it == headers_.end())
    {
        for(auto it : headers_)
        {
            if (it.first.size() == key.size() &&
                std::equal(it.first.begin(), it.first.end(), key.begin(), ci_compare))
            {
                return it.second;
            }

        }
        return "";
    }
    return it->second;
}

void http_response::SetBody(char * buffer, unsigned int length, bool append)
{
    if (!append)
    {
        body_.assign(buffer, length);
    }
    else
    {
        body_.append(buffer, length);
    }
}

const char* http_response::body()
{
    return body_.data();
}

std::size_t http_response::body_size()
{
    return body_.size();
}
