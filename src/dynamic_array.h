#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    void* start;
    size_t size;
    size_t capacity;
    size_t data_size;
} DArray;

DArray d_array_create(size_t data_size);

void d_array_append(DArray* d_array, void* data);

void* d_array_get(DArray* d_array, size_t index);
