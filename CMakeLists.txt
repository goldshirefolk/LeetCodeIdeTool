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
    main/src/leetcodeTool.cpp
    main/src/utils/api.cpp
    main/src/utils/config.cpp
    main/src/utils/stringUtils.cpp
    main/src/utils/languages.cpp
    main/src/utils/general.cpp
)

target_include_directories(lct PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lct PRIVATE
        CURL::libcurl
        PkgConfig::JSONCPP
        $<$<NOT:$<PLATFORM_ID:Darwin>>:stdc++fs>
)
