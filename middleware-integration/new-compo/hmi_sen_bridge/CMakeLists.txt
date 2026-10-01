
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
    public/hmi_sen_bridge/hmi_handle.h
    src/hmi_handle.cpp
  STL_FILES
    stl/hmi_sen_bridge/hmi_handle.stl
  PUBLIC_SYMBOLS
  DEPS
    sen::util
)

target_include_directories(hmi_handle_gen
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
)


add_sen_package(
  TARGET
    hmi_sen_bridge
  MAINTAINER
    "Maxime Haselbauer"
  VERSION
    "0.0.1"
  DESCRIPTION
    "HMI Sen component bridge"
  SOURCES
    public/hmi_sen_bridge/hmi_sen_bridge.h
    src/hmi_sen_bridge.cpp
  STL_FILES
    stl/hmi_sen_bridge/config.stl
  PUBLIC_SYMBOLS
  IS_COMPONENT
  PRIVATE_DEPS
    hmi_handle
  DEPS
    sen::util
    Qt6::Core
)

target_include_directories(hmi_sen_bridge_gen
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
)
