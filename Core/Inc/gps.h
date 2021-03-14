#ifndef GPS_H
#define GPS_H

#include "lib/TinyScheduler/ts.h"

// call this before using the GPS
void gps_init();

// call this in HAL_UART_RxCpltCallback for whatever uart the GPS is connected to
void gps_RxCallback();

#endif
