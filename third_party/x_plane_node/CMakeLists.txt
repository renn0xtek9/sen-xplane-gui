cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(
    x_plane_node_project
    VERSION 0.0.1
    LANGUAGES CXX
              C
)

if(DEFINED
   ENV{SEN_PATH}
)
    list(
        APPEND
        CMAKE_PREFIX_PATH
        "$ENV{SEN_PATH}/cmake"
    )
endif()

find_package(
    sen
    REQUIRED
)

add_sen_package(
    TARGET
    x_plane_node
    MAINTAINER
    "<your name goes here>"
    VERSION
    "0.0.1"
    DESCRIPTION
    "<package description goes here>"
    SOURCES
    src/x_plane_node.h
    src/x_plane_node.cpp
    STL_FILES
    stl/x_plane_node/x_plane_node.stl
    stl/x_plane_node/basic_types.stl
)
