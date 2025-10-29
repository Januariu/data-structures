#include "list.h"
#include "stack.h"
#include "queue.h"

void visit(elemtype e) {
        printf("%c ", e);
}
void print_stack(stack s) {
    traverse_stack(s, visit);
    printf("\n");
}
void print_list(ListPtr L){
    traverse_list(L, visit);
    printf("\n");
}

void text_stack(){
    stack s = init_stack();
    elemtype value;
    push(s, 10);
    push(s, 20);
    push(s, 30);
    // insert_elem(s)
    // traverse_stack(s, visit);
    printf("\n");
    value = get_top(s);
    printf("Element at top: %d\n", value);
    pop(s, &value);
    printf("Popped element: %d\n", value);

    free_stack(s);
}

void text_list(){
    ListPtr list = init_list();
    elemtype e;
    insert_elem(list, 0, 'c');
    insert_elem(list, 1, 'y');
    push_back(list, 'm');
    push_back(list, 'c');
    push_back(list, 'a');
    delete_elem(list, 0, &e);
    insert_elem(list, 0, 'o');
    printf("%c\n",e);
    print_list(list);
    free_stack(list);
}

void text_queue(){
    queue Q = init_queue();
    enqueue(Q, 'a');
    enqueue(Q, 'b');
    printf("%c\n", dequeue(Q));
    printf("%c\n",get_head(Q));
    enqueue(Q, 'c');
    printf("%c\n", dequeue(Q));
    free_queue(Q);
}

int main() {
    // text_stack();
    // text_list();
    text_queue();
    return 0;
}