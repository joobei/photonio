add_subdirectory(win32)
add_library(ip IpEndpointName.cpp)
add_dependencies(ip oscnetlib)
