
file(GLOB SRC_FILES "./grammar/*.cpp")

add_library(grammar ${SRC_FILES})
target_include_directories(grammar PUBLIC ${INCLUDE_DIR})
add_library(LIB::Grammar ALIAS grammar)
