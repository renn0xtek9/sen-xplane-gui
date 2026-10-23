add_library(
    hmi_plugin_sen
    INTERFACE
    public/hmi/plugin_sen.h
)
target_include_directories(
    hmi_plugin_sen INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_link_libraries(
    hmi_plugin_sen
    INTERFACE hmi_plugin_interface
              sen::kernel
)
