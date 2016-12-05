cmake_minimum_required(VERSION 3.2)
project(Combinatorial_Optimization)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -O3")

include_directories(lib src)

#Any file that contains a main()
set(MAIN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/task1.cpp ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp)

file(GLOB_RECURSE SOURCE_FILES "src/**.cpp" "src/**.h" "lib/*.cpp" )
file(GLOB_RECURSE TEST_SOURCE_FILES "tests/**.cpp")

list(REMOVE_ITEM SOURCE_FILES ${MAIN_FILES})
list(REMOVE_ITEM TEST_SOURCE_FILES ${MAIN_FILES})

#TASK1
add_executable(task1 ${SOURCE_FILES} src/task1.cpp)

#TESTS
find_package(Boost 1.59.0 COMPONENTS unit_test_framework)
if(Boost_FOUND)
    include_directories( ${Boost_INCLUDE_DIR} )
    add_executable(tests ${SOURCE_FILES} ${TEST_SOURCE_FILES} tests/tests.cpp)
    target_link_libraries(tests ${Boost_LIBRARIES})
endif()