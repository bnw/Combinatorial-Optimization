cmake_minimum_required(VERSION 3.5)
project(Combinatorial_Optimization)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Werror -Wfatal-errors")

include_directories(lib src)

file(GLOB_RECURSE SOURCE_FILES "src/**/*.cpp" "lib/*.cpp")
file(GLOB_RECURSE TEST_SOURCE_FILES "tests/**/*.cpp")

#TASK1
add_executable(task1 ${SOURCE_FILES} src/task1.cpp)

#TESTS
find_package( Boost 1.36.0 REQUIRED COMPONENTS unit_test_framework)
if(Boost_FOUND)
    include_directories( ${Boost_INCLUDE_DIR} )
    add_executable(tests ${SOURCE_FILES} ${TEST_SOURCE_FILES} tests/tests.cpp)
    target_link_libraries(tests ${Boost_LIBRARIES})
endif()