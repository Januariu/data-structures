cmake_minimum_required(VERSION 3.10)
project(claude_gen_string C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)

# 包含头文件目录
include_directories(include)

# 搜集所有源文件
file(GLOB SRC_FILES "src/*.c")

# 生成可执行文件
add_executable(main ${SRC_FILES})