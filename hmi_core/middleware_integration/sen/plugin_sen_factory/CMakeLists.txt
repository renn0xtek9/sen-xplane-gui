add_library(
    hmi_plugin_sen_factory
    SHARED
    public/hmi/plugin_sen_factory.h
    plugin_sen_factory.cpp
)
target_include_directories(
    hmi_plugin_sen_factory PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_link_libraries(
    hmi_plugin_sen_factory
    PUBLIC hmi_plugin_interface
           hmi_plugin_sen
)
