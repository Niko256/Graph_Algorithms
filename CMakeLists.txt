cmake_minimum_required(VERSION 3.30)

project(Graphs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/../Data_Structures) 


find_package(GTest REQUIRED)

if(GTEST_FOUND)
    file(GLOB_RECURSE SRC_FILES src/*.cpp)
    add_executable(Graphs ${SRC_FILES})
    target_compile_options(Graphs PRIVATE -std=c++20)

    file(GLOB_RECURSE TEST_FILES tests/*.cpp)
    add_executable(basic_tests ${TEST_FILES})
    target_compile_options(basic_tests PRIVATE -std=c++20)

    target_link_libraries(basic_tests GTest::GTest GTest::Main)

    enable_testing()
    add_test(NAME basic_tests COMMAND basic_tests)
endif()
