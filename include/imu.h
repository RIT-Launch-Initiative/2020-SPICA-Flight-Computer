/*
*   Control code for the IMU
*/
#ifndef IMU_H
#define IMU_H

extern "C" {
    #include "lib/TinyScheduler/ts.h"
    #include "lib/common/common.h"
}

// frequency should be slower or equal to the slowest IMU reading
// updates the filter at this frequency
#define IMU_SAMPLE_FREQ 10 // Hz
#define IMU_SAMPLE_PERIOD (1000 / IMU_SAMPLE_FREQ) // in ms

// call this before using the IMU
// set calibrate to 0 to skip calibration, 1 to calibrate, currently defaults to OFF
// returns RET_OK on success, RET_ERROR on failure
RetType imu_init(int calibrate = 0);

// start this task on the scheduler
extern tiny_task_t imu_task;

// filtered IMU data gets placed here
// TODO put this somewhere else?
extern float ax, ay, az; // accel
extern float roll, pitch, yaw; // gyro
extern float mx, my, mz;

#endif
