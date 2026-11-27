execute_process(
    COMMAND "${COVERAGE_CTEST_COMMAND}" --test-dir "${COVERAGE_BUILD_DIR}" -LE code-quality
            --output-on-failure RESULT_VARIABLE TEST_RESULT
)

execute_process(
    COMMAND
        "${COVERAGE_GCOVR}" --root "${COVERAGE_SOURCE_DIR}" --object-directory
        "${COVERAGE_BUILD_DIR}" --gcov-executable "${COVERAGE_GCOV}" --exclude
        "${COVERAGE_BUILD_DIR}/.*" --html-details --output "${COVERAGE_HTML}"
    RESULT_VARIABLE GCOVR_RESULT
)

if(NOT
   "${GCOVR_RESULT}"
   STREQUAL
   "0"
)
    message(FATAL_ERROR "gcovr failed to generate ${COVERAGE_HTML} (exit code ${GCOVR_RESULT})")
endif()

if(NOT
   "${TEST_RESULT}"
   STREQUAL
   "0"
)
    message(FATAL_ERROR "Tests failed (exit code ${TEST_RESULT}); coverage report was generated")
endif()
