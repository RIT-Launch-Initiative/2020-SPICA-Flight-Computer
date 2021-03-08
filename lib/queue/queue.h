#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_EMPTY(Q) (Q->head == NULL)

// 'sort' is a function that returns positive if snd should be dequeued AFTER fst
// if negative the opposite is true, if zero the order does not matter
typedef struct {
    void* head;
    void* tail;
    int (*sort)(void* fst, void* snd); // can be NULL (no sort function)
} queue_t;

// return negative on error
int q_enqueue(queue_t* q, void* data);

// return NULL on nothing or error
void* q_dequeue(queue_t* q);
void* q_peek(queue_t* q);

// NULL on error
queue_t* q_mkqueue(int (*sort)(void* fst, void* snd));

// TODO maybe add error check (only if 'free' fails)
void q_rmqueue(queue_t* q);

// for debug only
#ifdef DEBUG
void q_print_queue(queue_t* q);
#endif

#endif
