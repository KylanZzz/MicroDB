cmake_minimum_required(VERSION 3.26)
project(MicroDBTests)

include(FetchContent)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(catch2)

add_executable(tests
        test.cpp
        ../src/Btree.h
        ../src/Btree.cpp
        ../src/FileHeap.cpp
        ../src/FileHeap.h
        ../src/Pager.cpp
        ../src/Pager.h
        ../src/IOHandler.cpp
        ../src/IOHandler.h
        ../src/Constants.h
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)