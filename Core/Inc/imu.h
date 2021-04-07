/*
*   Control code for the IMU
*/
#ifndef IMU_H
#define IMU_H

extern "C" {
    #include "lib/TinyScheduler/ts.h"
}

// frequency should be slower than the slowest IMU reading
// updates the filter at this frequency
#define IMU_SAMPLE_FREQ 15
#define IMU_SAMPLE_PERIOD (1000 / IMU_SAMPLE_FREQ) // in ms

// call this before using the IMU
void imu_init();

// start this task on the scheduler
extern tiny_task_t imu_task;

// filtered IMU data gets placed here
// TODO put this somewhere else
extern float ax, ay, az;
extern float roll, pitch, yaw;


#endif
