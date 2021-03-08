#ifndef COMMON_H
#define COMMON_H

typedef enum {
    RET_ERROR,
    RET_BLOCKED,
    RET_OK
} RetType;

// defined in lib/sys/sys.c
int sys_init();

#endif
