add_library(
    hmi_plugin_interface
    INTERFACE
)
target_include_directories(
    hmi_plugin_interface INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_link_libraries(hmi_plugin_interface INTERFACE Qt6::Core)
