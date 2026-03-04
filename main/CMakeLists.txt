cmake_minimum_required(VERSION 3.15)
project(LeetcodeTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

add_executable(leetcode_tool src/leetcodeTool.cpp)

target_include_directories(leetcode_tool PRIVATE 
    include
    ${CURL_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

target_link_libraries(leetcode_tool PRIVATE 
    CURL::libcurl 
    ${JSONCPP_LIBRARIES}
    pthread
    stdc++fs
)