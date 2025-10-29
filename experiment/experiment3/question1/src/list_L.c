#include "list.h"

typedef struct Node {
    elemtype data;
    struct Node* next;
} Node, *NodePtr;



typedef struct {
    NodePtr head;  // 链表头指针
    NodePtr tail;  // 链表尾指针（方便插入）
} Link, *LinkPtr;


// 初始化链表
ListPtr init_list() {
    LinkPtr list = (LinkPtr)malloc(sizeof(Link));
    if (!list) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->head = list->tail = NULL;
    return (ListPtr)list;
}

// 释放链表
void free_list(ListPtr list) {
    LinkPtr L = (LinkPtr)list;
    NodePtr cur = L->head;
    while (cur) {
        NodePtr next = cur->next;
        free(cur);
        cur = next;
    }
    free(L);
}

// 判断是否为空
bool list_empty(ListPtr list) {
    LinkPtr L = (LinkPtr)list;
    return L->head == NULL;
}

// 清空链表（保留结构）
bool clear_list(ListPtr list) {
    LinkPtr L = (LinkPtr)list;
    NodePtr cur = L->head;
    while (cur) {
        NodePtr next = cur->next;
        free(cur);
        cur = next;
    }
    L->head = L->tail = NULL;
    return true;
}

// 求长度
int list_length(ListPtr list) {
    LinkPtr L = (LinkPtr)list;
    int len = 0;
    for (NodePtr cur = L->head; cur; cur = cur->next) len++;
    return len;
}

// 获取第 index 个元素（0-based）
elemtype get_elem(ListPtr list, int index) {
    LinkPtr L = (LinkPtr)list;
    if (index < 0) {
        fprintf(stderr, "Index out of range\n");
        exit(EXIT_FAILURE);
    }
    NodePtr cur = L->head;
    for (int i = 0; i < index && cur; i++)
        cur = cur->next;
    if (!cur) {
        fprintf(stderr, "Index out of range\n");
        exit(EXIT_FAILURE);
    }
    return cur->data;
}

// 定位元素，返回下标
int locate_elem(ListPtr list, elemtype e) {
    LinkPtr L = (LinkPtr)list;
    int idx = 0;
    for (NodePtr cur = L->head; cur; cur = cur->next, idx++) {
        if (cur->data == e)
            return idx;
    }
    return -1;
}

// 获取前驱节点
ElemPtr prior_elem(ListPtr list, ElemPtr cur_e) {
    LinkPtr L = (LinkPtr)list;
    if (L->head == cur_e) return NULL;
    for (NodePtr cur = L->head; cur && cur->next; cur = cur->next) {
        if (cur->next == cur_e)
            return cur;
    }
    return NULL;
}

// 获取后继节点
ElemPtr next_elem(ListPtr list, ElemPtr cur_e) {
    return cur_e ? ((NodePtr)cur_e)->next : NULL;
}

// 插入元素（在 index 处插入，0-based）
bool insert_elem(ListPtr list, int index, elemtype e) {
    LinkPtr L = (LinkPtr)list;
    if (index < 0) return false;

    NodePtr new_node = (NodePtr)malloc(sizeof(Node));
    if (!new_node) return false;
    new_node->data = e;
    new_node->next = NULL;

    // 插入头部
    if (index == 0) {
        new_node->next = L->head;
        L->head = new_node;
        if (!L->tail) L->tail = new_node;
        return true;
    }

    NodePtr prev = L->head;
    for (int i = 0; i < index - 1 && prev; i++)
        prev = prev->next;
    if (!prev) return false;

    new_node->next = prev->next;
    prev->next = new_node;
    if (!new_node->next) L->tail = new_node;

    return true;
}

// 删除 index 位置的元素
bool delete_elem(ListPtr list, int index, elemtype* deleted_e) {
    LinkPtr L = (LinkPtr)list;
    if (index < 0 || !L->head) return false;

    NodePtr to_delete = NULL;

    // 删除头节点
    if (index == 0) {
        to_delete = L->head;
        L->head = L->head->next;
        if (!L->head) L->tail = NULL;
    } else {
        NodePtr prev = L->head;
        for (int i = 0; i < index - 1 && prev; i++)
            prev = prev->next;
        if (!prev || !prev->next) return false;

        to_delete = prev->next;
        prev->next = to_delete->next;
        if (!prev->next) L->tail = prev;
    }

    if (deleted_e) *deleted_e = to_delete->data;
    free(to_delete);
    return true;
}

// 在链表尾部添加元素
void push_back(ListPtr list, elemtype e) {
    LinkPtr L = (LinkPtr)list;
    NodePtr new_node = (NodePtr)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = e;
    new_node->next = NULL;

    if (L->tail) {
        L->tail->next = new_node;
    } else {
        L->head = new_node;
    }
    L->tail = new_node;
}

// 遍历链表
void traverse_list(ListPtr list, void (*visit)(elemtype e)) {
    LinkPtr L = (LinkPtr)list;
    for (NodePtr cur = L->head; cur; cur = cur->next)
        visit(cur->data);
}

  