qt_add_library(
    x_plane_plugin_ui
    STATIC
    aircraftbackend.cpp
    public/x_plane_aircraft_hmi/aircraftbackend.h
)

target_include_directories(
    x_plane_plugin_ui PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)

target_link_libraries(x_plane_plugin_ui PRIVATE Qt6::Core)

qt_add_resources(
    x_plane_plugin_ui
    aircraft_qml
    PREFIX
    "/aircraft"
    FILES
    AircraftPanel.qml
)

set_target_properties(
    x_plane_plugin_ui
    PROPERTIES POSITION_INDEPENDENT_CODE
               ON
               LIBRARY_OUTPUT_DIRECTORY
               "${PROJECT_BINARY_DIR}/plugins"
)
