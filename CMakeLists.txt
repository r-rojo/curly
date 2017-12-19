project(curly)

cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CodeCoverage)

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


find_package(GTest)
if (GTEST_FOUND)
    add_executable(test_curly tests.cpp)
    include_directories(${GTEST_INCLUDE_DIRS})
    target_link_libraries(test_curly urly ${GTEST_BOTH_LIBRARIES})
    option(BUILD_COVERAGE "enable coverage build" OFF)
    if(BUILD_COVERAGE)
        setup_target_for_coverage(NAME curly_coverage
                                  EXECUTABLE test_curly --gtest_output=xml
                                  DEPENDENCIES urly)
        append_coverage_compiler_flags()
    endif()
endif()
