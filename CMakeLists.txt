cmake_minimum_required(VERSION 3.22)

project(monkelang LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

include("cmake/googletest.cmake")

add_subdirectory(src)
add_subdirectory(test)
