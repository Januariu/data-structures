#include "list.h"
#include "stack.h"

// void visit(elemtype e) {
//         printf("%d ", e);
// }
// void print_stack(stack s) {
//     traverse_stack(s, visit);
//     printf("\n");
// }
void text_stack(){
    stack s = init_stack();
    elemtype value;
    push(s, 10);
    push(s, 20);
    push(s, 30);
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
    insert_elem(list, 0, 10);
    insert_elem(list, 1, 20);
    insert_elem(list, 2, 20);
    insert_elem(list, 3, 30);
    elemtype e;
    e = get_elem(list, locate_elem(list, 20));
    ListPtr prior = prior_elem(list, e);
    ListPtr next = next_elem(list, e);
    if (prior != NULL) {
        printf("Prior element: %d\n", *(elemtype*)prior);
    } else {
        printf("No prior element.\n");
    }
    if (next != NULL) {
        printf("Next element: %d\n", *(elemtype*)next);
    } else {
        printf("No next element.\n");
    }
}

int main() {
    // text_stack();
    text_list();
    return 0;
}