qt_add_executable(
    aircraft_frontend_test
    aircraft_frontend_test.cpp
)
find_package(
    Qt6
    REQUIRED
    COMPONENTS Test
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_test(NAME aircraft_frontend_test COMMAND aircraft_frontend_test)

target_link_libraries(
    aircraft_frontend_test
    PUBLIC x_plane_plugin_ui
    PRIVATE Qt::Test
            x_plane_plugin_ui
            hmi::plugin_interface
)
