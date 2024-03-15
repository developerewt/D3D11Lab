cmake_minimum_required (VERSION 3.8)

include(FetchContent)

#GLFW LIBRARY
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
)