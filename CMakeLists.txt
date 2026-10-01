cmake_minimum_required(VERSION 3.21)

project(sen-xplane-hmi VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.10 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(sen-xplane-hmi
    app/main.cpp
)

qt_add_qml_module(sen-xplane-hmi
    URI SenXplaneHmi
    VERSION 1.0
    QML_FILES
        app/Main.qml
)

target_link_libraries(sen-xplane-hmi PRIVATE Qt6::Quick)

if(BUILD_TESTING)
    add_subdirectory(app/test)
endif()
