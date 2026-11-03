add_test(NAME hmi-app-smoke-test COMMAND $<TARGET_FILE:hmi-app> --smoke-test)

set_tests_properties(
    hmi-app-smoke-test
    PROPERTIES ENVIRONMENT
               "QT_QPA_PLATFORM=offscreen"
               LABELS
               "smoke-test"
               TIMEOUT
               10
)
