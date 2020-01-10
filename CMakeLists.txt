cmake_minimum_required(VERSION 3.10)
project(mbmex CXX)

# set parameters
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O1 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
set(SOURCES
	mbmexBase.cpp
	main.cpp
	changeEvent.cpp
	debug.cpp)
find_package (Threads)
add_executable(mbmex ${SOURCES})
target_link_libraries(mbmex Threads::Threads)


INSTALL(TARGETS mbmex RUNTIME DESTINATION bin)
INSTALL(FILES mbmex.service DESTINATION /lib/systemd/system/)

