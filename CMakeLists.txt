
cmake_minimum_required(VERSION 3.14)
project(WINX)

option(WINX_BUILD_EXAMPLE "Build the provided example program" ON)

if(WINX_BUILD_EXAMPLE)
	message(STATUS "WINX example will be build")
else()
	message(STATUS "WINX example will NOT be build")
endif()

add_library(winx winx.c)
include_directories(${CMAKE_CURRENT_LIST_DIR})

if(WIN32)
	find_package(OpenGL REQUIRED)
	target_link_libraries(winx PRIVATE OpenGL::GL)
endif()

if(UNIX)
	target_link_libraries(winx PRIVATE ${CMAKE_DL_LIBS})
	target_compile_options(winx PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

if(WINX_BUILD_EXAMPLE)
	add_subdirectory(example)
endif()
