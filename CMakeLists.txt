cmake_minimum_required(VERSION 3.15)
project(laba2)

option(COMPILE_TESTS "" ON)
option(USE_RAPID_JSON "" OFF)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

if(${USE_RAPID_JSON})
    find_package(RapidJson REQUIRED)
    add_compile_definitions(USE_JSON_BUILDER)
endif()

add_subdirectory("./src")

if(${COMPILE_TESTS})
    add_subdirectory("./tests")
endif()

