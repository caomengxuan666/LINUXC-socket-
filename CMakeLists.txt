cmake_minimum_required(VERSION 2.8.12.2)

project(SOCKET)

include_directories(include)

add_executable(server src/server.cpp src/socket.hpp src/server_socket.hpp)

add_executable(client src/client.cpp src/socket.hpp src/client_socket.hpp)

add_compile_options(-std=c++11 -o2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -g")

set(CMAKE_BUILD_TYPE Debug)
