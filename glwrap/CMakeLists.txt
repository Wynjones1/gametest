cmake_minimum_required(VERSION 2.8)
project(glwrap)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif()

add_definitions(-DGLEW_STATIC=1)
set(GLWRAP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDR ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(GLWRAP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/mesh" CACHE STRING "")
include_directories(${GLWRAP_INCLUDE_DIRS})

add_library(glwrap ${SRC} ${HDR})
set(GLWRAP_LIBRARIES glwrap CACHE STRING "")
