#ifndef RINGBUFF_H
#define RINGBUFF_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int head; // first byte occupied at beginning
    int tail; // first byte free at end
    uint8_t* ptr; // ptr to beginning of buffer
    int max; // max size
} ringbuff_t;

// need to pass in a buffer of size 'size' for the ring buffer to use
// assumes buffer and ringbuffer struct are preallocated
// sets head and tail correctly
void rbuff_init(ringbuff_t* rb, uint8_t* buff, size_t size);

// copy len bytes from buff into rb
// returns number of bytes copied
size_t rbuff_memcpyin(ringbuff_t* rb, uint8_t* buff, size_t len);

// copy len bytes from rb into buff
// returns number of bytes copied
size_t rbuff_memcpyout(uint8_t* buff, ringbuff_t* rb, size_t len);

#endif
