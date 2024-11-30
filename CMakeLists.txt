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


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/visualization/run_plots.sh
              ${CMAKE_CURRENT_BINARY_DIR}/run_plots.sh
              COPYONLY)


include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(GTest REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)

file(GLOB_RECURSE ALGORITHM_FILES src/algorithms/*.tpp)
file(GLOB_RECURSE GRAPH_FILES src/graph.tpp)

add_library(graphs_lib STATIC
    ${ALGORITHM_FILES}
    ${GRAPH_FILES}
)

set_target_properties(graphs_lib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(graphs_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(graphs_lib PUBLIC nlohmann_json::nlohmann_json)

add_executable(graphs src/main.cpp)
target_link_libraries(graphs PRIVATE graphs_lib)
add_dependencies(graphs python_env)

if(GTEST_FOUND)
    enable_testing()
    
    file(GLOB_RECURSE TEST_FILES tests/*.cpp)
    
    add_executable(tests ${TEST_FILES})
    target_link_libraries(tests 
        PRIVATE 
        graphs_lib 
        GTest::GTest 
        GTest::Main
    )

    
    add_custom_command(TARGET tests POST_BUILD
        COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/run_plots.sh
    )   

    gtest_discover_tests(tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
        PROPERTIES
            LABELS "unit"
            TIMEOUT 10  
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(tests PRIVATE --coverage)
        target_link_options(tests PRIVATE --coverage)
    endif()
    
    add_custom_target(check 
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --verbose
        DEPENDS tests
    )
endif()
