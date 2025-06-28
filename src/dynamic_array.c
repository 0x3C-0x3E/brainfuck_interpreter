#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void d_array_create(DArray* d_array, size_t data_size) {
    *d_array = (DArray) {
        .start = malloc(data_size),
        .data_size = data_size,
    };

    if (d_array->start == NULL) {
        printf("Error during initialisation of d_array");
    }
}

void d_array_append(DArray* d_array, void* data) {
    // check if array is full
    if (d_array->size == d_array->capacity) {
        size_t new_capacity = d_array->capacity == 0 ? 4 : d_array->capacity * 2;
        d_array->start = realloc(d_array->start, d_array->data_size* new_capacity);
        d_array->capacity = new_capacity;
    }

    void* target = (char*)d_array->start + (d_array->size * d_array->data_size);
    memcpy(target, data, d_array->data_size);
    d_array->size ++;
}

void* d_array_get(DArray* d_array, size_t index) {
    return d_array->start + (index * d_array->data_size);
}

void d_array_free(DArray* d_array) {
    free(d_array->start);
    free(d_array);
}
