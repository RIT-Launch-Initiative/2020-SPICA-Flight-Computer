#include "init.h"
#include "tasks.h"
#include "lib/MTK3339/MTK.h"
#include "lib/w25qxx/w25qxx.h"
#include <stdio.h>

extern "C" {
    #include "lib/TinyScheduler/ts.h"
    #include "lib/common/common.h"
}
// #include "main.h"
// #include "adc.h"
// #include "i2c.h"
// #include "spi.h"
// #include "usart.h"
// #include "gpio.h"
#include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include "lib/MPL3115A2/MPL3115A2.h"

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

extern UART_HandleTypeDef huart1;
int init() {
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    // MTK3339::init(NMEA_OUTPUT_DEFAULT, NMEA_RATE_DEFAULT);
    // if(!W25qxx_Init()) {
    //     printf("failed to init spi flash\r\n");
    //     return -1;
    // }

    // test();

    if(sys_init() < 0) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"error\r\n", 8, 100);
    }

    tiny_task_t leds;
    leds.start_time = ts_systime();
    leds.priority = LOW_PRIORITY;
    leds.task = &LED_loop;

    // tiny_task_t hw_chk;
    // hw_chk.start_time = ts_systime();
    // hw_chk.priority = LOW_PRIORITY;
    // hw_chk.task = &check_hw;

    // tiny_task_t spi_flash;
    // spi_flash.start_time = ts_systime();
    // spi_flash.priority = HIGH_PRIORITY;
    // spi_flash.task = SPI_flash;

    tiny_task_t gps;
    gps.start_time = ts_systime();
    gps.priority = LOW_PRIORITY;
    gps.task = &GPS_test;

    ts_add(&leds);
    // ts_add(&hw_chk);
    ts_add(&gps);
    // ts_add(&spi_flash);
    ts_schedule(NULL, 0);

    return -1; // should never return
}

// GPIO_TypeDef* LEDPorts[4] = {LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port};
// uint16_t LEDPins[4] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin};
// int i = 1;
// int direction = 1;
//
// void LED_loop() {
//     HAL_GPIO_TogglePin(LEDPorts[i], LEDPins[i]);
//     if(i == 3) {
//         direction *= -1;
//     } else if(i == 0) {
//         direction *= -1;
//     }
//     i += direction;
//     HAL_GPIO_TogglePin(LEDPorts[i], LEDPins[i]);
// }

// #ifdef DEBUG
//
// #define IMU_ADDR 0b11010111
// #define IMU_WHOAMI 0x0F
// #define IMU_MAG_ADDR 0b00111101
//
// #define MPL_ADDR 0xC0
// #define MPL_WHOAMI 0x0C
//
// #define HIGHG_ADDR 0x31
// #define HIGHG_WHOAMI 0x0F
//
// // check hardware
// void chkhw(void) {
//     uint8_t buff[4];
//     memset(buff, 0, 4);
//     //HAL_StatusTypeDef stat;
//
//     LED_loop();
//     HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, IMU_WHOAMI, 1, &buff[0], 1, 200);
//     printf("accel + gyro whoami(0x68): 0x%0x\r\n", buff[0]);
//     HAL_Delay(500);
//     HAL_I2C_Mem_Read(&hi2c1, IMU_MAG_ADDR, IMU_WHOAMI, 1, &buff[1], 1, 200);
//     printf("mag whoami(0x3d): 0x%0x\r\n", buff[1]);
//     HAL_Delay(500);
//     HAL_I2C_Mem_Read(&hi2c1, MPL_ADDR, MPL_WHOAMI, 1, &buff[2], 1, 200);
//     printf("mpl whoami(0xc4): 0x%0x\r\n", buff[2]);
//     HAL_Delay(500);
//     HAL_I2C_Mem_Read(&hi2c1, HIGHG_ADDR, HIGHG_WHOAMI, 1, &buff[3], 1, 200);
//     printf("high-g whoami(0x32): 0x%0x\r\n", buff[3]);
//     HAL_Delay(500);
//     printf("\r\n");
//     // HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
// }
// #endif

// int init(void) {
//
// #ifdef DEBUG
//     HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
//     while(1) {
//         // chkhw();
//         LED_loop();
//     }
// #endif
//
//     while (1) {
//       printf("Starting...\r\n");
//
//       MPL3115A2 mpl;
//       bool ret = mpl.begin(&hi2c1);
//       if(!ret) {
//           printf("Altimeter failed to begin.\r\n");
//           return 0;
//       }
//
//       //mpl.setSeaPressure(101325);
//       float alt = -1;
//       float temp = -1;
//
//       HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
//
//       while(1) {
//           LED_loop();
//
//           alt = mpl.getAltitude()*3.28084;
//           printf("Altitude: %f ft\r\n", alt);
//
//           temp = mpl.getTemperature();
//           printf("Temperature: %f degrees C\r\n", temp);
//
//           HAL_Delay(500);
//       }
//       return 0;
//     }
// }
