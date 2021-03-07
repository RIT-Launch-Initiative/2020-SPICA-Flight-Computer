#include "init.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include <string.h>

HAL_StatusTypeDef stat = HAL_OK;

char* failure = "failed!\r\n";
char* IRQ = "IRQ Executed!\r\n";
char* loop = "loop\r\n";

char echo_gps[1];
char echo_xbee[1];
char echo_debug[1];

int gps_read = 0;
int xbee_read = 0;
int debug_read = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart == &huart2) { // gps
        gps_read = 1;
    } else if(huart == &huart1) { // debug
        debug_read = 1;
    } else if(huart == &huart3) { //xbee
        xbee_read = 1;
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    HAL_UART_Transmit(&huart1, (uint8_t*)failure, strlen(failure), 10);
}

int init() {
    while(1) {
        if(gps_read) { // gps -> debug
            HAL_UART_Transmit(&huart1, (uint8_t*)echo_gps, 1, 10);
            gps_read = 0;
        }
        if(xbee_read) { // xbee -> debug
            HAL_UART_Transmit(&huart1, (uint8_t*)echo_xbee, 1, 10);
            xbee_read = 0;
        }
        if(debug_read) { // debug -> xbee
            HAL_UART_Transmit(&huart3, (uint8_t*)echo_debug, 1, 10);
            debug_read = 0;
        }

        stat = HAL_UART_Receive_DMA(&huart3, (uint8_t*)&echo_debug, 1);
        stat = HAL_UART_Receive_DMA(&huart1, (uint8_t*)&echo_xbee, 1);
        stat = HAL_UART_Receive_DMA(&huart2, (uint8_t*)echo_gps, 1);
    }
}
