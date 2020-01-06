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

