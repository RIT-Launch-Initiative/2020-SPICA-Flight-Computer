/*
*   Filesystem Utilities Header
*/

#ifndef FS_H
#define FS_H

#include "lib/TinyScheduler/ts.h"

// return values
#define FS_OK 0
#define FS_ALREADY_OPEN
#define FS_NOTHING_OPEN
#define FS_NO_SPACE
#define FS_FAIL

// open a new 'file' in the file system for a new flight
// can have ONE file opened globally
// if call this function twice without calling fs_close, returns error
// returns a file descriptor
int fs_open();

// close the globally opened file
int fs_close();

// write len bytes from data to the currently opened file
int fs_write(uint8_t* data, size_t len);

// dump all files out to file descriptor fd (calls fprintf with that value)
void dump_files((FILE*)fd);

// if this function is called, fs_write will overwrite other files if the system runs out of space
// should only be set for very high priority writes (e.g. an actual flight)
void enable_overwrite();

// disables overwrite
void disable_overwrite();

// needs to be added to the scheduler
// writes are queued and this task is called to flush the buffer to the storage device periodically
extern tiny_task_t fs_task();

#endif
