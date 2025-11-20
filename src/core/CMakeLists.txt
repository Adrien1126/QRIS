
add_library(qris_core STATIC
	utils/normal_distribution.cpp
)

target_include_directories(qris_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(qris_core PUBLIC cxx_std_17)
