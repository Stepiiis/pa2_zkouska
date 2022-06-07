cmake_minimum_required(VERSION 3.22)
project(hashtable)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(hashtable
        main.cpp CHashSet.cpp)
