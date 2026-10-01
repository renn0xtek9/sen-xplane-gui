find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(sen-bridge-controller SHARED
    sen_bridge_controller.cpp
)

target_include_directories(sen-bridge-controller
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sen-bridge-controller
    PUBLIC
        Qt6::Core
        sen::kernel
        hmi-sen-bridge
)
