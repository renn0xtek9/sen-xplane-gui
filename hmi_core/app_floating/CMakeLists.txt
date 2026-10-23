find_package(
    Qt6
    6.10
    REQUIRED
    COMPONENTS Core
               Quick
               QuickWidgets
               Widgets
)

qt_add_executable(
    hmi-app-floating
    main.cpp
)

target_link_libraries(
    hmi-app-floating
    PRIVATE Qt6::Core
            Qt6::Quick
            Qt6::QuickWidgets
            Qt6::Widgets
            hmi_sen_bridge_controller
            hmi_plugin_interface
            hmi_plugin_sen_factory
            hmi_plugin_manager
)

add_dependencies(
    hmi-app-floating
    x_plane_aircraft_plugin
)

set_target_properties(
    hmi-app-floating
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
               "${PROJECT_BINARY_DIR}"
)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
