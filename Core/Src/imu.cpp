/*
*   Control code for the IMU
*/
#include <stdio.h> // printf
#include "imu.h"
#include "lib/LSM9DS1/LSM9DS1.h"

extern "C" {
    #include "log.h"
}

tiny_task_t imu_task;

LSM9DS1 imu;

// filtered values
float ax, ay, az;
float roll, pitch, yaw;
float mx, my, mz;

// raw values
float raw_ax;
float raw_ay;
float raw_az;
float raw_gx;
float raw_gy;
float raw_gz;
float raw_mx;
float raw_my;
float raw_mz;

// function called in the imu_task
void imu_update(tiny_task_t* task) {
    // TODO separate these checks?
    if(!imu.accelAvailable() || !imu.gyroAvailable() || !imu.magAvailable()) {
        // try again (soonish) :(
        task->start_time += (IMU_SAMPLE_PERIOD / 2);
    }

    imu.readGyro();
    imu.readAccel();
    imu.readMag();

    raw_ax = imu.calcAccel(imu.ax);
    raw_ay = imu.calcAccel(imu.ay);
    raw_az = imu.calcAccel(imu.az);

    raw_gx = imu.calcGyro(imu.gx);
    raw_gy = imu.calcGyro(imu.gy);
    raw_gz = imu.calcGyro(imu.gz);

    raw_mx = imu.calcMag(imu.mx);
    raw_my = imu.calcMag(imu.my);
    raw_mz = imu.calcMag(imu.mz);

    // copy the values out to the logger
    log_packet.ax = raw_ax;
    log_packet.ay = raw_ay;
    log_packet.az = raw_az;
    log_packet.gx = raw_gx;
    log_packet.gy = raw_gy;
    log_packet.gz = raw_gz;
    log_packet.mx = raw_mx;
    log_packet.my = raw_my;
    log_packet.mz = raw_mz;


    // TODO filter/integrate the raw values and do something with them
    // for now just print them out of the debug UART
    #ifdef DEBUG
    printf("ax: %f, ay: %f, az: %f\r\n", raw_ax, raw_ay, raw_az);
    printf("gx: %f, gy: %f, gz: %f\r\n\r\n", raw_gx, raw_gy, raw_gz);
    printf("mx: %f, my: %f, mz: %f\r\n\r\n", raw_gx, raw_gy, raw_gz);
    // printf("%f\r\n", raw_ax);
    #endif

    task->start_time += IMU_SAMPLE_PERIOD;
}

RetType imu_init(int calibrate) {
    // set task first so we don't deref a null ptr in the scheduler
    imu_task.start_time = ts_systime() + IMU_SAMPLE_PERIOD;
    imu_task.default_priority = LOW_PRIORITY;
    imu_task.task = &imu_update;

    if(0 == imu.begin()) {
        #ifdef DEBUG
        printf("IMU init fail\r\n");
        #endif

        return RET_ERROR;
    }

    // calibrate
    if(calibrate) {
        HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
        HAL_Delay(1000); // give ourselves a second to stand back
        imu.calibrate();
        HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
    }

    // TODO fiddle with these settings
    imu.setGyroScale(G_SCALE_245DPS);
    imu.setAccelScale(A_SCALE_4G);
    imu.setGyroODR(G_ODR_149);
    imu.setAccelODR(XL_ODR_10);
    imu.setMagScale(M_SCALE_4GS);
    imu.setMagODR(M_ODR_10);

    return RET_OK;
}
