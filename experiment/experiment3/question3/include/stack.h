#ifndef STACK_H
#define STACK_H
#include "list.h"

typedef void* stack;


/**
 * @brief 初始化一个顺序栈。
 * 
 * 为栈动态分配内存，并调用 init_list() 初始化内部线性表。
 * 将栈顶指针 top 置为 0，表示栈为空。
 * 
 * @return stack 初始化成功则返回新栈的指针，若内存分配失败返回 NULL。
 */
stack init_stack();

/**
 * @brief 判断栈是否为空。
 * 
 * 调用底层线性表的 list_empty() 函数。
 * 若栈中无元素则返回 true，否则返回 false。
 * 
 * @param s 要检查的栈。
 * @return bool 若栈为空返回 true，否则返回 false。
 */
bool stack_empty(stack s);

/**
 * @brief 获取栈中元素个数。
 * 
 * 调用 list_length() 获取线性表中元素数量。
 * 
 * @param s 要检查的栈。
 * @return int 当前栈中元素的数量。
 */
int stack_length(stack s);

/**
 * @brief 清空顺序栈。
 * 
 * 调用 clear_list() 删除所有元素，并将栈顶指针 top 重置为 0。
 * 栈的结构仍然保留，可继续使用。
 * 
 * @param s 要清空的栈。
 */
void clear_stack(stack s);

/**
 * @brief 释放顺序栈占用的内存。
 * 
 * 先调用 clear_list() 释放内部元素，再释放整个栈结构体。
 * 释放后该栈指针将失效。
 * 
 * @param s 要释放的栈。
 */
void free_stack(stack s);

/**
 * @brief 向栈中压入一个元素。
 * 
 * 将元素 e 插入到栈顶（即线性表尾部），并将 top 加 1。
 * 若操作成功返回 true。
 * 
 * @param s 栈对象。
 * @param e 要压入的元素。
 * @return bool 入栈成功返回 true；当前实现未检测栈满情况。
 */
bool push(stack s, elemtype e);

/**
 * @brief 弹出栈顶元素。
 * 
 * 若栈为空则打印错误信息并终止程序。
 * 否则通过 get_top() 获取栈顶元素，将其保存至 *e，
 * 并调用 delete_elem() 删除对应元素。
 * 
 * @param s 栈对象。
 * @return 出栈成功返回a。
 */
elemtype pop(stack s);

/**
 * @brief 获取栈顶元素的值（不出栈）。
 * 
 * 若栈为空则终止程序。
 * 否则返回栈顶（top - 1 位置）元素的值。
 * 
 * @param s 栈对象。
 * @return elemtype 栈顶元素的值。
 */
elemtype get_top(stack s);

/**
 * @brief 遍历栈中所有元素。
 * 
 * 从栈底到栈顶依次访问每个元素，并对每个元素执行 visit 回调函数。
 * 
 * @param s 栈对象。
 * @param visit 指向用户自定义访问函数的函数指针。
 */
void traverse_stack(stack s, void (*visit)(elemtype e));

#endif // STACK_H