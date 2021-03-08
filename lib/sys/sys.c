/*
*   Retarget the nosys "syscalls"
*   Without these calling a syscall just calls an empty(?) stub
*/
#include "main.h"
#include "lib/common/common.h"
#include "lib/queue/queue.h"
#include <stdint.h>
#include <stdlib.h>

#define enable_irq __ASM volatile ("cpsie i" : : : "memory");
#define disable_irq __ASM volatile ("cpsid i" : : : "memory");

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

// okay to initiate a read?
int read1 = 1;
int read2 = 1;
int read3 = 1;

// okay to initiate a write?
int write1 = 1;
int write2 = 1;
int write3 = 1;

// read queues
queue_t* qr1;
queue_t* qr2;
queue_t* qr3;

// write queues
queue_t* qw1;
queue_t* qw2;
queue_t* qw3;


// declared in common/common.h
// does initialization for using the sys library
// returns -1 on failure, 1 otherwise
int sys_init() {
    // initialize queues
    // TODO perhaps sort and add priorities?
    qr1 = q_mkqueue(NULL);
    qr2 = q_mkqueue(NULL);
    qr3 = q_mkqueue(NULL);
    qw1 = q_mkqueue(NULL);
    qw2 = q_mkqueue(NULL);
    qw3 = q_mkqueue(NULL);

    // something didn't initialize
    if(!qr1 || !qr2 || !qr3 || !qw1 || !qw2 || !qw3) {
        return -1;
    }

    return 1;
}

// message type
typedef struct {
    uint8_t* data;
    uint16_t len;
} msg_node_t;

// TX complete, called by the HAL UART IRQ
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    void* next = NULL;
    if(huart == &huart1) {
        write1 = 1;
        next = q_dequeue(qw1);
    } else if(huart == &huart2) {
        write2 = 1;
        next = q_dequeue(qw2);
    } else if(huart == &huart3) {
        write3 = 1;
        next = q_dequeue(qw3);
    }

    if(next) {
        HAL_UART_Transmit_DMA(huart, ((msg_node_t*)next)->data, ((msg_node_t*)next)->len);
        free(next);
    }
}

// RX complete, called by the HAL UART IRQ
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    void* next = NULL;
    if(huart == &huart1) {
        read1 = 1;
        next = q_dequeue(qr1);
    } else if(huart == &huart2) {
        read2 = 1;
        next = q_dequeue(qr2);
    } else if(huart == &huart3) {
        read3 = 1;
        next = q_dequeue(qr3);
    }

    if(next) {
        HAL_UART_Receive_DMA(huart, ((msg_node_t*)next)->data, ((msg_node_t*)next)->len);
        free(next);
    }
}


int _write(int file, char *ptr, int len) {
    msg_node_t* msg = (msg_node_t*)malloc(sizeof(msg_node_t));
    msg->data = (uint8_t*)ptr;
    msg->len = len;

    disable_irq;
    switch(file) {
        case 0: // UART 3 (XBee)
            if(write3) { // okay to start a tx
                write3 = 0;
                HAL_UART_Transmit_DMA(&huart3, (uint8_t*)ptr, len);
                free(msg);
            } else { // busy, queue the write
                q_enqueue(qw3, (void*)msg);
            }
            break;
        case 1: // UART 1 (Debug / Camera)
            if(write1) { // okay to start a tx
                write1 = 0;
                HAL_UART_Transmit_DMA(&huart1, (uint8_t*)ptr, len);
                free(msg);
            } else { // busy, queue the write
                q_enqueue(qw1, (void*)msg);
            }
            break;
        case 2: // UART 2 (GPS)
            if(write2) { // okay to start a tx
                write2 = 0;
                HAL_UART_Transmit_DMA(&huart2, (uint8_t*)ptr, len);
                free(msg);
            } else { // busy, queue the write
                q_enqueue(qw2, (void*)msg);
            }
            break;
        default:
            enable_irq;
            return -1;
    }
    enable_irq;
    return len;
}


int _read(int file, char* ptr, int len) {
    msg_node_t* msg = (msg_node_t*)malloc(sizeof(msg_node_t));
    msg->data = (uint8_t*)ptr;
    msg->len = len;

    switch(file) {
        case 0: // UART 3 (XBee)
            // HAL_UART_Receive(&huart3, (uint8_t*)ptr, len, 10);
            if(read3) { // okay to start a rx
                read3 = 0;
                HAL_UART_Receive_DMA(&huart3, (uint8_t*)ptr, len);
                free(msg);
            } else { // busy, queue the read
                q_enqueue(qr3, (void*)msg);
            }
            break;
        case 1: // UART 1 (Debug / Camer)
            // HAL_UART_Receive(&huart1, (uint8_t*)ptr, len, 10);
            if(read2) { // okay to start a rx
                read2 = 0;
                HAL_UART_Receive_DMA(&huart1, (uint8_t*)ptr, len);
                free(msg);
            } else { // busy, queue the read
                q_enqueue(qr2, (void*)msg);
            }
            break;
        case 2: // UART 2 (GPS)
            // TODO perhaps change this to do a character match for '\n'?
            // HAL_UART_Receive(&huart2, (uint8_t*)ptr, len, 10);
            if(read2) { // okay to start a rx
                read2 = 0;
                HAL_UART_Receive_DMA(&huart2, (uint8_t*)ptr, len);
                free(msg);
            } else { // busy, queue the read
                q_enqueue(qr2, (void*)msg);
            }
            break;
        default:
            return -1;
    }
    return len;
}
