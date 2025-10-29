#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef char elemtype;
typedef void* ListPtr;
typedef void* ElemPtr;

ListPtr init_list();
void free_list(ListPtr head);
bool list_empty(ListPtr head);
bool clear_list(ListPtr head);
int list_length(ListPtr head);
elemtype get_elem(ListPtr head, int index);
int locate_elem(ListPtr head, elemtype e);
ElemPtr prior_elem(ListPtr head, ElemPtr cur_e);
ElemPtr next_elem(ListPtr head, ElemPtr cur_e);
bool insert_elem(ListPtr head, int index, elemtype e);
bool delete_elem(ListPtr head, int index, elemtype* deleted_e);
void push_back(ListPtr head, elemtype e);
void traverse_list(ListPtr head, void (*visit)(elemtype e));

#endif // LIST_H