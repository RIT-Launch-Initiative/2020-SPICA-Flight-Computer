/*
*   Code for telemetry logging
*/
#include "log.h"
#include <string.h> // memset
#include <stdio.h>
#include "lib/common/common.h" // _write
#include "lib/fs/fs.h"

// XBEE UART file descriptor (as in lib/sys.c)
#define XBEE_FD 1

tiny_task_t log_task;
log_packet_t log_packet;

typedef struct {
    uint8_t preamble[4]; // ">>>>"
    uint32_t payload_size;
} header_t;

header_t header;

// function called in the log task
void log_update(tiny_task_t* task) {
    log_packet.uptime = ts_systime();
    _write(XBEE_FD, (char*)&header, sizeof(header_t));
    _write(XBEE_FD, (char*)&log_packet, sizeof(log_packet_t));

    #ifdef DEBUG
    printf("logged packet\r\n");
    #endif

    // write to nonvolatile storage on flash chip
    fs_write((uint8_t*)&log_packet, sizeof(log_packet_t));

    task->start_time += LOG_PERIOD;
}

// init function
RetType log_init() {
    // setup task
    log_task.start_time = ts_systime() + LOG_PERIOD;
    log_task.default_priority = MEDIUM_PRIORITY; // higher priority than collecting data
    log_task.task = &log_update;

    // setup header
    header.preamble[0] = '>';
    header.preamble[1] = '>';
    header.preamble[2] = '>';
    header.preamble[3] = '>';
    header.payload_size = sizeof(log_packet_t);

    // zero the packet
    memset(&log_packet, 0, sizeof(log_packet_t));

    if(FS_OK != fs_open()) {
        printf("FLASH failed to initialize\r\n");
        return RET_OK; // still want to run the XBee logging
    }

    return RET_OK;
}
