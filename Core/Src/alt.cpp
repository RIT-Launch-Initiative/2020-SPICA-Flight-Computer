/*
*   Control code for the altimeter(s)
*/
#include <stdio.h>
#include "alt.h"
#include "lib/MPL3115A2/MPL3115A2.h"
extern "C" {
    #include "log.h"
    #include "lib/common/common.h"
}

tiny_task_t alt_task;
MPL3115A2 altimeter;

float raw_alt;

void alt_update(tiny_task_t* task) {
    if(altimeter.getAltitude(&raw_alt) != RET_OK) {
        // blocked or error, try again soonish
        printf("alt fail\r\n");
        task->start_time += (ALT_SAMPLE_PERIOD / 2);
    } else {
        // TODO send the altitude to a filter
        // for now just blast it out the debug UART
        #ifdef DEBUG
        printf("alt: %f ft\r\n", raw_alt * METERS_TO_FEET);
        #endif

        // copy the altitude out to the logger
        log_packet.alt1 = raw_alt * METERS_TO_FEET;

        task->start_time += ALT_SAMPLE_PERIOD;
    }
    return;
}

RetType alt_init() {
    alt_task.start_time = ts_systime() + ALT_SAMPLE_PERIOD;
    alt_task.default_priority = LOW_PRIORITY;
    alt_task.task = &alt_update;

    if(altimeter.begin() == 0) {
        #ifdef DEBUG
        printf("ALT init fail\r\n");
        #endif

        return RET_ERROR;
    }

    // TODO set sea level??

    return RET_OK;
}
