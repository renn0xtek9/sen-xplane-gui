find_package(
    GTest
    REQUIRED
)

include(GoogleTest)

add_executable(plugin_manager_test plugin_manager_test.cpp)
target_link_libraries(
    plugin_manager_test
    PRIVATE hmi::plugin_manager
            GTest::gtest_main
)

gtest_discover_tests(plugin_manager_test)
