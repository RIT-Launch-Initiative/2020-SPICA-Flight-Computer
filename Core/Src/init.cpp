#include "init.h"
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/MPL3115A2/MPL3115A2.h"


GPIO_TypeDef* LEDPorts[4] = {LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port};
uint16_t LEDPins[4] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin};
int i = 1;
int direction = 1;

void LED_loop() {
    HAL_GPIO_TogglePin(LEDPorts[i], LEDPins[i]);
    if(i == 3) {
        direction *= -1;
    } else if(i == 0) {
        direction *= -1;
    }
    i += direction;
    HAL_GPIO_TogglePin(LEDPorts[i], LEDPins[i]);
}

#define IMU_ADDR 0b11010111
#define IMU_WHOAMI 0x0F
#define IMU_MAG_ADDR 0b00111101

#define MPL_ADDR 0xC0
#define MPL_WHOAMI 0x0C

#define HIGHG_ADDR 0x31
#define HIGHG_WHOAMI 0x0F

int init(void) {
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    uint8_t buff[4];
    memset(buff, 0, 4);
    HAL_StatusTypeDef stat;
    while(1) {
        LED_loop();
        HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, IMU_WHOAMI, 1, &buff[0], 1, 200);
        printf("accel + gyro whoami(0x68): 0x%0x\r\n", buff[0]);
        HAL_Delay(500);
        HAL_I2C_Mem_Read(&hi2c1, IMU_MAG_ADDR, IMU_WHOAMI, 1, &buff[1], 1, 200);
        printf("mag whoami(0x3d): 0x%0x\r\n", buff[1]);
        HAL_Delay(500);
        HAL_I2C_Mem_Read(&hi2c1, MPL_ADDR, MPL_WHOAMI, 1, &buff[2], 1, 200);
        printf("mpl whoami(0xc4): 0x%0x\r\n", buff[2]);
        HAL_Delay(500);
        stat = HAL_I2C_Mem_Read(&hi2c1, HIGHG_ADDR, HIGHG_WHOAMI, 1, &buff[3], 1, 200);
        //printf("status: %d\r\n", stat);
        printf("high-g whoami(0x32): 0x%0x\r\n", buff[3]);
        HAL_Delay(500);
        printf("\r\n");
        //HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
    }
    return 0;
}

// int init(void) {
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
//}
