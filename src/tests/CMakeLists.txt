enable_testing()

# Collect all test sources under src/tests (recursively). Using CONFIGURE_DEPENDS
# so CMake will re-run when test files are added/removed during development.
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/tests/*.cpp
)

add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(unit_tests
    PRIVATE
        qris_core
        gtest_main
)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(GoogleTest)
gtest_discover_tests(unit_tests)
