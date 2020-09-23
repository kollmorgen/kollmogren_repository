cmake_minimum_required (VERSION 2.6)
project (test3)

set(cmake_system_name Linux)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


add_definitions("-DNOT_USE_DYNAMIC_LOADING")



add_executable(test2 test5.c)

find_package(Threads REQUIRED)
target_link_libraries(test2 Threads::Threads)
target_link_libraries(test2 -lpthread -lrt)

#set(CMAKE_EXE_LINKER_FLAGS "-lpthread -lrt -ldl -lm")


