cmake_minimum_required(VERSION 3.10)
project(QRIS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# --------------------------------------------------------------------
# GoogleTest (FetchContent) — required to build gtests automatically
# --------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Subprojects
add_subdirectory(src/core)
add_subdirectory(src/tests)
