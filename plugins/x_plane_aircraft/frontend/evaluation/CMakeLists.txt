qt_add_executable(
    x_plane_plugin_ui_evaluation
    main.cpp
)

qt_add_qml_module(
    x_plane_plugin_ui_evaluation
    URI
    XPlaneAircraftEvaluation
    VERSION
    1.0
    QML_FILES
    Main.qml
)

target_link_libraries(
    x_plane_plugin_ui_evaluation
    PRIVATE Qt6::Quick
            x_plane_plugin_ui
)

if(BUILD_TESTING)
    add_test(NAME x_plane_plugin_ui_evaluation_smoke_test COMMAND x_plane_plugin_ui_evaluation
                                                                  --smoke-test
    )

    set_tests_properties(
        x_plane_plugin_ui_evaluation_smoke_test
        PROPERTIES ENVIRONMENT
                   "QT_QPA_PLATFORM=offscreen"
                   LABELS
                   "smoke-test"
                   TIMEOUT
                   10
    )
endif()
