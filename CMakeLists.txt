# Cmake for project keimoteGL
# Possible configuration required

cmake_minimum_required (VERSION 2.8)

add_definitions("-std=c++11 -fpermissive")

include_directories(include)
include_directories(lib)
include_directories(src)
include_directories(src/assets)

set(PROJECTNAME "photonio")

project(${PROJECTNAME})

set(${PROJECTNAME}_VERSION_MAJOR 0)
set(${PROJECTNAME}_VERSION_MINOR 3)

add_subdirectory(lib)
add_subdirectory(src)

