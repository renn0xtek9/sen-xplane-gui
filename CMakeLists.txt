cmake_minimum_required(VERSION 3.21)

project(
    hmi-app
    VERSION 0.1.0
    LANGUAGES CXX
)

include(CTest)

list(
    APPEND
    CMAKE_PREFIX_PATH
    "${SEN_PREFIX}"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(
    sen
    CONFIG
    REQUIRED
)
find_package(
    Qt6
    6.10
    REQUIRED
    COMPONENTS Quick
)

qt_standard_project_setup(
    REQUIRES
    6.10
)

add_subdirectory(hmi_core)
add_subdirectory(plugins/x_plane_aircraft)
