#include "fs.h"
#include "lib/w25qxx/w25qxx.h"
#include <stdint.h>
#include <string.h>

// 64 files w/ a page size of 256 bytes
// each file needs a 32-bit page size at it's index on the first (0th) page
#define MAX_NUM_FILES (PAGE_SIZE / sizeof(uint32_t))

// the number of pages we're using
static uint32_t num_pages = 0;

// our file index
// should be big enough to fit MAX_NUM_FILES values and -1
static int8_t file_index = -1;

// buffer to hold data until we have a full page's worth to write
uint8_t buffer[PAGE_SIZE];

// offset into file_buffer we have currently written to
uint8_t buffer_index = 0;

// next block to write to when the buffer is full
static uint32_t next_page;

// boolean flag for if the flash chip has been initialized
static uint8_t flash_open = 0;

// open the filesystem with a new file
int fs_open() {
    if(!flash_open) {
        if(!W25qxx_Init()) {
            return FS_FAIL;
        }
        flash_open = 1;
    }

    if(w25qxx.PageSize != PAGE_SIZE) {
        #ifdef DEBUG
        printf("Error: page size mismatch with flash filesystem!\r\n");
        #endif

        return FS_FAIL;
    }

    // first page
    // should be the size of PAGE_SIZE
    // holds MAX_NUM_FILES values representing numbers of pages used at index
    // e.g. index 0 is file 0 and contains the number of pages used by file 0
    uint32_t config_page[MAX_NUM_FILES];

    // read in the first page, which is always the configuration page
    W25qxx_ReadPage((uint8_t*)config_page, 0, 0, PAGE_SIZE);

    // find the first file entry we can use
    uint32_t start_page = 1;
    for(int i = 0; i < MAX_NUM_FILES; i++) {
        if(config_page[i] == 0) { // we found an unused file!
            file_index = i;
            next_page = start_page;
        } else {
            start_page += config_page[i];
        }
    }

    if(file_index == -1) {
        #ifdef DEBUG
        printf("Error: no unused files in filesystem!\r\n");
        #endif

        return FS_NO_SPACE;
    }

    return FS_OK;
}


int fs_write(uint8_t* data, uint16_t len) {
    if(file_index == -1) {
        return FS_NOT_OPEN;
    }

    if(len + buffer_index > PAGE_SIZE) {
        // copy what we can to the buffer
        uint16_t empty = PAGE_SIZE - buffer_index;
        memcpy(buffer + buffer_index, data, empty);

        // check if we can have space
        if(next_page > w25qxx.PageCount) {
            return FS_NO_SPACE;
        }

        // flush the buffer to the flash chip
        W25qxx_WritePage(buffer, next_page++, 0, PAGE_SIZE);

        // increment the number of pages we're using and write it to the config page
        num_pages++;
        W25qxx_WritePage((uint8_t*)&num_pages, 0, (file_index * sizeof(uint32_t)), sizeof(uint32_t));

        // put the remaining data in the beginning of the buffer
        memcpy(buffer, data + empty, len - empty);
        buffer_index = len - empty;
    } else {
        memcpy(buffer + buffer_index, data, len);
    }

    return FS_OK;
}


int dump_files(FILE* fd) {
    if(!flash_open) {
        if(!W25qxx_Init()) {
            return FS_FAIL;
        }
        flash_open = 1;
    }

    // read in the first page, which is always the configuration page
    // contains uint32's that represent how many pages each file is using
    uint32_t config_page[MAX_NUM_FILES];
    W25qxx_ReadPage((uint8_t*)config_page, 0, 0, PAGE_SIZE);

    uint32_t page = 1;
    uint8_t buff[PAGE_SIZE];
    for(int i = 0; i < MAX_NUM_FILES; i++) {
        fprintf(fd, "FILE %i:\n", i);
        for(int j = 0; j < config_page[i]; j++) {
            W25qxx_ReadPage(buff, page++, 0, PAGE_SIZE);
            fwrite(buff, 1, PAGE_SIZE, fd); // write out the file descriptor
        }
    }
    return FS_OK;
}

int wipe_fs() {
    if(!flash_open) {
        if(!W25qxx_Init()) {
            return FS_FAIL;
        }
        flash_open = 1;
    }

    W25qxx_EraseChip();
    
    return FS_OK;
}
