#include "stdio.h"
#include "queue.h"

int sort(void* fst, void* snd) {
    int one = *((int*)fst);
    int two = *((int*)snd);

    return two - one;
}

int main() {
    queue_t* q = q_mkqueue(&sort);

    int a = 2;
    int b = 3;
    int c = 1;

    q_enqueue(q, &a);
    q_print_queue(q);
    q_enqueue(q, &b);
    q_print_queue(q);
    q_enqueue(q, &c);
    q_print_queue(q);

    int* test;

    test = (int*)q_dequeue(q);
    printf("%i\n", *test);

    test = (int*)q_dequeue(q);
    printf("%i\n", *test);

    test = (int*)q_dequeue(q);
    printf("%i\n", *test);

    printf("%i\n", QUEUE_EMPTY(q));

    q_rmqueue(q);
}
