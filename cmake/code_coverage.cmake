if(CMAKE_BUILD_TYPE
   STREQUAL
   "Debug"
)
    add_compile_options(--coverage)
    add_link_options(--coverage)

    find_program(
        GCOV_EXECUTABLE
        gcov
        REQUIRED
    )
    find_program(
        GCOVR_EXECUTABLE
        gcovr
        REQUIRED
    )

    set(COVERAGE_REPORT_DIR "${CMAKE_BINARY_DIR}/coverage-report")
    set(COVERAGE_REPORT "${COVERAGE_REPORT_DIR}/index.html")

    add_custom_command(
        OUTPUT "${COVERAGE_REPORT}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${COVERAGE_REPORT_DIR}"
        COMMAND "${CMAKE_CTEST_COMMAND}" --test-dir "${CMAKE_BINARY_DIR}" -LE code-quality
                --output-on-failure
        COMMAND
            "${GCOVR_EXECUTABLE}" --root "${CMAKE_SOURCE_DIR}" --object-directory
            "${CMAKE_BINARY_DIR}" --gcov-executable "${GCOV_EXECUTABLE}" --exclude
            "${CMAKE_BINARY_DIR}/.*" --html-details --output "${COVERAGE_REPORT}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running tests and generating the HTML coverage report"
        VERBATIM
    )

    add_custom_target(code-coverage DEPENDS "${COVERAGE_REPORT}")

    add_custom_target(
        code-quality-check
        COMMAND
            "${GCOVR_EXECUTABLE}" --root "${CMAKE_SOURCE_DIR}" --object-directory
            "${CMAKE_BINARY_DIR}" --gcov-executable "${GCOV_EXECUTABLE}" --exclude
            "${CMAKE_BINARY_DIR}/.*" --fail-under-line 100
        DEPENDS "${COVERAGE_REPORT}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        VERBATIM
    )

    add_test(NAME code-quality COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target
                                       code-quality-check
    )
    set_tests_properties(
        code-quality
        PROPERTIES LABELS
                   "code-quality"
                   WILL_FAIL
                   TRUE
    )
endif()
