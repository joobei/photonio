# Cmake for project keimoteGL
# Possible configuration required

cmake_minimum_required (VERSION 2.8)

set(PROJECTNAME "uikun")

project(${PROJECTNAME})

#list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
list( APPEND CMAKE_CXX_FLAGS "-std=c++11")

include_directories(include)
include_directories(lib)
include_directories(src)
include_directories(src/assets)
include_directories(/usr/include/bullet)
include_directories(/opt/local/include)
include_directories(/opt/local/include/bullet)

add_subdirectory(lib)

SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
#set(Boost_DEBUG 1)

FIND_PACKAGE(Boost COMPONENTS system thread filesystem regex date_time serialization timer chrono)
IF(Boost_FOUND)
      include_directories(${Boost_INCLUDE_DIRS})
ENDIF()

FIND_PACKAGE(Protobuf)
IF(PROTOBUF_FOUND)
    INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS src/data.proto)
ENDIF()

file(GLOB_RECURSE cpp_sources src/*.h src/*.cpp)
file(GLOB shaders shaders/*.*)

set(assetpath ${PROJECT_SOURCE_DIR}/assets)
set(shaderpath ${PROJECT_SOURCE_DIR}/shaders)

configure_file("${PROJECT_SOURCE_DIR}/shaderpath" "${PROJECT_BINARY_DIR}/shaderpath")
configure_file("${PROJECT_SOURCE_DIR}/assetpath" "${PROJECT_BINARY_DIR}/assetpath")

link_directories(/opt/local/lib)
add_executable(${PROJECTNAME} ${cpp_sources} ${shaders} ${PROTO_SRCS} ${PROTO_HDRS})

find_library(GLFW_LIB glfw PATH /opt/local/lib)
find_library(GL_LIB OpenGL)
find_library(COCOA_LIB Cocoa)

target_link_libraries(${PROJECTNAME} ${Boost_LIBRARIES} ${GLFW_LIB} ${COCOA_LIB} ${GL_LIB} GLEW assimp protobuf tuio posix ip osc pthread BulletSoftBody BulletDynamics BulletCollision LinearMath)

