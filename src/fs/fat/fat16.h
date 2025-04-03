#ifndef FAT16_H
#define FAT16_H
#include "config.h"
#include "fs/file.h"

struct filesystem* fat16_init();
void fat16_read_dir(const char* str);
int fat16_read_dir_return(const char* path, char output[][PEACHOS_MAX_PATH], int max_items);
int fat16_mkdir(const char* path);
void fat16_cat(const char* path);

#endif