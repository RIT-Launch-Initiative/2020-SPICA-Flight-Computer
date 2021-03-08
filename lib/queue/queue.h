#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
    void* head;
    void* tail;
} queue_t;

// return negative on error
int q_enqueue(queue_t* q, void* data);

// return NULL on nothing or error
void* q_dequeue(queue_t* q);
void* q_peek(queue_t* q);

// NULL on error
queue_t* q_mkqueue();

// TODO maybe add error check (only if 'free' fails)
void q_rmqueue(queue_t* q);

#endif
