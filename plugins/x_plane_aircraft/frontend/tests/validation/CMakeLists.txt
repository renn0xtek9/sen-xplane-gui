qt_add_executable(
    x_plane_plugin_ui_validation
    main.cpp
)

qt_add_qml_module(
    x_plane_plugin_ui_validation
    URI
    XPlaneAircraftvalidation
    VERSION
    1.0
    QML_FILES
    AircraftPanelEval.qml
    AircraftGraphEval.qml
)

target_link_libraries(
    x_plane_plugin_ui_validation
    PRIVATE Qt6::Quick
            x_plane_plugin_ui
)

if(BUILD_TESTING)
    add_test(NAME x_plane_plugin_ui_validation_smoke_test COMMAND x_plane_plugin_ui_validation
                                                                  --smoke-test
    )

    set_tests_properties(
        x_plane_plugin_ui_validation_smoke_test
        PROPERTIES ENVIRONMENT
                   "QT_QPA_PLATFORM=offscreen"
                   LABELS
                   "smoke-test"
                   TIMEOUT
                   10
    )
endif()
