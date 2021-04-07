/*
*   Control code for the altimeter(s)
*/
#include <stdio.h>
#include "alt.h"
#include "lib/common/common.h"
#include "lib/MPL3115A2/MPL3115A2.h"

tiny_task_t alt_task;
MPL3115A2 altimeter;

float raw_alt;

void alt_update(tiny_task_t* task) {
    if(altimeter.getAltitude(&raw_alt) != RET_OK) {
        // blocked or error, try again soonish
        task->start_time = ts_systime() + (ALT_SAMPLE_PERIOD / 2);
    } else {
        // TODO either filter the altitude or put it somewhere
        // for now just blast it out the debug UART
        #ifdef DEBUG
        printf("alt: %f ft\r\n", raw_alt * METERS_TO_FEET);
        #endif

        task->start_time = ts_systime() + ALT_SAMPLE_PERIOD;
    }
    return;
}

void alt_init() {
    alt_task.start_time = ts_systime() + ALT_SAMPLE_PERIOD;
    alt_task.default_priority = LOW_PRIORITY;
    alt_task.task = &alt_update;

    if(altimeter.begin() == 0) {
        printf("ALT init fail\r\n");
        return;
        // TODO return failure
    }

    // TODO set sea level??
}
