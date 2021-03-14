#ifndef COMMON_H
#define COMMON_H

typedef enum {
    RET_ERROR,
    RET_BLOCKED,
    RET_OK
} RetType;

// defined in lib/sys/sys.c
int sys_init();

// enable / disable interrupt masks on the CPU
#define enable_irq __ASM volatile ("cpsie i" : : : "memory");
#define disable_irq __ASM volatile ("cpsid i" : : : "memory");

// basic I/O (sys.c, retargeted libc functions used by stdio)
int _write(int file, char *ptr, int len);
int _read(int file, char *ptr, int len);

#endif
