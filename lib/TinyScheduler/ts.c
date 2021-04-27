/*
/   Tiny Scheduler
/
/   Will Merges 2021
*/
#include "ts.h"
#include <string.h>

// task pointers
tiny_task_t* tasks[TS_MAX_TASKS];

// mark a ttid, used = 1, unused = 0
unsigned char ttid_map[TS_MAX_TASKS];


int ts_add(tiny_task_t* task) {
    // find a free ttid
    for(int i = 0; i < TS_MAX_TASKS; i++) {
        if(ttid_map[i] == 0) {
            task->priority = task->default_priority;
            task->ttid = i;
            ttid_map[i] = 1;
            tasks[i] = task;
            return i;
        }
    }
    return TS_ERR_TTID; // no ttid's available
}

void ts_rem(int ttid) {
    // bounds check
    if(ttid < 0 || ttid >= TS_MAX_TASKS) {
        return;
    }
    // clear and mark unused
    memset(tasks + ttid, 0, sizeof(tiny_task_t*));
    ttid_map[ttid] = 0;
}

// not all that great of time complexity,  ¯\_(ツ)_/¯
void ts_schedule(int* stop_var, int stop_cond) {
    int dummy = 1;
    if(stop_var == NULL) {
        stop_var = &dummy;
        stop_cond = 0; // just loop forever
    }

    long int curr_time = ts_systime();
    long int best_ttid = -1;
    while(*stop_var != stop_cond) {
        while(curr_time == ts_systime()) {}; // spinning is okay here, we have nothing better to do
        curr_time = ts_systime();
        for(size_t i = 0; i < TS_MAX_TASKS; i++) {
            if(ttid_map[i] == 1) {
                if(tasks[i]->priority == SLEEP_PRIORITY) {
                    continue; // skip sleeping tasks
                }
                if(tasks[i]->start_time < curr_time) {
                    if(best_ttid == -1) {
                        best_ttid = i;
                    } else if(tasks[i]->priority > tasks[best_ttid]->priority) {
                        tasks[best_ttid]->priority++;
                        best_ttid = i;
                    }
                }
            }
        }
        if(best_ttid != -1) {
            // change the priority BEFORE calling the task so it can change it's own priority
            tasks[best_ttid]->priority = tasks[best_ttid]->default_priority;
            tasks[best_ttid]->task(tasks[best_ttid]);
            best_ttid = -1;
        }
    }
}
