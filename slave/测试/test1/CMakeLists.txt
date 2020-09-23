cmake_minimum_required (VERSION 2.6)
project (test1)


set(cmake_system_name Linux)
set(CMAKE_EIGEN_PATH  /home/wenhao/kollmorgen/2-下位机/tronlong/eigen-eigen-3)
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
set(CMAKE_FIND_ROOT_PATH  /home/wenhao/kollmorgen/2-下位机/tronlong/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
include_directories(${CMAKE_EIGEN_PATH})

add_definitions("-DNOT_USE_DYNAMIC_LOADING")


add_executable(test23 test1.cpp)
