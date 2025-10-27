#include "list.h"

typedef struct Node {
    elemtype data;
    struct Node* next;
} Node, *NodePtr;

// 初始化链表
ListPtr init_list() {
    NodePtr head = (NodePtr)malloc(sizeof(Node));
    if (head == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    head->next = NULL;
    return (ListPtr)head; 
}

// 释放链表
void free_list(ListPtr list) {
    NodePtr current = (NodePtr)list; 
    while (current != NULL) {
        NodePtr next = current->next;
        free(current);
        current = next;
    }
}

// 判断空
bool is_empty(ListPtr list) {
    NodePtr head = (NodePtr)list;
    return head->next == NULL;
}

// 清空链表
bool clear_list(ListPtr list) {
    NodePtr head = (NodePtr)list;
    NodePtr current = head->next;
    while (current != NULL) {
        NodePtr next = current->next;
        free(current);
        current = next;
    }
    head->next = NULL;
    return true;
}

// 求长度
int list_length(ListPtr list) {
    NodePtr head = (NodePtr)list;
    int length = 0;
    NodePtr current = head->next;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

// 获取第 index 个元素
elemtype get_elem(ListPtr list, int index) {
    NodePtr head = (NodePtr)list;
    if (index < 0) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    NodePtr current = head->next;
    for (int i = 0; i < index; i++) {
        if (current == NULL) exit(EXIT_FAILURE);
        current = current->next;
    }
    if (current == NULL) exit(EXIT_FAILURE);
    return current->data;
}

// 定位元素
int locate_elem(ListPtr list, elemtype e) {
    NodePtr head = (NodePtr)list;
    int index = 0;
    NodePtr current = head->next;
    while (current != NULL) {
        if (current->data == e) return index;
        index++;
        current = current->next;
    }
    return -1;
}

// 插入元素
bool insert_elem(ListPtr list, int index, elemtype e) {
    NodePtr head = (NodePtr)list;
    if (index < 0) return false;
    NodePtr current = head;
    for (int i = 0; i < index; i++) {
        if (current == NULL) return false;
        current = current->next;
    }

    NodePtr new_node = (NodePtr)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->data = e;
    new_node->next = current->next;
    current->next = new_node;
    return true;
}

// 删除元素
bool delete_elem(ListPtr list, int index, elemtype* deleted_e) {
    NodePtr head = (NodePtr)list;
    if (index < 0) return false;
    NodePtr current = head;
    for (int i = 0; i < index; i++) {
        if (current->next == NULL) return false;
        current = current->next;
    }
    NodePtr to_delete = current->next;
    if (to_delete == NULL) return false;
    *deleted_e = to_delete->data;
    current->next = to_delete->next;
    free(to_delete);
    return true;
}

// 遍历
void traverse_list(ListPtr list, void (*visit)(elemtype e)) {
    NodePtr head = (NodePtr)list;
    NodePtr current = head->next;
    while (current != NULL) {
        visit(current->data);
        current = current->next;
    }
}
  