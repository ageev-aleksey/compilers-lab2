
find_package(GTest REQUIRED)
file(GLOB SRC_FILES "./src/*.cpp")
add_executable(Tests ${SRC_FILES})
target_link_libraries(Tests LIB::Grammar gtest_main gtest pthread)