/*
*   Control code for the altimeter(s)
*/
#ifndef ALT_H
#define ALT_H

extern "C" {
    #include "lib/TinyScheduler/ts.h"
}

#define ALT_SAMPLE_PERIOD 100 // sample at 10 Hz
#define METERS_TO_FEET 3.28084

// call this before using the altimeter
void alt_init();

// start this task on the scheduler
extern tiny_task_t alt_task;

// filtered altimeter data gets placed here
// TODO put this somewhere else
extern float alt;

#endif
