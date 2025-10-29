#ifndef QUEUE_H
#define QUEUE_H
#include "list.h"

typedef void* queue;

/**
 * @brief 初始化一个队列。
 * 
 * 为队列动态分配内存并调用 init_list() 初始化内部线性表。
 * 将队列长度 length 置为 0，表示队列为空。
 * 
 * 若内存分配失败则程序终止。
 * 
 * @return queue 初始化成功则返回新队列对象。
 */
queue init_queue();

/**
 * @brief 释放队列所占用的内存空间。
 * 
 * 释放队列内部线性表及其本身的结构体空间。
 * 调用该函数后，队列对象将失效。
 * 
 * @param Q 要释放的队列。
 */
void free_queue(queue Q);

/**
 * @brief 清空队列中的所有元素。
 * 
 * 调用 free_list() 删除队列中的所有节点，
 * 然后重新初始化内部线性表并将 length 重置为 0。
 * 
 * @param Q 要清空的队列。
 */
void clear_queue(queue Q);

/**
 * @brief 判断队列是否为空。
 * 
 * 检查队列的 length 是否为 0。
 * 
 * @param Q 要检查的队列。
 * @return bool 若队列为空返回 true，否则返回 false。
 */
bool queue_empty(queue Q);

/**
 * @brief 获取队列中元素的个数。
 * 
 * 返回 length 成员的值，即当前队列的长度。
 * 
 * @param Q 要检查的队列。
 * @return int 队列中元素的数量。
 */
int queue_length(queue Q);

/**
 * @brief 获取队头元素。
 * 
 * 返回队列首元素（即线性表第 0 个位置的元素）。
 * 
 * 若队列为空则可能导致程序异常，调用者应在使用前判断 queue_empty()。
 * 
 * @param Q 要操作的队列。
 * @return elemtype 队头元素的值。
 */
elemtype get_head(queue Q);

/**
 * @brief 将元素 e 入队。
 * 
 * 调用 push_back() 将元素 e 插入到队尾。
 * 并将队列长度 length 加 1。
 * 
 * @param Q 队列对象。
 * @param e 要入队的元素。
 */
void enqueue(queue Q, elemtype e);

/**
 * @brief 将队头元素出队。
 * 
 * 删除队列首元素（线性表第 0 个位置）并返回其值。
 * 若队列为空则行为未定义，调用者需自行判断。
 * 
 * @param Q 队列对象。
 * @return elemtype 出队元素的值。
 */
elemtype dequeue(queue Q);

/**
 * @brief 遍历队列中的所有元素。
 * 
 * 从队头到队尾依次访问每个元素，并对每个元素执行 visit 回调函数。
 * 
 * @param Q 队列对象。
 * @param visit 指向用户自定义访问函数的函数指针。
 */
void queue_traverse(queue Q, void (*visit)(elemtype));

#endif //QUEUE_H