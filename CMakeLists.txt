cmake_minimum_required(VERSION 3.20)
project(cpp_quant CXX)

cmake_policy(SET CMP0167 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(OpenSSL REQUIRED)
find_package(Boost 1.80 REQUIRED)

add_executable(main
    src/main.cpp
    src/feed/binance_feed.cpp
)

target_include_directories(main PRIVATE src)

target_link_libraries(main PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::headers
    pthread
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(main PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
    )
    target_link_options(main PRIVATE -fsanitize=address,undefined)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(main PRIVATE -O3 -march=native)
endif()
