qt_add_library(
    x_plane_aircraft_plugin
    MODULE
    aircraftplugin.cpp
    public/x_plane_aircraft_hmi/aircraftplugin.h
)

target_include_directories(
    x_plane_aircraft_plugin PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)

target_link_libraries(
    x_plane_aircraft_plugin
    PRIVATE Qt6::Core
            hmi_plugin_sen
            x_plane_plugin_sen_component
            x_plane_plugin_ui
)

set_target_properties(
    x_plane_aircraft_plugin
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY
               "${PROJECT_BINARY_DIR}/plugins"
)
