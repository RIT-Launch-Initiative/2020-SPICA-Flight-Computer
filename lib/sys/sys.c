/*
*   Retarget the nosys/newlib "syscalls"
*   Without these calling a syscall just calls an empty(?) stub
*/
#include "main.h"
#include "usart.h"
#include "gps.h"
#include "lib/common/common.h"
#include "lib/queue/queue.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// okay to initiate a write?
int write1 = 1;
int write2 = 1;
int write3 = 1;

// write queues
queue_t* qw1;
queue_t* qw2;
queue_t* qw3;

// write pointers
char* wptr1;
char* wptr2;
char* wptr3;

// message type used for queueing messages
typedef struct {
    uint8_t* data;
    uint16_t len;
} msg_node_t;


// declared in common/common.h
// does initialization for using the sys library
// returns -1 on failure, 1 otherwise
int sys_init() {
    // initialize queues
    // TODO perhaps sort and add priorities?
    qw1 = q_mkqueue(NULL);
    qw2 = q_mkqueue(NULL);
    qw3 = q_mkqueue(NULL);

    // something didn't initialize
    if(!qw1 || !qw2 || !qw3) {
        return -1;
    }

    return 1;
}


// TX complete, called by the HAL UART IRQ
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    void* next = NULL;
    if(huart == &huart1) {
        write1 = 1;
        next = q_dequeue(qw1);
        free(wptr1);
        wptr1 = (char*)((msg_node_t*)next)->data;
    } else if(huart == &huart2) {
        write2 = 1;
        next = q_dequeue(qw2);
        free(wptr2);
        wptr2 = (char*)((msg_node_t*)next)->data;
    } else if(huart == &huart3) {
        write3 = 1;
        next = q_dequeue(qw3);
        free(wptr3);
        wptr3 = (char*)((msg_node_t*)next)->data;
    }

    if(next) {
        HAL_UART_Transmit_DMA(huart, ((msg_node_t*)next)->data, ((msg_node_t*)next)->len);
        free(next);
    }
}


// RX complete, called by the HAL UART IRQ
// TODO probably want to make each uart have a function handle for other things to set
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart == &huart1) {
        // TODO (debug uart)
    } else if(huart == &huart2) {
        gps_RxCallback(); // from gps.c
    } else if(huart == &huart3) {
        // TODO (XBee uart)
    }
}


// retarget _write for stdio functions to use
// nonblocking, initiates a DMA transfer or queues it if the UART controller is busy
// if an error happens it likely won't caught since the function will have already returned
int _write(int file, char *ptr, int len) {
    msg_node_t* msg = (msg_node_t*)malloc(sizeof(msg_node_t));
    msg->data = (uint8_t*)malloc(sizeof(char) * len);
    memcpy(msg->data, ptr, len);
    msg->len = len;

    disable_irq;
    switch(file) {
        case 0: // UART 3 (XBee)
            if(write3) { // okay to start a tx
                write3 = 0;
                wptr3 = (char*)msg->data;
                HAL_UART_Transmit_DMA(&huart3, msg->data, len);
                free(msg);
            } else { // busy, queue the write
                q_enqueue(qw3, (void*)msg);
            }
            break;
        case 1: // UART 1 (Debug / Camera)
            if(write1) { // okay to start a tx
                write1 = 0;
                wptr1 = (char*)msg->data;
                HAL_UART_Transmit_DMA(&huart1, msg->data, len);
                free(msg);
            } else { // busy, queue the write
                q_enqueue(qw1, (void*)msg);
            }
            break;
        case 2: // UART 2 (GPS)
            if(write2) { // okay to start a tx
                write2 = 0;
                wptr2 = (char*)msg->data;
                HAL_UART_Transmit_DMA(&huart2, msg->data, len);
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


// read a certain amount of characters (blocking)
// probably best to stay away from (don't use stdio input functions if speed is an issue)
int _read(int file, char* ptr, int len) {
    switch(file) {
        case 0: // UART 3 (XBee)
            if(HAL_OK != HAL_UART_Receive(&huart3, (uint8_t*)ptr, len, 10)) {
                return -1;
            }
            break;
        case 1: // UART 1 (Debug / Camera)
            if(HAL_OK != HAL_UART_Receive(&huart1, (uint8_t*)ptr, len, 10)) {
                return -1;
            }
            break;
        case 2: // UART 2 (GPS)
            if(HAL_OK != HAL_UART_Receive(&huart2, (uint8_t*)ptr, len, 10)) {
                return -1;
            }
            break;
        default:
            return -1;
    }
    return len;
}
