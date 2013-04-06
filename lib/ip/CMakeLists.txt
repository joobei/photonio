add_subdirectory(posix)
add_library(ip IpEndpointName.cpp)
add_dependencies(ip oscnetlib)
