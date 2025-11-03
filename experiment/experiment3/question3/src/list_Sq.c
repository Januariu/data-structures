#include "list.h"

#define INIT_SIZE 10
#define INCREMENT_SIZE 5

typedef struct {
    elemtype *data;
    int length;
    int capacity;
} List;

// typedef elemtype* ElemPtr;

// ============ 基本操作 ============

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

// 动态调整线性表容量
bool list_resize(List *list, int new_capacity) {
    elemtype *new_data = (elemtype *)realloc(list->data, new_capacity * sizeof(elemtype));
    if (!new_data) return false;
    list->data = new_data;
    list->capacity = new_capacity;
    return true;
}

// 判断是否为空
bool list_empty(ListPtr head) {
    List *list = (List *)head;
    return list->length == 0;
}

// 清空线性表
bool clear_list(ListPtr head) {
    List *list = (List *)head;
    if (!list) return false;
    list->length = 0;
    return true;
}

// 求长度
int list_length(ListPtr head) {
    List *list = (List *)head;
    return list->length;
}

// 获取 index 位置的元素
elemtype get_elem(ListPtr head, int index) {
    List *list = (List *)head;
    if (index < 0 || index >= list->length) {
        fprintf(stderr, "Index out of range\n");
        exit(EXIT_FAILURE);
    }
    return list->data[index];
}

// 查找元素位置，返回第一个匹配元素的索引，未找到返回 -1
int locate_elem(ListPtr head, elemtype e) {
    List *list = (List *)head;
    for (int i = 0; i < list->length; i++)
        if (list->data[i] == e) return i;
    return -1;
}

// 获取元素的前驱
ElemPtr prior_elem(ListPtr head, ElemPtr cur_e) {
    List *list = (List *)head;
    if (!list || !cur_e || (elemtype *)cur_e <= list->data) return NULL;
    return cur_e - 1;
}

// 获取元素的后继
ElemPtr next_elem(ListPtr head, ElemPtr cur_e) {
    List *list = (List *)head;
    if (!list || !cur_e || (elemtype *)cur_e >= list->data + list->length - 1) return NULL;
    return cur_e + 1;
}

// 在 index 位置插入元素
bool insert_elem(ListPtr head, int index, elemtype e) {
    List *list = (List *)head;
    if (index < 0 || index > list->length)
        return false;
    if (list->length >= list->capacity)
        if (!list_resize(list, list->capacity + INCREMENT_SIZE))
            return false;

    for (int i = list->length; i > index; i--)
        list->data[i] = list->data[i - 1];

    list->data[index] = e;
    list->length++;
    return true;
}

// 删除 index 位置的元素
bool delete_elem(ListPtr head, int index, elemtype *deleted_e) {
    List *list = (List *)head;
    if (index < 0 || index >= list->length)
        return false;
    if (deleted_e) *deleted_e = list->data[index];

    for (int i = index; i < list->length - 1; i++)
        list->data[i] = list->data[i + 1];

    list->length--;
    return true;
}

// 在表尾添加元素
void push_back(ListPtr head, elemtype e) {
    List *list = (List *)head;
    if (list->length >= list->capacity)
        list_resize(list, list->capacity + INCREMENT_SIZE);
    list->data[list->length++] = e;
}

// 遍历线性表
void traverse_list(ListPtr head, void (*visit)(elemtype e)) {
    List *list = (List *)head;
    for (int i = 0; i < list->length; i++)
        visit(list->data[i]);
}
