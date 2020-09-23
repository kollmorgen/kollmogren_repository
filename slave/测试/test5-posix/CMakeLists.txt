cmake_minimum_required (VERSION 2.6)
project (test1)

set(cmake_system_name Linux)

set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
set(CMAKE_FIND_ROOT_PATH  /home/wenhao/kollmorgen/2-下位机/tronlong/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


add_definitions("-DNOT_USE_DYNAMIC_LOADING")


add_executable(test2 time2.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(test2 Threads::Threads)
target_link_libraries(test2 -lpthread -lrt)
#set(CMAKE_EXE_LINKER_FLAGS "-lpthread -lrt -ldl -lm")
