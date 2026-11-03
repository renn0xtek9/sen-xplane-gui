# TODO implement a sen component here in this folder
add_sen_package(
    TARGET
    x_plane_plugin_sen_component
    MAINTAINER
    "Maxime Haselbauer"
    VERSION
    "0.0.1"
    DESCRIPTION
    "X plane aircaft HMI Sen component"
    SOURCES
    public/x_plane_aircraft_hmi/sen_component.h
    src/sen_component.cpp
    STL_FILES
    stl/x_plane_aircraft_hmi/config.stl
    PUBLIC_SYMBOLS
    IS_COMPONENT
    # PRIVATE_DEPS
    DEPS
    x_plane_plugin_ui
    sen::util
    Qt6::Core
)

target_include_directories(
    x_plane_plugin_sen_component_gen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
)
