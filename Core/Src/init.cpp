#include "init.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
    #include "lib/fs/fs.h"
    #include "lib/common/common.h"
}


int init() {
    if(sys_init() == -1) {
        while(1) {}; // loop forever and don't buzz
    }

    // buzz for 5s
    HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
    HAL_Delay(5000);
    HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);

    // dump filesystem out the UART
    if(fs_dump_files((FILE*)1) == FS_FAIL) {
        printf("\r\nDUMP FAIL\r\n");
    }

    // loop forever
    while(1) {};

    return -1; // should never return
}
