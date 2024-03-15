cmake_minimum_required (VERSION 3.8)

project ("D3D11Lab")

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)

#COMPILER VERSION CHECK
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	message(STATUS "MSVC Version: ${MSVC_VERSION}")
else()
	message(FATAL_ERROR "Compiler not supported.")
endif()

add_subdirectory ("Source")
