#ifndef FAT16_H
#define FAT16_H

#include "fs/file.h"

struct filesystem* fat16_init();
void fat16_read_dir(const char* str);
int fat16_mkdir(const char* path);
#endif