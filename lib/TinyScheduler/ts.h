/*
/   Tiny Scheduler
/   A low memory usage non-preemptive scheduler
/
/   Not very good but works for a small number of tasks
/
/   Will Merges 2021
*/
#ifndef TINY_SCHED_H
#define TINY_SCHED_H

// maximum number of tasks for the scheduler to track at a time
#define TS_MAX_TASKS 20

// the ttid returned when an error occurs (no more room to store task)
#define TS_ERR_TTID -1

// this needs to be defined by the system!
// the extern is unnecessary, just a reminder to define it!
extern long int ts_systime();

// priority level
// can increase the values of these since priorities are incremented
// each time a task is skipped
typedef enum {
    IDLE_PRIORITY, SLEEP_PRIORITY, LOW_PRIORITY, MEDIUM_PRIORITY, HIGH_PRIORITY
} priority_t;

// task structure
typedef struct tiny_task tiny_task_t;
struct tiny_task {
    int ttid; // tiny task id, set by the scheduler
    long int start_time; // requested start time, set by user, units are what ts_systime uses
    priority_t default_priority; // priority level, set by user
    priority_t priority;
    void (*task) (tiny_task_t*); // task function, defined by programmer
};

// add a task to the scheduler
// returns ttid
int ts_add(tiny_task_t* task);

// remove a task from the scheduler
void ts_rem(int ttid);

// runs the scheduler
// will not exit unless *stop_var == stop_cond
void ts_schedule(int* stop_var, int stop_cond);

#endif
