cmake_minimum_required(VERSION 2.8)
project(searchEvent CXX)

# set parameters
set(CMAKE_CXX_STANDARD 11)
set(SOURCES
	../mbmexBase.cpp
	searchEvent.cpp
	../debug.cpp)
find_package (Threads)
add_executable(searchEvent ${SOURCES})
target_link_libraries(searchEvent Threads::Threads)


INSTALL(TARGETS searchEvent RUNTIME DESTINATION bin)

#set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
#set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)
#set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

#add_custom_target(gcov
#	COMMAND mkdir -p coverage
#	COMMAND {$CMAKE_MAKE_PROGRAM} test
#	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})


