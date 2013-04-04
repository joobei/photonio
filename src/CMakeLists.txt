

SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost 1.49.0 COMPONENTS system thread filesystem asio)
IF(Boost_FOUND)
      include_directories(${Boost_INCLUDE_DIRS})
ENDIF()

FIND_PACKAGE(Protobuf)
IF(PROTOBUF_FOUND)
    INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS data.proto)
ENDIF()

add_executable(${PROJECTNAME} main.cpp eventQueue.cpp asio.cpp asio.h eventQueue.h util.h util.cpp photonio.h photonio.cpp ${PROTO_SRCS} ${PROTO_HDRS} assets/assets.h assets/assets.cpp assets/mesh.h assets/mesh.cpp assets/shader.h assets/shader.cpp "${CMAKE_SOURCE_DIR}/shaders/shader.vert" "${CMAKE_SOURCE_DIR}/shaders/shader.frag" "${CMAKE_SOURCE_DIR}/shaders/specular.vert" "${CMAKE_SOURCE_DIR}/shaders/specular.frag" "${CMAKE_SOURCE_DIR}/shaders/offscreen.frag" "${CMAKE_SOURCE_DIR}/shaders/offscreen.vert" "${CMAKE_SOURCE_DIR}/shaders/circle.vert" "${CMAKE_SOURCE_DIR}/shaders/circle.frag" "${CMAKE_SOURCE_DIR}/shaders/circle.geom")

##For some reason ${Boost_LIBRARIES} doesn't work on Ubuntu 12.10
target_link_libraries(${PROJECTNAME} ${Boost_LIBRARIES} IL ILU glfw GLEW assimp protobuf tuio oscpack GL boost_system boost_thread pthread boost_filesystem)

ADD_CUSTOM_COMMAND(TARGET ${PROJECTNAME}
          POST_BUILD

          COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/src/shaders
          COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${PROJECT_BINARY_DIR}/src/shaders

          COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/assets
          COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/src/assets
)
