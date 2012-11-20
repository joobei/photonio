add_library(tuio TuioClient.cpp TuioServer.cpp TuioTime.cpp)
add_dependencies(tuio osc oscnetlib)
