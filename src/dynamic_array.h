#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    void* start;
    size_t size;
    size_t data_size;
} DArray;

void d_array_create(DArray* d_array, size_t data_size);

void d_array_append(DArray* d_array, void* data);

void d_array_append_clear(DArray* d_array);

void* d_array_get(DArray* d_array, size_t index);

void d_array_free(DArray* d_array);
