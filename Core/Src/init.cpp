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
#define IMU_MAG_ADDR 0b00111011

int init(void) {
    uint8_t buff[1];
    uint16_t send[1];
    send[0] = IMU_WHOAMI;
    while(1) {
        HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, *send, 1, buff, 1, 200);
        printf("imu whoami: 0x%0x\r\n", buff[0]);
        HAL_I2C_Mem_Read(&hi2c1, IMU_MAG_ADDR, *send, 1, buff, 1, 200);
        printf("mag whoami: 0x%0x\r\n", buff[0]);
        HAL_Delay(500);
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
