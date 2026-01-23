cmake_minimum_required(VERSION 3.20)

# ------------------------------------------------------------------------------
# Project
# ------------------------------------------------------------------------------
project(mathcore VERSION 1.0 LANGUAGES CXX)

# Export compile_commands.json (VS Code / clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# Global C++ configuration
# ------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# Options
# ------------------------------------------------------------------------------
option(BUILD_TESTS "Build unit tests" ON)

# ------------------------------------------------------------------------------
# Library: qris_lib
# ------------------------------------------------------------------------------
add_library(qris_lib
    src/time/date.cpp
    src/time/calendar.cpp
    src/time/businessDayConvention.cpp
    src/time/dayCountConvention.cpp
    src/time/period.cpp
    src/time/time_arithmetic.cpp
    src/time/scheduleBuilder.cpp

    src/core/types/currency.cpp
    src/core/types/rateIndex.cpp

    src/contract/coupon/fixedCoupon.cpp
    src/contract/coupon/floatingCoupon.cpp
    src/contract/cashflow.cpp
    src/contract/leg/leg.cpp
    src/contract/leg/fixedLeg.cpp
    src/contract/leg/floatingLeg.cpp

    src/audit/fixedLegAudit.cpp
    src/audit/toString.cpp
    src/audit/floatingLegAudit.cpp
)

# Alias public (bonne pratique)
add_library(qris::qris ALIAS qris_lib)

# Includes publics
target_include_directories(qris_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# Options de compilation (par target)
target_compile_options(qris_lib
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

# Standard C++ explicite (niveau pro)
target_compile_features(qris_lib
    PUBLIC
        cxx_std_20
)

# ------------------------------------------------------------------------------
# Executables
# ------------------------------------------------------------------------------
add_executable(program
    src/main.cpp
)

target_link_libraries(program
    PRIVATE
        qris::qris
)
# ------------------------------------------------------------------------------
# Audit executables
# ------------------------------------------------------------------------------

add_executable(audit_fixed_leg
    apps/auditFixedLeg.cpp
)

target_link_libraries(audit_fixed_leg
    PRIVATE
        qris::qris
)

add_executable(audit_floating_leg
    apps/auditFloatingLeg.cpp
)

target_link_libraries(audit_floating_leg
    PRIVATE
        qris::qris
)
# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------
if (BUILD_TESTS)
    include(FetchContent)

    # Politique FetchContent propre (warning CMP0135)
    cmake_policy(SET CMP0135 NEW)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(tests
        tests/test_main.cpp

        tests/test_time/test_date.cpp
        tests/test_time/test_calendar.cpp
        tests/test_time/test_business_day_convention.cpp
        tests/test_time/test_day_count_convention.cpp
        tests/test_time/test_period.cpp
        tests/test_time/test_time_arithmetic.cpp
        tests/test_time/test_scheduleBuilder.cpp
        tests/test_time/test_schedule.cpp

        tests/test_core/types/test_currency.cpp
        tests/test_core/types/test_rateIndex.cpp

        tests/test_contract/coupon/test_fixedCoupon.cpp
        tests/test_contract/coupon/test_floatingCoupon.cpp
        tests/test_contract/test_cashflow.cpp
        tests/test_contract/leg/test_fixedLeg.cpp
        tests/test_contract/leg/test_floatingLeg.cpp
    )

    target_link_libraries(tests
        PRIVATE
            qris::qris
            gtest_main
    )

    add_test(NAME qris_tests COMMAND tests)
endif()
