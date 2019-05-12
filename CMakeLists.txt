cmake_minimum_required (VERSION 3.10.2)
project (ProgressTracker VERSION 0.0.1 LANGUAGES CXX)

###Compiling
add_compile_options (-Wall -g)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_BUILD_TYPE Debug)
enable_testing()

###Executables
file(GLOB_RECURSE main_sources "src/*.cpp")
add_executable (main ${main_sources})

###Tests
list(REMOVE_ITEM main_sources "${CMAKE_SOURCE_DIR}/src/main.cpp")
file(GLOB_RECURSE test_sources "test/*.cpp")
add_executable (main_test ${test_sources} ${main_source})
target_include_directories (main_test PUBLIC "src")
target_link_libraries (main_test /usr/local/lib/boost_1_69_0/libboost_unit_test_framework.so)
