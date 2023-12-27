cmake_minimum_required(VERSION 3.22)
project(serwer)

set(CMAKE_CXX_STANDARD 20)

add_executable(serwer serwer.cpp
        TCPserwer.cpp
        TCPserwer.h
        BuildResponse.cpp
        BuildResponse.h
)
