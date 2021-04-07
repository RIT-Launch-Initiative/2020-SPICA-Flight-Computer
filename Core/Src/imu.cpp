/*
*   Control code for the IMU
*/
#include <stdio.h> // printf
#include "imu.h"
#include "lib/LSM9DS1/LSM9DS1.h"

tiny_task_t imu_task;

LSM9DS1 imu;

// filtered values
float ax, ay, az;
float roll, pitch, yaw;

// raw values
float raw_ax;
float raw_ay;
float raw_az;
float raw_gx;
float raw_gy;
float raw_gz;

// function called in the imu_task
void imu_update(tiny_task_t* task) {
    if(!imu.accelAvailable() || !imu.gyroAvailable()) {
        // try again (soonish) :(
        task->start_time = ts_systime() + (IMU_SAMPLE_PERIOD / 2.0);
    }

    imu.readGyro();
    imu.readAccel();

    raw_ax = imu.calcAccel(imu.ax);
    raw_ay = imu.calcAccel(imu.ay);
    raw_ay = imu.calcAccel(imu.ay);

    raw_gx = imu.calcGyro(imu.gx);
    raw_gy = imu.calcGyro(imu.gy);
    raw_gz = imu.calcGyro(imu.gz);

    // TODO filter/integrate the raw values and do something with them
    // send some data to the ground and log the raw values
    // for now just print them out of the debug UART
    #ifdef DEBUG
    printf("ax: %f, ay: %f, az: %f\r\n", raw_ax, raw_ay, raw_az);
    printf("gx: %f, gy: %f, gz: %f\r\n\r\n", raw_gx, raw_gy, raw_gz);
    #endif

    task->start_time = ts_systime() + IMU_SAMPLE_PERIOD;
}

void imu_init() {
    // set task first so we don't deref a null ptr in the scheduler
    imu_task.start_time = ts_systime() + IMU_SAMPLE_PERIOD;
    imu_task.priority = LOW_PRIORITY;
    imu_task.task = &imu_update;

    if(0 == imu.begin()) {
        printf("IMU init fail\r\n");
        return;
        // TODO return failure code
    }
    // TODO calibrate?? (probably a good idea)

    // TODO change these settings
    // imu.setGyroScale(G_SCALE_245DPS);
    // imu.setAccelScale(A_SCALE_2G);
    // imu.setGyroODR(G_ODR_149);
    // imu.setAccelODR(XL_ODR_10);
}
