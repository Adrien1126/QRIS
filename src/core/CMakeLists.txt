
## Build qris_core from all implementation files under src/core
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qris_core STATIC ${CORE_SOURCES})

target_include_directories(qris_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(qris_core PUBLIC cxx_std_17)
