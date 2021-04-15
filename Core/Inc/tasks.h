/*
*   Declares a number of TinyScheduler tasks and defines ts_systime for the scheduler
*/
#ifndef TASKS_H
#define TASKS_H

extern "C" {
    #include "lib/TinyScheduler/ts.h"
}

void LED_loop(tiny_task_t*);
void check_hw(tiny_task_t*);
void SPI_flash(tiny_task_t*);

#endif
