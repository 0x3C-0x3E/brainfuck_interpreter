#pragma once
#include "log_util.h"
#include <stdio.h>
#include <string.h>

typedef struct InputFile {
    const char* filepath;
    FILE* file_ptr;
    size_t file_size;

} InputFile;

int init_file(const char* filepath, InputFile* input_file);
