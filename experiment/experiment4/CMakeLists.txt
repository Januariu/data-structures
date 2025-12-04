cmake_minimum_required(VERSION 3.10)
project(claude_gen_string C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)

# 包含头文件目录
include_directories(include)

# 主程序可执行文件
add_executable(main
    src/main.c
    src/blockchain.c
)

# 测试程序可执行文件
add_executable(test_string
    src/test_string.c
    src/blockchain.c
)

# 启用测试
enable_testing()
add_test(NAME test_string_unit COMMAND test_string)