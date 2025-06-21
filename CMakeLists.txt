cmake_minimum_required(VERSION 3.22.1)
project(PlexiStruct)

set(CMAKE_CXX_STANDARD 23)

find_package(xtensor REQUIRED)

# 1. Find Graphviz Include Directory
find_path(GRAPHVIZ_INCLUDE_DIR
        NAMES graphviz/cgraph.h # A common header file in Graphviz
        HINTS /usr/include /usr/local/include
        PATH_SUFFIXES graphviz # Graphviz often puts its headers in a 'graphviz' subdirectory
)

INCLUDE_DIRECTORIES(SYSTEM "/usr/include/graphviz/") # So you kinda need to install graphviz in your library

message(DIR " ${GRAPHVIZ_INCLUDE_DIR}")

add_executable(PlexiStruct main.cpp
        include/engine/value.hpp
        include/engine/utils.hpp
)


target_include_directories(PlexiStruct PUBLIC ${xtensor_INCLUDE_DIRS})
target_link_libraries(PlexiStruct PUBLIC xtensor cgraph gvc)
