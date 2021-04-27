#include "ts.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// needed by scheduler
// use seconds so it's nice and slow
long int ts_systime() {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    return timeinfo->tm_sec;
}

void task1(tiny_task_t* t) {
    printf("task 1\n");
    for(int i = 0; i < 1000; i++) {
        // burn some time
    }
    // schedule us to run again in 1 seconds
    t->start_time = ts_systime() + 1;
}

void task2(tiny_task_t* t) {
    printf("task 2\n");
    for(int i = 0; i < 1000; i++) {
        // burn some time
    }
    // schedule us to run again in 2 seconds
    t->start_time = ts_systime() + 2;
}

int main(int argc, char** argv) {
    tiny_task_t t1;
    t1.start_time = ts_systime();
    t1.default_priority = LOW_PRIORITY;
    t1.task = &task1;

    tiny_task_t t2;
    t2.start_time = ts_systime();
    t2.default_priority = LOW_PRIORITY;
    t2.task = &task2;

    ts_add(&t1);
    ts_add(&t2);

    ts_schedule(NULL, 0);
}
