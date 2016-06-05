#!/bin/bash
if [ ! -f ./curly ]; then
    g++ curly.cpp url.cpp client.cpp request.cpp response.cpp  -o curly -g3 -std=c++11
fi
if [ $# -eq 0 ]; then
    echo "***********  http://www.httpbin.org/html **************"
    ./curly http://www.httpbin.org/html
    echo "***********  http://www.httpbin.org/deny **************"
    ./curly http://www.httpbin.org/deny
    echo "***********  http://www.httpbin.org/ip **************"
    ./curly http://www.httpbin.org/ip
    echo "***********  http://www.httpbin.org/user-agent **************"
    ./curly http://www.httpbin.org/user-agent
    echo "***********  http://www.httpbin.org/headers **************"
    ./curly http://www.httpbin.org/headers
    echo "***********  http://www.httpbin.org/get **************"
    ./curly http://www.httpbin.org/get
    echo "***********  http://www.httpbin.org/xml **************"
    ./curly http://www.httpbin.org/xml
else
    ./curly $@
fi
