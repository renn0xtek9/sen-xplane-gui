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
    set(GCOVR_COMMON_ARGS
        --root
        "${CMAKE_SOURCE_DIR}"
        --object-directory
        "${CMAKE_BINARY_DIR}"
        --gcov-executable
        "${GCOV_EXECUTABLE}"
        --exclude
        "${CMAKE_BINARY_DIR}/.*"
    )

    add_custom_target(
        code-coverage
        COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${COVERAGE_REPORT_DIR}"
        COMMAND
            "${CMAKE_COMMAND}" "-DCOVERAGE_CTEST_COMMAND=${CMAKE_CTEST_COMMAND}"
            "-DCOVERAGE_BUILD_DIR=${CMAKE_BINARY_DIR}" "-DCOVERAGE_SOURCE_DIR=${CMAKE_SOURCE_DIR}"
            "-DCOVERAGE_GCOV=${GCOV_EXECUTABLE}" "-DCOVERAGE_GCOVR=${GCOVR_EXECUTABLE}"
            "-DCOVERAGE_HTML=${COVERAGE_REPORT}" -P "${CMAKE_CURRENT_LIST_DIR}/run_coverage.cmake"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running tests and generating the HTML coverage report"
        VERBATIM
    )

    add_custom_target(
        code-quality-check
        COMMAND "${GCOVR_EXECUTABLE}" ${GCOVR_COMMON_ARGS} --fail-under-line 100
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        VERBATIM
    )
    add_dependencies(
        code-quality-check
        code-coverage
    )

    add_test(NAME code-quality COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target
                                       code-quality-check
    )
    set_tests_properties(
        code-quality
        PROPERTIES LABELS
                   "code-quality"
    )
endif()
