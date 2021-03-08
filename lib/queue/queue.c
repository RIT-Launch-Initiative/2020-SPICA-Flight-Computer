#include "stdlib.h" // malloc
#include "lib/queue/queue.h"

typedef struct {
    void* data;
    void* next;
    void* prev;
} q_node_t;

queue_t* q_mkqueue() {
    queue_t* head = (queue_t*)malloc(sizeof(queue_t));
    head->head = NULL;
    head->tail = NULL;
    return head;
}

void q_rmqueue(queue_t* q) {
    q_node_t* node = (q_node_t*)q->head;
    q_node_t* next;
    while(node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
    free(q);
}

int q_enqueue(queue_t* q, void* data) {
    q_node_t* node = (q_node_t*)malloc(sizeof(q_node_t));
    if(node == NULL) {
        return -1; // malloc failed, no memory left :(
    }

    node->data = data;

    node->next = q->head;
    node->prev = NULL;

    if(q->head == NULL) {
        q->tail = node;
    } else {
        ((q_node_t*)q->head)->prev = node;
    }

    q->head = node;

    return 1;
}

void* q_dequeue(queue_t* q) {
    if(q->tail == NULL) {
        return NULL;
    }

    void* ret = ((q_node_t*)q->tail)->data;

    // only one element
    if(q->head == q->tail) {
        free(q->head);
        q->head = NULL;
        q->tail = NULL;
        return ret;
    }

    q_node_t* old_tail = q->tail;
    q->tail = old_tail->prev;
    ((q_node_t*)q->tail)->next = NULL;

    free(old_tail);

    return ret;
}

void* q_peek(queue_t* q) {
    if(q->tail == NULL) {
        return NULL;
    }

    return ((q_node_t*)q->tail)->data;
}
