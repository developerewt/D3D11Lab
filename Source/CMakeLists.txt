cmake_minimum_required (VERSION 3.8)

# Include 3rd party libraries
include("${CMAKE_SOURCE_DIR}/Scripts/FetchLibraries.cmake")

# Populate the list of 3rd party libraries
FetchContent_MakeAvailable(glfw)

# CREATE EXECUTABLE
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/*.h" "${CMAKE_SOURCE_DIR}/Source/*.cpp")
source_group("Source" "${CMAKE_SOURCE_DIR}/Source" ${SUBDIRECTORIES})
add_executable ("Source" main.cpp ${SUBDIRECTORIES})

# INCLUDE DIRECTORIES AND LINK LIBRARIES
target_include_directories("Source" PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries("Source" PRIVATE glfw)

# DEBUG AND RELEASE FLAGS
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions("Source" PRIVATE DEBUG)
	# target_compile_options("Source" PRIVATE -g -O0)
else()
	target_compile_definitions("Source" PRIVATE NDEBUG)
	# target_compile_options("Source" PRIVATE -O3)
endif()

# MACRO DEFINITIONS
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_definitions("Source" PUBLIC RUNTIME_API=__declspec\(dllexport\))
	target_compile_definitions("Source" PUBLIC FORCEINLINE=__forceinline)
else()
	message(FATAL_ERROR "Unsupport compiler.")
endif()