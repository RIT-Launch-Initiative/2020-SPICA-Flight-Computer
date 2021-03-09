#include "ringbuff.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t buff[3];
ringbuff_t rb;

uint8_t out[3];
uint8_t test[3];
uint8_t dummy[3];

int main() {
    rbuff_init(&rb, buff, 3);
    test[0] = 1;
    test[1] = 2;
    test[2] = 3;

    dummy[0] = 54;
    dummy[1] = 55;
    dummy[2] = 56;

    rbuff_memcpyin(&rb, test, 3);
    rbuff_memcpyin(&rb, dummy, 3); // shouldn't do anything
    rbuff_memcpyout(out, &rb, 1);
    rbuff_memcpyin(&rb, &test[2], 1);
    rbuff_memcpyout(out, &rb, 3);
    rbuff_memcpyout(out, &rb, 3);

    printf("%i, %i, %i\n", test[0], test[1], test[2]); // {1,2,3}
    printf("%i, %i, %i\n", out[0], out[1], out[2]); // {2,3,3}
    printf("%i, %i, %i\n", dummy[0], dummy[1], dummy[2]); // {54,55,56}
}
