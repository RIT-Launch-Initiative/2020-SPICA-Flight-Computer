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
        case 0:
            HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 10);
            break;
        case 1:
            HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 10);
            break;
    }
    return len;
}
