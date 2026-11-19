add_library(
    plugin_manager
    SHARED
    pluginmanager.cpp
    plugin_instance_provider.cpp
    public/hmi/pluginmanager.h
    public/hmi/plugin_instance_provider.h
)
target_include_directories(
    plugin_manager PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)
target_link_libraries(
    plugin_manager
    PUBLIC hmi::plugin_interface
           Qt6::Core
)

add_library(
    hmi::plugin_manager
    ALIAS
    plugin_manager
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
