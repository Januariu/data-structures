cmake_minimum_required(VERSION 3.10)
project(question1 LANGUAGES C)


# Include 头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)


# 可配置选项（允许命令行指定实现类型）
set(LIST_IMPL "Sq" CACHE STRING "List implementation type: Sq (sequential) or L (linked)")
set(QUEUE_IMPL "Sq" CACHE STRING "Queue implementation type: Sq (sequential) or L (linked)")


# 根据选项选择对应的源文件
if(LIST_IMPL STREQUAL "L")
    set(LIST_SRC src/list_L.c)
else()
    set(LIST_SRC src/list_Sq.c)
endif()

if(QUEUE_IMPL STREQUAL "L")
    set(QUEUE_SRC src/queue_L.c)
else()
    set(QUEUE_SRC src/queue_Sq.c)
endif()


# 打印当前配置（提示信息）
message(STATUS "Using list implementation: ${LIST_IMPL}")
message(STATUS "Using queue implementation: ${QUEUE_IMPL}")


# 定义最终要编译的可执行文件
add_executable(main
    src/main.c
    ${LIST_SRC}
    ${QUEUE_SRC}
    src/stack_Sq.c
)

