add_executable(x_plane_plugin_sen_component_smoke_test main.cpp)

target_link_libraries(
    x_plane_plugin_sen_component_smoke_test
    PRIVATE sen::kernel
            x_plane_plugin_sen_component
)
