#include "init.h"
#include "tasks.h"
#include "lib/MTK3339/MTK.h"
#include "lib/w25qxx/w25qxx.h"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
    #include "lib/TinyScheduler/ts.h"
    #include "lib/common/common.h"
    #include "gps.h"
}

void test() {
    printf("start\r\n");
    uint8_t data = 0x9f;
    uint8_t ret[3];
    HAL_StatusTypeDef stat;
    while(1) {
        stat = HAL_SPI_TransmitReceive(&hspi1,&data,ret,3,100);
        if(stat != HAL_OK) {
            printf("err\r\n");
        } else {
            printf("%02x %02x %02x\r\n", ret[0], ret[1], ret[2]);
        }
        HAL_Delay(1000);
    }
}

int init() {
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    if(sys_init() < 0) {
        extern UART_HandleTypeDef huart1;
        HAL_UART_Transmit(&huart1, (uint8_t*)"error\r\n", 8, 100);
    }

    gps_init();
    ts_add(&gps_task); // from gps.h/c

    tiny_task_t idle;
    idle.start_time = ts_systime();
    idle.priority = IDLE_PRIORITY;
    idle.task = &LED_loop;

    // tiny_task_t hw_chk;
    // hw_chk.start_time = ts_systime();
    // hw_chk.priority = LOW_PRIORITY;
    // hw_chk.task = &check_hw;

    // tiny_task_t spi_flash;
    // spi_flash.start_time = ts_systime();
    // spi_flash.priority = HIGH_PRIORITY;
    // spi_flash.task = SPI_flash;

    ts_add(&idle);
    // ts_add(&hw_chk);
    // ts_add(&spi_flash);

    // start the scheduler
    ts_schedule(NULL, 0);

    return -1; // should never return
}
