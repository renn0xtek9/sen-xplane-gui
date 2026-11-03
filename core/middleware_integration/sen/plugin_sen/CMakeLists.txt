add_library(
    plugin_sen
    INTERFACE
    public/hmi/plugin_sen.h
)
target_include_directories(
    plugin_sen INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_link_libraries(
    plugin_sen
    INTERFACE hmi::plugin_interface
              sen::kernel
)
add_library(
    hmi::plugin_sen
    ALIAS
    plugin_sen
)
