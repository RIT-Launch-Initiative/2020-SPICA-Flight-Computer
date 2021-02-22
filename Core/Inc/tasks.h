
#ifndef TASKS_H
#define TASKS_H

extern "C" {
    #include "lib/TinyScheduler/ts.h"
}

void LED_loop(tiny_task_t*);
void check_hw(tiny_task_t*);
void GPS_test(tiny_task_t*);

#endif
