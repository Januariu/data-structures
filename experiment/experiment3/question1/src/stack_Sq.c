#include "stack.h"

typedef struct stack{
    ListPtr list;
    int top; 
} *SqStack;

stack init_stack(){
    SqStack s = (SqStack)malloc(sizeof(struct stack));
    if (!s) return NULL;
    s->list = init_list();
    s->top = 0;
    return (stack)s;
}

bool stack_empty(stack s){
    SqStack stack = (SqStack)s;
    return list_empty(stack->list);
}

int stack_length(stack s){
    SqStack stack = (SqStack)s;
    return list_length(stack->list);
}

void clear_stack(stack s){
    SqStack stack = (SqStack)s;
    clear_list(stack->list);
    stack->top = 0;
}

void free_stack(stack s){
    SqStack stack = (SqStack)s;
    clear_list(stack->list);
    free(stack);
}

bool push(stack s, elemtype e){
    SqStack stack = (SqStack)s;
    // if (stack->top >= MAX_SIZE) return false;
    stack->top++;
    // insert_elem(stack->list, stack->top++, e);
    push_back(stack->list,e);
    return true;
}

bool pop(stack s, elemtype* e){
    SqStack stack = (SqStack)s;
    if (stack->top == 0) {
        printf("Stack is empty.\n");
        exit(EXIT_FAILURE);
    }
    *e = get_top(stack);
    delete_elem(stack->list, --stack->top, e);
    // stack->top--;
    return true;
}

elemtype get_top(stack s){
    SqStack stack = (SqStack)s;
    if (stack->top == 0) exit(EXIT_FAILURE);
    return get_elem(stack->list, stack->top-1);
}

void traverse_stack(stack s, void (*visit)(elemtype e)){
    SqStack stack = (SqStack)s;
    traverse_list(stack->list, visit);
}
