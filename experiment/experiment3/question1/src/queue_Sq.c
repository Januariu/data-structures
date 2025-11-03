#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_CAPACITY 4

// internal data tructure
typedef struct {
    elemtype* data;
    int front;
    int rear;
    int capacity;
} QueueInter;

#define Q_Inter(q) ((QueueInter*)(q))

queue init_queue() {
    /*
    time complexity:: O(1)
    space complexity:: O(1)
    */
    QueueInter* Q = (QueueInter*)malloc(sizeof(QueueInter));
    if (!Q) exit(EXIT_FAILURE);

    Q->data = (elemtype*)malloc(sizeof(elemtype) * DEFAULT_CAPACITY);
    if (!Q->data) {
        free(Q);
        exit(EXIT_FAILURE);
    }

    Q->front = 0;
    Q->rear = 0;
    Q->capacity = DEFAULT_CAPACITY;
    return (queue)Q;
}

void free_queue(queue Q) {
    /*
    time complexity:: O(1)
    space complexity:: O(1)
    */
    QueueInter* q = Q_Inter(Q);
    if (!q) return;
    free(q->data);
    free(q);
}

bool queue_empty(queue Q) {
    /*
    time complexity:: O(1)
    space complexity:: O(1)
    */
    QueueInter* q = Q_Inter(Q);
    return q->front == q->rear;
}

int queue_length(queue Q) {
    /*
    time complexity:: O(1)
    space complexity:: O(1)
    */
    QueueInter* q = Q_Inter(Q);
    return (q->rear - q->front + q->capacity) % q->capacity;
}

elemtype get_head(queue Q) {
    /*
    time complexity:: O(1)
    space complexity:: O(1)
    */
    QueueInter* q = Q_Inter(Q);
    if (queue_empty(Q)) exit(EXIT_FAILURE);
    return q->data[q->front];
}

static void expand_queue(QueueInter* q) {
    /*
    time complexity:: O(n)
    space complexity:: O(n)
    */
    int old_cap = q->capacity;
    int new_cap = old_cap * 2;
    elemtype* new_data = (elemtype*)malloc(sizeof(elemtype) * new_cap);

    //flatten old queue
    int len = queue_length((queue)q);
    for (int i = 0; i < len; ++i) {
        new_data[i] = q->data[(q->front + i) % old_cap];
    }

    free(q->data);
    q->data = new_data;
    q->front = 0;
    q->rear = len;
    q->capacity = new_cap;
}

void enqueue(queue Q, elemtype e) {
    QueueInter* q = Q_Inter(Q);
    if ((q->rear + 1) % q->capacity == q->front) {
        expand_queue(q);
    }
    q->data[q->rear] = e;
    q->rear = (q->rear + 1) % q->capacity;
}

elemtype dequeue(queue Q) {
    QueueInter* q = Q_Inter(Q);
    if (queue_empty(Q)) exit(EXIT_FAILURE);
    elemtype e = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    return e;
}

void queue_traverse(queue Q, void (*visit)(elemtype)) {
    QueueInter* q = Q_Inter(Q);
    for (int i = q->front; i != q->rear; i = (i + 1) % q->capacity) {
        visit(q->data[i]);
    }
}

