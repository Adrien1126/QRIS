enable_testing()

add_executable(unit_tests
    utils/test_normal_distribution_gtest.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        qris_core
        gtest_main
)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(GoogleTest)
gtest_discover_tests(unit_tests)
