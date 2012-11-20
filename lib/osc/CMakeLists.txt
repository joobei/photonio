add_library(osc OscOutboundPacketStream.cpp OscPrintReceivedElements.cpp OscReceivedElements.cpp OscTypes.cpp)
add_dependencies(osc ip oscnetlib)
