qt_add_library(
    x-plane-aircraft
    MODULE
    aircraftbackend.cpp
    aircraftbackend.h
    aircraftplugin.cpp
    aircraftplugin.h
)

target_include_directories(x-plane-aircraft PRIVATE "${PROJECT_SOURCE_DIR}/app")

target_link_libraries(x-plane-aircraft PRIVATE Qt6::Core)

qt_add_resources(
    x-plane-aircraft
    aircraft_qml
    PREFIX
    "/aircraft"
    FILES
    AircraftPanel.qml
)

set_target_properties(
    x-plane-aircraft
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY
               "${PROJECT_BINARY_DIR}/plugins"
)
