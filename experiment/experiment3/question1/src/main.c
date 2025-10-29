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
void print_queue(queue Q){
    queue_traverse(Q, visit);
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

void menu_queue(){
    queue Q = init_queue();
    printf("\n===== Queue Menu =====\n");
    while (1)
    {
        printf("1.Enqueue\n");
        printf("2.Dequeue\n");
        printf("3.view front element\n");
        printf("4.print all element\n");
        printf("5.exit\n");
        int choice;
        scanf(" %d", &choice);

        switch (choice)
        {
        case 1: {
            printf("Enter the element to enqueue: ");
            char temp;
            scanf(" %c", &temp);  // 前面有空格避免读入换行
            enqueue(Q, temp);
            printf("Enqueued: %c\n", temp);
        }
        break;

        case 2: {
            if(queue_empty(Q)){
                printf("Queue is empty!\n");
            } else {
                char val = dequeue(Q);
                printf("Dequeued element is: %c\n", val);
            }
        }
        break;

        case 3: {
            if(queue_empty(Q)){
                printf("Queue is empty!\n");
            } else {
                printf("Front element is: %c\n", get_head(Q));
            }
        }
        break;

        case 4: {
            print_queue(Q);
            printf("\n");
        }
        break;

        case 5:
            free_queue(Q);
            return;

        default:
            printf("Invalid choice\n");
        }

    }
}

void menu_stack() {
    stack S = init_stack();
    printf("\n===== Stack Menu =====\n");
    while (1) {
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. View top element\n");
        printf("4. Print all elements\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: {
            printf("Enter element to push: ");
            char temp;
            scanf(" %c", &temp); // 注意前置空格防止读取回车
            push(S, temp);
            printf("Pushed: %c\n", temp);
            break;
        }

        case 2: {
            if (stack_empty(S)) {
                printf("Stack is empty, cannot pop.\n");
            } else {
                elemtype val;
                pop(S, &val);
                printf("Popped: %c\n", val);
            }
            break;
        }

        case 3: {
            if (stack_empty(S)) {
                printf("Stack is empty.\n");
            } else {
                printf("Top element: %c\n", get_top(S));
            }
            break;
        }

        case 4: {
            printf("Stack elements (from bottom to top): ");
            print_stack(S);
            printf("\n");
            break;
        }

        case 5:
            free_stack(S);
            return;

        default:
            printf("Invalid choice, try again.\n");
        }
    }
}


int main() {
    // text_stack();
    // text_list();
    // text_queue();
    // menu_queue();
    printf("Choice data structure\n");
    printf("1.stack\n");
    printf("2.queue\n");

    int choice;
    scanf("%d",&choice);
    if(choice == 1) menu_stack();
    else if(choice == 2) menu_queue();
    else printf("invalid choice");
    
    return 0;
}