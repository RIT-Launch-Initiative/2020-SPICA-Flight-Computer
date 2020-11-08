/*
* Defines "write" function for use with stdio lib (part of newlib libc)
* Send help
*/
#include "main.h"
#include <stdint.h>

extern UART_HandleTypeDef huart1;

int _write(int file, char *ptr, int len) {
    // ignore the file, I guess
    HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 10);
    return len;
}
