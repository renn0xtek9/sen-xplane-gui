find_package(
    Qt6
    REQUIRED
    COMPONENTS Core
)

add_library(
    sen_bridge_controller
    SHARED
    sen_bridge_controller.cpp
    public/hmi/sen_bridge_controller.h
)

target_include_directories(
    sen_bridge_controller PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)

target_link_libraries(
    sen_bridge_controller
    PUBLIC Qt6::Core
           sen::kernel
           sen::util
           hmi::plugin_interface
           hmi::sen_component
           hmi::plugin_sen
)
add_library(
    hmi::sen_bridge_controller
    ALIAS
    sen_bridge_controller
)
