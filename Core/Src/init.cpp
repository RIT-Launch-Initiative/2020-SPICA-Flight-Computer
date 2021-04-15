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

    // init system module
    if(sys_init() < 0) {
        extern UART_HandleTypeDef huart1;
        HAL_UART_Transmit(&huart1, (uint8_t*)"sys init error\r\n", 16, 100);
    }

    // GPS
    gps_init();
    ts_add(&gps_task); // from gps.h/c

    // Altimeter
    alt_init();
    ts_add(&alt_task);

    // IMU
    imu_init();
    ts_add(&imu_task);

    // logger
    log_init();
    ts_add(&log_task);

    // SPI Flash
    // TODO

    // make a little buzz buzz when we're ready
    HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
    HAL_Delay(500);
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
