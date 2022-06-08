cmake_minimum_required(VERSION 3.22)
project(hashtable)

set(CMAKE_CXX_STANDARD 17)

#aux_source_directory(. DIR_SRCS) ${DIR_SRCS}
add_executable(hashtable
        CHashFitWiki.cpp  )
