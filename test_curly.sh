#!/bin/bash
if [ $# -eq 0 ]; then
    echo "***********  http://www.google.com (is chunked) **************"
    ./curly http://www.google.com
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
    echo "***********  http://www.httpbin.org/html save to curly.txt **************"
    ./curly http://www.httpbin.org/html curly.txt
    open curly.txt
    echo "***********  http://www.httpbin.org/image.png save to image.png **************"
    ./curly http://www.httpbin.org/image/png image.png
    open image.png
else
    ./curly $@
fi
