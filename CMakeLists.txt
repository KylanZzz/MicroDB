cmake_minimum_required(VERSION 3.26)
project(MicroDB)

set(CMAKE_CXX_STANDARD 17)

add_executable(MicroDB main.cpp
        Pager.h
        Pager.cpp
        IOHandler.h
        IOHandler.cpp
        Constants.h
        Btree.h
        Btree.cpp
)
