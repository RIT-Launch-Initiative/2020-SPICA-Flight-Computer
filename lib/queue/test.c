#include "stdio.h"
#include "queue.h"


int main() {
    queue_t* q = q_mkqueue();

    int a = 1;
    int b = 2;
    int c = 3;

    q_enqueue(q, &a);
    q_enqueue(q, &b);
    q_enqueue(q, &c);

    int* test;

    test = (int*)q_dequeue(q);
    printf("%i\n", *test);

    test = (int*)q_dequeue(q);
    printf("%i\n", *test);

    test = (int*)q_dequeue(q);
    printf("%i\n", *test);

    q_rmqueue(q);
}
