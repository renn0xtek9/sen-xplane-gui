find_package(sen CONFIG REQUIRED)

add_library(hmi-sen-bridge SHARED
    hmi_sen_bridge.cpp
)

target_include_directories(hmi-sen-bridge
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(hmi-sen-bridge
    PRIVATE
        SEN_TARGET_NAME="hmi-sen-bridge"
        SEN_TARGET_MAINTAINER="renn0xtek9"
        SEN_TARGET_DESCRIPTION="HMI Sen bridge component"
        SEN_TARGET_VERSION="0.1.0"
        SEN_COMPILER_STRING="${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
        GIT_REF_SPEC="unknown"
        GIT_HASH="unknown"
        GIT_STATUS="clean"
)

target_link_libraries(hmi-sen-bridge PUBLIC sen::kernel)
