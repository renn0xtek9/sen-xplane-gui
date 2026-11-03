add_library(
    hmi_plugin_manager
    SHARED
    pluginmanager.cpp
    public/hmi/pluginmanager.h
)
target_include_directories(
    hmi_plugin_manager PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_link_libraries(
    hmi_plugin_manager
    PUBLIC hmi_plugin_interface
           Qt6::Core
)
