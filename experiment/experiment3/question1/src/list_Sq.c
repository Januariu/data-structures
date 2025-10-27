#include "list.h"

#define INIT_SIZE 10      // 初始容量
#define INCREMENT_SIZE 5  // 每次扩容的增量

typedef struct {
    elemtype *data;   // 存储元素的数组
    int length;       // 当前元素数量
    int capacity;     // 当前数组容量
} List;

// 初始化线性表
ListPtr init_list() {
    List *list = (List *)malloc(sizeof(List));
    if (!list) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->data = (elemtype *)malloc(INIT_SIZE * sizeof(elemtype));
    if (!list->data) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->length = 0;
    list->capacity = INIT_SIZE;
    return (ListPtr)list;
}

// 释放线性表
void free_list(ListPtr head) {
    List *list = (List *)head;
    if (list) {
        free(list->data);
        free(list);
    }
}

// 判断是否为空
bool list_empty(ListPtr head) {
    List *list = (List *)head;
    return list->length == 0;
}

// 清空线性表但保留结构体
bool clear_list(ListPtr head) {
    List *list = (List *)head;
    if (!list) return false;
    list->length = 0;
    return true;
}

// 返回长度
int list_length(ListPtr head) {
    List *list = (List *)head;
    return list->length;
}

// 获取第 index 个元素（从 0 开始）
elemtype get_elem(ListPtr head, int index) {
    List *list = (List *)head;
    if (index < 0 || index >= list->length) {
        fprintf(stderr, "Index out of range\n");
        exit(EXIT_FAILURE);
    }
    return list->data[index];
}

// 查找元素位置，返回索引（没找到返回 -1）
int locate_elem(ListPtr head, elemtype e) {
    List *list = (List *)head;
    for (int i = 0; i < list->length; i++) {
        if (list->data[i] == e)
            return i;
    }
    return -1;
}

// 返回前驱元素指针（若无则返回 NULL）
ListPtr prior_elem(ListPtr head, elemtype cur_e) {
    List *list = (List *)head;
    int index = locate_elem(head, cur_e);
    if (index <= 0)
        return NULL;
    return &list->data[index - 1];
}

// 返回后继元素指针（若无则返回 NULL）
ListPtr next_elem(ListPtr head, elemtype cur_e) {
    List *list = (List *)head;
    int index = locate_elem(head, cur_e);
    if (index < 0 || index >= list->length - 1)
        return NULL;
    return &list->data[index + 1];
}

// 插入元素到 index 位置（从 0 开始）
bool insert_elem(ListPtr head, int index, elemtype e) {
    List *list = (List *)head;
    if (index < 0 || index > list->length)
        return false;

    // 扩容检查
    if (list->length >= list->capacity) {
        elemtype *new_data = (elemtype *)realloc(list->data, (list->capacity + INCREMENT_SIZE) * sizeof(elemtype));
        if (!new_data)
            return false;
        list->data = new_data;
        list->capacity += INCREMENT_SIZE;
    }

    // 元素右移
    for (int i = list->length; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }

    // 插入新元素
    list->data[index] = e;
    list->length++;
    return true;
}

// 删除第 index 个元素
bool delete_elem(ListPtr head, int index, elemtype *deleted_e) {
    List *list = (List *)head;
    if (index < 0 || index >= list->length)
        return false;

    if (deleted_e)
        *deleted_e = list->data[index];

    // 元素左移
    for (int i = index; i < list->length - 1; i++) {
        list->data[i] = list->data[i + 1];
    }

    list->length--;
    return true;
}

// 遍历线性表
void traverse_list(ListPtr head, void (*visit)(elemtype e)) {
    List *list = (List *)head;
    for (int i = 0; i < list->length; i++) {
        visit(list->data[i]);
    }
}
