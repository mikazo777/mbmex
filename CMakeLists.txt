cmake_minimum_required(VERSION 2.8)
project(mbmex CXX)

# set parameters
set(CMAKE_CXX_STANDARD 11)
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

