add_sen_package(
    TARGET
    hmi_handle
    MAINTAINER
    "Maxime Haselbauer"
    VERSION
    "0.0.1"
    DESCRIPTION
    "HMI Sen object handle"
    SOURCES
    public/hmi/hmi_handle.h
    src/hmi_handle.cpp
    STL_FILES
    stl/hmi/hmi_handle.stl
    PUBLIC_SYMBOLS
    DEPS
    sen::util
)

target_include_directories(hmi_handle_gen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)

add_sen_package(
    TARGET
    sen_component
    MAINTAINER
    "Maxime Haselbauer"
    VERSION
    "0.0.1"
    DESCRIPTION
    "HMI Sen component bridge"
    SOURCES
    public/hmi/sen_component.h
    src/sen_component.cpp
    STL_FILES
    stl/hmi/config.stl
    PUBLIC_SYMBOLS
    IS_COMPONENT
    PRIVATE_DEPS
    hmi_handle
    DEPS
    sen::util
    Qt6::Core
)

target_include_directories(sen_component_gen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)

add_library(
    hmi::sen_component
    ALIAS
    sen_component
)
