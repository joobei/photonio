# Cmake for project keimoteGL
# Possible configuration required

cmake_minimum_required (VERSION 2.8)

set(PROJECTNAME "uikun")

project(${PROJECTNAME})

list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

include_directories(include)
include_directories(lib)
include_directories(src)
include_directories(src/assets)

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


add_executable(${PROJECTNAME} ${cpp_sources} ${shaders} ${PROTO_SRCS} ${PROTO_HDRS})

##For some reason ${Boost_LIBRARIES} doesn't work on Ubuntu 12.10
target_link_libraries(${PROJECTNAME} ${Boost_LIBRARIES} IL ILU glfw GLEW assimp protobuf tuio posix ip osc GL pthread BulletCollision LinearMath)

