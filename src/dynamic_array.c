#include "dynamic_array.h"
#include <stdlib.h>
#include <string.h>

DArray d_array_create(size_t data_size) {
    DArray new_array = {
        .start = malloc(data_size),
        .data_size = data_size,
    };
    return new_array;
}

void d_array_append(DArray* d_array, void* data) {
    // check if array is full
    if (d_array->size == d_array->capacity) {
        size_t new_capacity = d_array->capacity == 0 ? 4 : d_array->capacity * 2;
        void* new_start = realloc(d_array->start, d_array->data_size* new_capacity);
        if (new_start == NULL) {
            printf("!! D_ARRAY: REALLOC FAILED\n");
            return;
        }
    }

    void* target = (char*)d_array->start + (d_array->size * d_array->data_size);
    memcpy(target, data, d_array->data_size);
    d_array->size ++;
}

void* d_array_get(DArray* d_array, size_t index) {
    return d_array->start + (index * d_array->data_size);
}
