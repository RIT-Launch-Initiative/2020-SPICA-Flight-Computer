#include "lib/ring_buffer/ringbuff.h"
#include <stdint.h>
#include <stdlib.h>

void rbuff_init(ringbuff_t* rb, uint8_t* buff, size_t size) {
    rb->head = buff;
    rb->tail = buff;
    rb->ptr = buff;
    rb->allocated = 0;
    rb->size = size;
}


size_t rbuff_memcpyin(ringbuff_t* rb, uint8_t* buff, size_t len) {
    if(rb->allocated == rb->size) {
        return 0;
    }

    size_t i = 0; // index of buff

    uint8_t* max;
    if(rb->tail <= rb->head) {
        max = rb->ptr + rb->size;
    } else {
        max = rb->head;
    }

    while(rb->tail < max && i < len) {
        *(rb->tail) = buff[i];
        i++;
        rb->tail++;
    }

    // we reached the end and need to wrap around
    if(i < len) {
        rb->tail = rb->ptr;
        while(rb->tail < rb->head) {
            *(rb->tail) = buff[i];
            i++;
            rb->tail++;
        }
    }
    // don't overrun and return number of bytes copied
    rb->allocated += i;
    return i;
}


size_t rbuff_memcpyout(uint8_t* buff, ringbuff_t* rb, size_t len) {
    if(rb->allocated == 0) {
        return 0;
    }

    size_t i = 0;

    uint8_t* max;
    if(rb->head < rb->tail) {
        max = rb->tail;
    } else {
        max = rb->ptr + rb->size;
    }

    while(rb->head < max && i < len) {
        buff[i] = *(rb->head);
        i++;
        rb->head++;
    }

    if(i < len) { // wrap around
        rb->head = rb->ptr;
        while(rb->head < rb->tail) {
            buff[i] = *(rb->head);
            i++;
            rb->head++;
        }
    }
    rb->allocated -= i;
    return i;
}
