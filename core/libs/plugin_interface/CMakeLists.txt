add_library(
    plugin_interface
    INTERFACE
)
target_include_directories(
    plugin_interface INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_link_libraries(plugin_interface INTERFACE Qt6::Core)

add_library(
    hmi::plugin_interface
    ALIAS
    plugin_interface
)
