add_library(
    plugin_sen_factory
    SHARED
    public/hmi/plugin_sen_factory.h
    plugin_sen_factory.cpp
)
target_include_directories(
    plugin_sen_factory PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_link_libraries(
    plugin_sen_factory
    PUBLIC hmi::plugin_interface
           hmi::plugin_sen
)
add_library(
    hmi::plugin_sen_factory
    ALIAS
    plugin_sen_factory
)
