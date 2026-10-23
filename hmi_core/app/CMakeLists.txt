qt_add_executable(
    hmi-app
    main.cpp
)

qt_add_qml_module(
    hmi-app
    URI
    SenXplaneHmi
    VERSION
    1.0
    QML_FILES
    Main.qml
)

target_link_libraries(
    hmi-app
    PRIVATE Qt6::Quick
            Qt6::Core
            hmi_sen_bridge_controller
            hmi_plugin_interface
            hmi_plugin_sen_factory
            hmi_plugin_manager
)

add_dependencies(
    hmi-app
    x_plane_aircraft_plugin
)

set_target_properties(
    hmi-app
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
               "${PROJECT_BINARY_DIR}"
)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
