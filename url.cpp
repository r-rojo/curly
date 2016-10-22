#include "url.h"
#include <boost/regex.hpp>
#include <iostream>

using namespace boost;

const char* URL_PATTERN = "(http|https|ftp)://([\\w\\.\\-]+)(?::(\\d+))?(/.*)?";

url::url()
{
    
}


url url::parse(const std::string & urlstring)
{
    url url;
    regex rex(URL_PATTERN); 
    smatch match;
    if (regex_match(urlstring, match, rex))
    {
        url.urlstring_ = urlstring;
        url.scheme_ = match.str(1);
        url.host_ = match.str(2);
        url.port_ = stoi(match[3] == "" ? "80" : match.str(3));
        url.resource_ = match.str(4) == "" ? "/" : match.str(4);
    }
    
    return url;
}
