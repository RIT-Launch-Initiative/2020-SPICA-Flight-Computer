/*
*   Code for telemetry logging
*/
#ifndef LOG_H
#define LOG_H

#include "stdint.h"
#include "lib/TinyScheduler/ts.h"
#include "lib/common/common.h"

// amount of time between each log
// currently set to the speed the IMU updates (10 Hz)
// pull this back down later
#define LOG_PERIOD 100 // ms

// initialize the logger
RetType log_init();

// logger task
extern tiny_task_t log_task;

// we're assuming that floats are 4 bytes although the C standard does not guarantee this
// do not do any implicit packing, but should be try to make 4 byte aligned for faster reads/writes though
// 64 bytes
#pragma pack(1)
typedef struct {
    // uptime
    uint32_t uptime;
    // GPS
    float latitude;
    float longitude;
    float gps_alt;
    uint32_t gps_time;
    uint16_t gps_fix;
    uint16_t gps_sats;
    // altimeter 1
    float alt1;
    // IMU
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float mx;
    float my;
    float mz;
} log_packet_t;

// packet that gets sent every update
// other tasks should update this packet to get their data sent
extern log_packet_t log_packet;

#endif
