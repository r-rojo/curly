#!/bin/bash
if [ ! -f ./curly ]; then
    g++ curly.cpp url.cpp client.cpp request.cpp response.cpp  -o curly -g3 -std=c++11
fi
./curly $@
