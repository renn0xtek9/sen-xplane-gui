
add_sen_package(
  TARGET
    hmi-sen-bridge
  MAINTAINER
    "Maxime Haselbauer"
  VERSION
    "0.0.1"
  DESCRIPTION
    "HMI Sen component bridge"
  SOURCES
    public/hmi-sen-bridge/hmi_sen_bridge.h
    public/hmi-sen-bridge/hmi_handle.h
    src/hmi_sen_bridge.cpp
    src/hmi_handle.cpp
  STL_FILES
    stl/hmi_sen_bridge/config.stl
  PUBLIC_SYMBOLS
  IS_COMPONENT
  DEPS
    sen::util
    Qt6::Core
)

target_include_directories(hmi-sen-bridge_gen
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
)
