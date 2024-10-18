cmake_minimum_required(VERSION 3.30)

project(Graphs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20")

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(GTest REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)  

if(GTEST_FOUND)
    file(GLOB_RECURSE SRC_FILES src/*.cpp)
    file(GLOB_RECURSE ALGORITHM_FILES src/algorithms/*.cpp)
    set(ALL_SRC_FILES ${SRC_FILES} ${ALGORITHM_FILES})

    add_executable(Graphs ${ALL_SRC_FILES})
    target_compile_options(Graphs PRIVATE -std=c++20)
    target_link_libraries(Graphs PRIVATE nlohmann_json::nlohmann_json)  

    file(GLOB_RECURSE TEST_FILES tests/*.cpp)
    add_executable(basic_tests ${TEST_FILES})
    target_compile_options(basic_tests PRIVATE -std=c++20)
    target_link_libraries(basic_tests GTest::GTest GTest::Main nlohmann_json::nlohmann_json)  

    enable_testing()
    add_test(NAME basic_tests COMMAND basic_tests)
endif()
