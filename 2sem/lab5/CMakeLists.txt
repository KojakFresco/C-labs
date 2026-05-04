cmake_minimum_required(VERSION 3.14)
project(lab5 C)

set(CMAKE_C_STANDARD 11)

set(LAB5_C_SOURCES
    graph.c
    dijkstra.c
    priority_queue.c
)

add_library(lab5_lib STATIC ${LAB5_C_SOURCES})
target_include_directories(lab5_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(lab5_app main.c)
target_link_libraries(lab5_app PRIVATE lab5_lib)
set_target_properties(lab5_app PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(BUILD_BENCH "Build benchmark target" ON)
if(BUILD_BENCH)
    add_executable(lab5_bench bench/bench.c)
    target_link_libraries(lab5_bench PRIVATE lab5_lib)
    set_target_properties(lab5_bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/heads/main.zip
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)
    message(STATUS "googletest_SOURCE_DIR=${googletest_SOURCE_DIR}")
    # enable C++ only for tests (keep project primarily a C project)
    enable_language(CXX)
    set(CMAKE_CXX_STANDARD 17)

    enable_testing()
    add_executable(test_priority_queue tests/test_priority_queue.cpp)
    target_link_libraries(test_priority_queue PRIVATE lab5_lib GTest::gtest_main)
    target_include_directories(test_priority_queue PRIVATE ${googletest_SOURCE_DIR}/googletest/include)
    set_target_properties(test_priority_queue PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    add_test(NAME pq_test COMMAND test_priority_queue)

    add_executable(test_dijkstra tests/test_dijkstra.cpp)
    target_link_libraries(test_dijkstra PRIVATE lab5_lib GTest::gtest_main)
    target_include_directories(test_dijkstra PRIVATE ${googletest_SOURCE_DIR}/googletest/include)
    set_target_properties(test_dijkstra PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    add_test(NAME dijkstra_test COMMAND test_dijkstra)
endif()

