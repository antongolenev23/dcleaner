include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip  # Фиксируем версию
)
FetchContent_MakeAvailable(googletest)

set(UNIT_TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/unit")

add_executable(run_tests 
    "${UNIT_TESTS_PATH}/test_globs.cpp"
    "${UNIT_TESTS_PATH}/test_analyze_execute.cpp"
)

target_include_directories(run_tests PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_test(
    NAME DcleanerTests
    COMMAND run_tests
)

target_link_libraries(run_tests PRIVATE dcleaner_code gtest_main)
