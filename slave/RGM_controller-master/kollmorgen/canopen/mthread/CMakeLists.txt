# set(CMAKE_C_COMPILER "g++")
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_MTHREAD 变量
aux_source_directory(. DIR_SRCS_MTHREAD)
# 生成链接库
add_library (mthread ${DIR_SRCS_MTHREAD})

