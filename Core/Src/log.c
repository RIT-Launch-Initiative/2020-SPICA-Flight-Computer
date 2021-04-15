/*
*   Code for telemetry logging
*/
#include "log.h"
#include <string.h> // memset
#include "lib/common/common.h" // _write

// XBEE UART file descriptor (as in lib/sys.c)
#define XBEE_FD 0

tiny_task_t log_task;
log_packet_t log_packet;


// function called in the log task
void log_update(tiny_task_t* task) {
    log_packet.uptime = ts_systime();

    _write(XBEE_FD, (char*)&log_packet, sizeof(log_packet_t));

    task->start_time += LOG_PERIOD;
}

// init function
void log_init() {
    // setup task
    log_task.start_time = ts_systime() + LOG_PERIOD;
    log_task.default_priority = MEDIUM_PRIORITY; // higher priority than collecting data
    log_task.task = &log_update;

    // zero the packet
    memset(&log_packet, 0, sizeof(log_packet_t));
}
