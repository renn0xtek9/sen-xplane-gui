find_package(
    GTest
    REQUIRED
)

include(GoogleTest)

qt_add_library(
    plugin_manager_test_utils
    STATIC
    dummy_frontend.h
    dummy_plugins.h
)
target_link_libraries(
    plugin_manager_test_utils
    PRIVATE Qt6::Core
            hmi::plugin_interface
)

add_executable(plugin_manager_test plugin_manager_test.cpp)
target_link_libraries(
    plugin_manager_test
    PRIVATE hmi::plugin_manager
            plugin_manager_test_utils
            GTest::gtest_main
)

gtest_discover_tests(plugin_manager_test)
