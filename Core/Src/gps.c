#include "gps.h"
#include "usart.h"
#include "lib/common/common.h"
#include "lib/ringbuff/ringbuff.h"
#include "lib/TinyScheduler/ts.h"
// #include "lib/MTK3339/MTK.h" // this is borked right now, TODO unbork it
#include <stdint.h>
#include <unistd.h>

#define GPS_UART huart2
#define GPS_STARTL '$'
#define GPS_ENDL '\n'
#define GPS_BUFFER_SIZE 256 // should be 2 x maximum packet length

uint8_t gps_char;
unsigned char lines = 0;
uint8_t rb_buff[GPS_BUFFER_SIZE];
ringbuff_t rb;
tiny_task_t gps_task;


void gps_update(tiny_task_t* task);

// initialize
void gps_init() {
    rbuff_init(&rb, rb_buff, GPS_BUFFER_SIZE);
    HAL_UART_Transmit_IT(&GPS_UART, &gps_char, 1);
    gps_task.start_time = ts_systime();
    gps_task.priority = SLEEP_PRIORITY;
    gps_task.task = &gps_update;
}

// TODO check for errors on return with memcpyout
void gps_update(tiny_task_t* task) {
    disable_irq;
    lines--; // event is being handled
    enable_irq;

    uint8_t buff[GPS_BUFFER_SIZE / 2];
    uint8_t c = '\0';
    while(c != GPS_STARTL) {
        rbuff_memcpyout(&c, &rb, 1);
    }

    size_t i = 0;
    while(c != GPS_ENDL && i < (GPS_BUFFER_SIZE / 2)) { // maybe remove sanity check for size
        buff[i] = c;
        rbuff_memcpyout(&c, &rb, 1);
        i++;
    }

    // TODO
    // parse out buff into usable data
    // for now blast it out the debug port
    _write(1, (char*)buff, i);

    if(!lines) { // check if we have more lines to process
        task->priority = SLEEP_PRIORITY; // we can sleep the task
    }
}

// TODO check for errors on return with memcpyin
// we can assume interrupts are disabled (TODO double check this condition is true)
void gps_RxCallback() {
    rbuff_memcpyin(&rb, &gps_char, 1);
    if(gps_char == GPS_ENDL) {
        lines++; // we have another line to handle!
        gps_task.priority = LOW_PRIORITY; // TODO maybe make this dynamic
    }
}
