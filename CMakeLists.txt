# Cmake for project keimoteGL
# Possible configuration required

cmake_minimum_required (VERSION 2.8)

include_directories(include)
include_directories(lib)

set(PROJECTNAME "photonio")

project(${PROJECTNAME})

set(${PROJECTNAME}_VERSION_MAJOR 0)
set(${PROJECTNAME}_VERSION_MINOR 3)

add_subdirectory(lib)
add_subdirectory(src)

