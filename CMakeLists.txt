cmake_minimum_required(VERSION 3.20)
project(mathcore VERSION 1.0 LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options globales de compilation (propagées à toutes les targets)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Option pour activer/désactiver les tests
option(BUILD_TESTS "Build unit tests" ON)

if (BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )

    # Empêche GoogleTest de forcer des options qui pollueraient le projet
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)
endif()

add_library(qris_lib
    src/time/date.cpp
	src/time/calendar.cpp
    src/time/businessDayConvention.cpp
    src/time/dayCountConvention.cpp
    src/core/types/currency.cpp
    src/contract/coupon/fixedCoupon.cpp
)

add_library(qris::qris ALIAS qris_lib)

target_include_directories(qris_lib
    PUBLIC
        include
)

add_executable(program src/main.cpp)

target_link_libraries(program
    PRIVATE
        qris::qris
)

if (BUILD_TESTS)
    enable_testing()

    add_executable(tests
        tests/test_main.cpp
        tests/test_time/test_date.cpp
		tests/test_time/test_calendar.cpp
        tests/test_time/test_business_day_convention.cpp
        tests/test_time/test_day_count_convention.cpp
        tests/test_core/types/test_currency.cpp
        tests/test_contract/coupon/test_fixedCoupon.cpp
    )

    target_link_libraries(tests
        PRIVATE
            qris::qris
            gtest_main
    )

    add_test(NAME qris_tests COMMAND tests)
endif()