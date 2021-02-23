#include "init.h"
#include "stm32f1xx_hal.h"
#include "usart.h"

uint8_t echo;
HAL_StatusTypeDef stat;

int init() {
    while(1) {
        // XBee -> debug
        stat = HAL_UART_Receive(&huart2, &echo, 1, 10);
        if(stat == HAL_OK) {
            HAL_UART_Transmit(&huart1, &echo, 1, 10);
        }

        // debug -> XBee
        stat = HAL_UART_Receive(&huart1, &echo, 1, 10);
        if(stat == HAL_OK) {
            HAL_UART_Transmit(&huart2, &echo, 1, 10);
        }
    }
}
