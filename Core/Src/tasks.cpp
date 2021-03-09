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
#include "lib/MTK3339/MTK.h"
#include "lib/w25qxx/w25qxx.h"

extern "C" {
    extern int _write(int file, char *ptr, int len);
    #include "lib/TinyScheduler/ts.h"
}

// using namespace MTK3339;


// needed by tiny scheduler
long int ts_systime() {
    return (long int)HAL_GetTick();
}


// LED cylon eye
GPIO_TypeDef* LEDPorts[4] = {LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port};
uint16_t LEDPins[4] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin};
int i = 1;
int direction = 1;

void LED_loop(tiny_task_t* t) {
    HAL_GPIO_TogglePin(LEDPorts[i], LEDPins[i]);
    if(i == 3) {
        direction *= -1;
    } else if(i == 0) {
        direction *= -1;
    }
    i += direction;
    HAL_GPIO_TogglePin(LEDPorts[i], LEDPins[i]);

    t->start_time = ts_systime() + 50; // run again in 50ms
}


// HARDWARE CHECKING
// TODO probably get rid of this later
#define IMU_ADDR 0b11010111
#define IMU_WHOAMI 0x0F
#define IMU_MAG_ADDR 0b00111101

#define MPL_ADDR 0xC0
#define MPL_WHOAMI 0x0C

#define HIGHG_ADDR 0x31
#define HIGHG_WHOAMI 0x0F

uint8_t buff[4];
void check_hw(tiny_task* task) {
    memset(buff, 0, 4);
    //HAL_StatusTypeDef stat;

    // LED_loop();
    HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, IMU_WHOAMI, 1, &buff[0], 1, 200);
    printf("accel + gyro whoami(0x68): 0x%0x\r\n", buff[0]);
    // HAL_Delay(500);
    HAL_I2C_Mem_Read(&hi2c1, IMU_MAG_ADDR, IMU_WHOAMI, 1, &buff[1], 1, 200);
    printf("mag whoami(0x3d): 0x%0x\r\n", buff[1]);
    // HAL_Delay(500);
    HAL_I2C_Mem_Read(&hi2c1, MPL_ADDR, MPL_WHOAMI, 1, &buff[2], 1, 200);
    printf("mpl whoami(0xc4): 0x%0x\r\n", buff[2]);
    // HAL_Delay(500);
    HAL_I2C_Mem_Read(&hi2c1, HIGHG_ADDR, HIGHG_WHOAMI, 1, &buff[3], 1, 200);
    printf("high-g whoami(0x32): 0x%0x\r\n", buff[3]);
    printf("\r\n");
    // HAL_Delay(500);
    // HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);

    task->start_time = ts_systime() + 2000; // run again in 2000ms
}


// test SPI flash
void SPI_flash(tiny_task_t*) {
    uint8_t test;
    W25qxx_EraseBlock(0);
    W25qxx_WriteByte('a', 0);
    W25qxx_ReadByte(&test, 0);
    printf("got: %c\r\n", (char)test);
}

int gps_i = 0;
// test GPS
void GPS_test(tiny_task_t* task) {
    // gga_packet_t data = MTK3339::read_packet();
	// printf("GPS data collected at %s\n", data.time);
    //
    // task->start_time = ts_systime() + 500; // run again in 500ms
}
