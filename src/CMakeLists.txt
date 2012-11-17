

SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE( Boost 1.49.0 COMPONENTS system thread filesystem asio)
IF(Boost_FOUND)
      INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
ENDIF()

FIND_PACKAGE(Protobuf)
IF(PROTOBUF_FOUND)
    INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS data.proto)
ENDIF()

include_directories(C:/dev/winddk/inc)
include_directories(C:/dev/glew-1.7.0/include)
include_directories(C:/dev/glfw-2.7.7/include)
include_directories(C:/dev/winddk/inc/api)
include_directories(C:/dev/boost_1_49_0)
include_directories(C:/dev/assimp/include)
include_directories(C:/dev/devil-x86-1.7.8/include)
include_directories(${CMAKE_SOURCE_DIR}/src/assets)
include_directories(${CMAKE_SOURCE_DIR}/src)


link_directories(C:/dev/TUIO_CPP/Release)
link_directories(C:/dev/oscpacklib/Release)
link_directories(C:/dev/glfw-2.7.7/support/msvc100/Debug)
link_directories(C:/dev/glew-1.7.0/lib)
link_directories(C:/dev/winddk/lib/win7/i386)
link_directories(C:/dev/boost_1_49_0/stage/lib)
link_directories(C:/dev/protobuf-2.4.1/vsprojects/Debug)
link_directories(C:/dev/protobuf-2.4.1/vsprojects/Release)
link_directories(C:/dev/assimp/lib/assimp_debug-dll_win32)
link_directories(C:/dev/Assimpsdk/bin/x86)
link_directories(C:/dev/devil-x86-1.7.8/lib)

add_executable(${PROJECTNAME} wiimote.cpp main.cpp eventQueue.cpp asio.cpp asio.h eventQueue.h util.h util.cpp photonio.h photonio.cpp 
${PROTO_SRCS} ${PROTO_HDRS} assets/assets.h assets/assets.cpp assets/mesh.h assets/mesh.cpp assets/shader.h assets/shader.cpp 
"${CMAKE_SOURCE_DIR}/shaders/shader.vert" "${CMAKE_SOURCE_DIR}/shaders/shader.frag" 
"${CMAKE_SOURCE_DIR}/shaders/specular.vert"  "${CMAKE_SOURCE_DIR}/shaders/specular.frag" 
"${CMAKE_SOURCE_DIR}/shaders/offscreen.frag" "${CMAKE_SOURCE_DIR}/shaders/offscreen.vert" 
"${CMAKE_SOURCE_DIR}/shaders/texader.frag" "${CMAKE_SOURCE_DIR}/shaders/texader.vert" 
)

##"${CMAKE_SOURCE_DIR}/shaders/normals.vert" "${CMAKE_SOURCE_DIR}/shaders/normals.geom" "${CMAKE_SOURCE_DIR}/shaders/normals.frag"   
target_link_libraries(${PROJECTNAME} debug devil ilu ilut glfw debug glew32d optimized glew32 assimp ${Boost_LIBRARIES} debug libprotobufd optimized libprotobuf oscnetlib ip osc tuio opengl32 winmm)

ADD_CUSTOM_COMMAND(TARGET ${PROJECTNAME}
          POST_BUILD

		  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/debug/glew32d.dll ${PROJECT_BINARY_DIR}
		  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/debug/Assimp32d.dll ${PROJECT_BINARY_DIR}
		  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/Release/Assimp32.dll ${PROJECT_BINARY_DIR}
		  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/debug/DevIL.dll ${PROJECT_BINARY_DIR}
		  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/debug/GLFW.dll ${PROJECT_BINARY_DIR}
		  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/debug/ILU.dll ${PROJECT_BINARY_DIR}
		  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/debug/ILUT.dll ${PROJECT_BINARY_DIR}

		  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/src/shaders
          COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${PROJECT_BINARY_DIR}/src/shaders

          COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/src/assets
          COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/src/assets

		  #COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${PROJECTNAME}.exe C:/Users/jubei/
)

#set(CMAKE_INSTALL_PREFIX "C:\Users\jubei\Google Drive\dev")
#INSTALL(TARGETS ${PROJECTNAME} RUNTIME DESTINATION C:\Users\jubei\Google Drive\dev)
