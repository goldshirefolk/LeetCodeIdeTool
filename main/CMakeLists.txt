cmake_minimum_required(VERSION 3.15)
project(LeetcodeTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Dependencies
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
# This creates a target named PkgConfig::JSONCPP
pkg_check_modules(JSONCPP REQUIRED IMPORTED_TARGET jsoncpp)

add_executable(lct
    src/leetcode_tool.cpp
    src/api.cpp
    src/config.cpp
    src/string_utils.cpp
    src/languages.cpp
    src/general.cpp
    src/commands.cpp
)

target_include_directories(lct PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_link_libraries(lct PRIVATE
        CURL::libcurl
        PkgConfig::JSONCPP
        $<$<NOT:$<PLATFORM_ID:Darwin>>:stdc++fs>
)
