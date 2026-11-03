find_package(
    Qt6
    REQUIRED
    COMPONENTS Core
)

add_library(
    hmi_sen_bridge_controller
    SHARED
    sen_bridge_controller.cpp
    public/hmi/sen_bridge_controller.h
)

target_include_directories(
    hmi_sen_bridge_controller PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)

target_link_libraries(
    hmi_sen_bridge_controller
    PUBLIC Qt6::Core
           sen::kernel
           sen::util
           hmi_plugin_interface
           hmi_sen_component
           hmi_plugin_sen
)
