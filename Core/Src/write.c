/*
* Defines "write" function for use with stdio lib (part of newlib libc)
* Send help
*/
#include "main.h"
#include <stdint.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

int _write(int file, char *ptr, int len) {
    switch(file) {
        case 0: // this should be whatever UART the XBee is on
            HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 10);
            break;
        case 1: // this should be whatever UART the debug/camera line is
            HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 10);
            break;
        case 2:
            HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, 10);
            break;
        default:
            return -1;
    }
    return len;
}
