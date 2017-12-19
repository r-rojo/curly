project(curly)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS --std=c++11)

find_package(Boost COMPONENTS regex)

include_directories(${Boost_INCLUDE_DIRS})

set(src client.cpp
        request.cpp
        response.cpp
        url.cpp
)

if (APPLE)
    set(src ${src} event_darwin.cpp)
elseif (UNIX)
    set(src ${src} event_linux.cpp)
endif()

add_library(urly SHARED
    ${src}
)
target_link_libraries(urly ${Boost_LIBRARIES})

add_executable(curly curly.cpp)
target_link_libraries(curly urly)
