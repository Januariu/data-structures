#ifndef STACK_H
#define STACK_H
#include "list.h"

typedef void* stack;

stack init_stack();
bool stack_empty(stack s);
int stack_length(stack s);
void clear_stack(stack s);
void free_stack(stack s);
bool push(stack s, elemtype e);
bool pop(stack s, elemtype* e);
elemtype get_top(stack s);
void traverse_stack(stack s, void (*visit)(elemtype e));

#endif // STACK_H