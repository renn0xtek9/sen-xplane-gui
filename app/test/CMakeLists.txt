add_test(NAME app-smoke-test COMMAND $<TARGET_FILE:sen-xplane-hmi> --smoke-test)

set_tests_properties(
    app-smoke-test
    PROPERTIES ENVIRONMENT
               "QT_QPA_PLATFORM=offscreen"
               LABELS
               "smoke-test"
               TIMEOUT
               10
)
