qt_add_executable(sen-xplane-hmi
    main.cpp
    pluginmanager.cpp
    pluginmanager.h
    plugininterface.h
)

qt_add_qml_module(sen-xplane-hmi
    URI SenXplaneHmi
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_link_libraries(sen-xplane-hmi PRIVATE Qt6::Quick sen-bridge-controller)

add_dependencies(sen-xplane-hmi x-plane-aircraft)
