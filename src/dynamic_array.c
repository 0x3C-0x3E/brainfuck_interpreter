#include "dynamic_array.h"
#include "log_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void d_array_create(DArray* d_array, size_t data_size) {
    *d_array = (DArray) {
        .start = malloc(1 * data_size),
        .data_size = data_size,
        .size = 1,
    };


    if (d_array->start == NULL) {
        printf("Error during initialisation of d_array");
    }
}

void d_array_append(DArray* d_array, void* data) {
    d_array->size ++;
    void* new_start = realloc(d_array->start, d_array->data_size * d_array->size);
    if (!new_start) {
        log_msg(LOG_CRITICAL_ERROR, "REALLOC FAILED!\n");
        return;
    }

    void* target = (char*)d_array->start + ((d_array->size - 1) * d_array->data_size);
    memcpy(target, data, d_array->data_size);
}

void d_array_append_clear(DArray* d_array) {
    unsigned char a = 0;
    d_array_append(d_array, &a);
}

void* d_array_get(DArray* d_array, size_t index) {
    return d_array->start + (index * d_array->data_size);
}

void d_array_free(DArray* d_array) {
    free(d_array->start);
    free(d_array);
}
