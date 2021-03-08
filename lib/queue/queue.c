#include "stdlib.h" // malloc
#include "lib/queue/queue.h"

// TODO add pre-allocation for nodes

typedef struct {
    void* data;
    void* next;
    void* prev;
} q_node_t;

#ifdef DEBUG
void q_print_queue(queue_t* q) {
    printf("head: %p\n", q->head);
    printf("tail: %p\n\n", q->tail);

    int i = 0;
    q_node_t* node = q->head;
    while(node != NULL) {
        printf("node %i --> ptr: %p data: %i next: %p prev: %p", i, node, *(int*)node->data, node->next, node->prev);
        printf("\n");
        node = node->next;
        i++;
    }
    printf("\n\n");
}
#endif

queue_t* q_mkqueue(int (*sort)(void* fst, void* snd)) {
    queue_t* q = (queue_t*)malloc(sizeof(queue_t));
    if(!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->sort = sort;
    return q;
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

    // queue is empty
    if(q->head == NULL) {
        node->next = NULL;
        node->prev = NULL;
        q->tail = node;
        q->head = node;
        return 1;
    }

    // find 'n' such that 'node' should go directly before 'n'
    q_node_t* n = q->head;
    if(q->sort) {
        while(n != NULL) {
            if(q->sort(n->data, data) > 0) { // new node should go after 'n'
                n = n->next;
            } else {
                break;
            }
        }
    }

    // new node goes on last
    if(n == NULL) {
        node->prev = q->tail;
        node->next = NULL;
        ((q_node_t*)q->tail)->next = node;
        q->tail = node;
    } else {
        if(n == q->head) {
            q->head = node;
        }
        node->next = n;
        node->prev = n->prev;
        n->prev = node;
    }

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
