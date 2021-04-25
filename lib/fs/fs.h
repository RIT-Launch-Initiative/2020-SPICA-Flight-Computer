/*
*   Filesystem Utilities Header
*/
#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <stdio.h>

#define PAGE_SIZE 256
#define NUM_FILES 12 // number of files supported

// return values
#define FS_OK 0
#define FS_ALREADY_OPEN 1
#define FS_NOTHING_OPEN 2
#define FS_NO_SPACE 3
#define FS_FAIL 4

// open a new 'file' in the file system for a new flight
// can have ONE file opened globally
// if call this function twice without calling fs_close, returns error
// returns a file descriptor
int fs_open();

// close the globally opened file
// int fs_close();

// write len bytes from data to the currently opened file
int fs_write(uint8_t* data, uint16_t len);

// dump all files out to file descriptor fd (calls fprintf with that value)
void dump_files(FILE* fd);

// if this function is called, fs_write will overwrite other files if the system runs out of space
// should only be set for very high priority writes (e.g. an actual flight)
// void enable_overwrite();

// disables overwrite
// void disable_overwrite();

#endif
