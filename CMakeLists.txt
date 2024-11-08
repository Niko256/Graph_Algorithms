
cmake_minimum_required(VERSION 3.30)

project(Graphs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20")

find_package(Python3 REQUIRED)

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/visualization/venv/bin/activate
    COMMAND ${Python3_EXECUTABLE} -m venv ${CMAKE_SOURCE_DIR}/visualization/venv
    COMMAND ${CMAKE_SOURCE_DIR}/visualization/venv/bin/pip install --upgrade pip
    COMMAND ${CMAKE_SOURCE_DIR}/visualization/venv/bin/pip install manim
    COMMENT "Creating Python virtual environment and installing manim"
)

add_custom_target(python_env
    DEPENDS ${CMAKE_SOURCE_DIR}/visualization/venv/bin/activate
)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(GTest REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE ALGORITHM_FILES src/algorithms/*.tpp)
file(GLOB_RECURSE GRAPH_FILES src/graph.tpp)
set(ALL_SRC_FILES ${SRC_FILES} ${ALGORITHM_FILES} ${GRAPH_FILES})

add_executable(graphs ${ALL_SRC_FILES})
target_compile_options(graphs PRIVATE -std=c++20)
target_link_libraries(graphs PRIVATE nlohmann_json::nlohmann_json)
add_dependencies(graphs python_env)

if(GTEST_FOUND)
    file(GLOB_RECURSE TEST_FILES tests/*.cpp)

    foreach(TEST_FILE ${TEST_FILES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_FILE})
        target_compile_options(${TEST_NAME} PRIVATE -std=c++20)
        target_link_libraries(${TEST_NAME} GTest::GTest GTest::Main nlohmann_json::nlohmann_json)

        enable_testing()
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()
