#include "init.h"
#include "tasks.h"
#include "imu.h"
#include "alt.h"
#include "lib/MTK3339/MTK.h"
#include "lib/w25qxx/w25qxx.h"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
    #include "lib/TinyScheduler/ts.h"
    #include "lib/common/common.h"
    #include "gps.h"
    #include "log.h"
}


int init() {
    // init LED for idle tasks
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    // make a little buzz buzz before we initialize everything
    HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
    HAL_Delay(500);

    // initialize all modules
    // try to initialize all of them 5 times and if they fail print an error when in debug mode

    unsigned char good_init = 0;

    // init system module
    for(int i = 0; i < 5; i++) {
        if(sys_init() >= 0) {
            good_init = 1;
            break;
        }
        HAL_Delay(500);
    }

    if(good_init) {
        good_init = 0;
    } else {
        #ifdef DEBUG
        extern UART_HandleTypeDef huart1;
        HAL_UART_Transmit(&huart1, (uint8_t*)"sys failed to init\r\n", 16, 100);
        #endif
    }

    // GPS
    for(int i = 0; i < 5; i++) {
        if(RET_OK == gps_init()) {
            ts_add(&gps_task);
            good_init = 1;
            break;
        }
        HAL_Delay(500);
    }

    if(good_init) {
        good_init = 0;
    } else {
        #ifdef DEBUG
        printf("GPS failed to init\r\n");
        #endif
    }

    // Altimeter
    for(int i = 0; i < 5; i++) {
        if(RET_OK == alt_init()) {
            ts_add(&alt_task);
            good_init = 1;
            break;
        }
        HAL_Delay(500);
    }

    if(good_init) {
        good_init = 0;
    } else {
        #ifdef DEBUG
        printf("altimeter failed to init\r\n");
        #endif
    }

    // IMU
    for(int i = 0; i < 5; i++) {
        if(RET_OK == imu_init()) {
            ts_add(&imu_task);
            good_init = 1;
            break;
        }
        HAL_Delay(500);
    }

    if(good_init) {
        good_init = 0;
    } else {
        #ifdef DEBUG
        printf("IMU failed to init\r\n");
        #endif
    }

    // logger
    for(int i = 0; i < 5; i++) {
        if(RET_OK == log_init()) {
            ts_add(&log_task);
            good_init = 1;
            break;
        }
        HAL_Delay(500);
    }

    if(good_init) {
        good_init = 0;
    } else {
        #ifdef DEBUG
        printf("logger failed to init\r\n");
        #endif
    }

    // turn off the buzzer, initialization done
    HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);

    // idle task
    tiny_task_t idle;
    idle.start_time = ts_systime();
    idle.default_priority = IDLE_PRIORITY;
    idle.task = &LED_loop;
    ts_add(&idle);

    // i2c hw checking task
    // tiny_task_t hw_chk;
    // hw_chk.start_time = ts_systime();
    // hw_chk.default_priority = LOW_PRIORITY;
    // hw_chk.task = &check_hw;
    // ts_add(&hw_chk);


    // start the scheduler
    ts_schedule(NULL, 0);

    // loop forever
    while(1) {};

    return -1; // should never return
}
