#include "queue.h"

typedef struct que{
    ListPtr list;
    int length;
}*queuePtr;

queue init_queue(){
    queuePtr Q = (queuePtr)malloc(sizeof(struct que));
    if(Q == NULL){
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    Q->list = init_list();
    Q->length = 0;
    return (queue)Q;
}

void free_queue(queue Q){
    free_list(((queuePtr)Q)->list);
    free((queuePtr)Q);
}

void clear_queue(queue Q){
    queuePtr temp = (queuePtr)Q;
    free_list(temp->list);
    temp->list = init_list();
    temp->length = 0;
}

bool queue_empty(queue Q){
    return ((queuePtr)Q)->length == 0;
}

int queue_length(queue Q){
    return ((queuePtr)Q)->length;
}

elemtype get_head(queue Q){
    return get_elem(((queuePtr)Q)->list, 0);
}

void enqueue(queue Q, elemtype e){
    ((queuePtr)Q)->length++;
    push_back(((queuePtr)Q)->list, e);
}

elemtype dequeue(queue Q){
    elemtype temp;
    ((queuePtr)Q)->length--;
    delete_elem(((queuePtr)Q)->list, 0, &temp);
    return temp;
}

void queue_traverse(queue Q, void (*visit)(elemtype)){
    traverse_list(((queuePtr)Q)->list, visit);
}