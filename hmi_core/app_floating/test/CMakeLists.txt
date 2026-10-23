add_test(NAME hmi-app-floating-smoke-test COMMAND $<TARGET_FILE:hmi-app-floating> --smoke-test)

set_tests_properties(
    hmi-app-floating-smoke-test
    PROPERTIES ENVIRONMENT
               "QT_QPA_PLATFORM=offscreen"
               LABELS
               "smoke-test"
               TIMEOUT
               10
)
