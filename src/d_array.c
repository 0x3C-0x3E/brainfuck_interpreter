#include "d_array.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

D_Array* d_array_new(size_t data_size) {
    D_Array* new_array = malloc(sizeof(D_Array));
    if (new_array == NULL) {
        goto failed_init;
    }

    void* new_data = malloc(0);
    if (new_array == NULL) {
        goto failed_init;
    }

    *new_array = (D_Array){
        .data = new_data,
        .data_size = data_size,
        .size = 0,
        .capacity = 0,
    };
    return new_array;

failed_init:
    perror("[ERROR] D_Array failed to init\n");
    return NULL;
}

void d_array_append(D_Array* d_array, void* data) {
    // check if we already exhausted the capacity and need to realloc
    if (d_array->size == d_array->capacity) {
        size_t new_capacity = (d_array->capacity != 0) ? d_array->capacity * 2 : 2;

        void* new_data = realloc(d_array->data, new_capacity * d_array->data_size);
        if (new_data == NULL) {
            perror("[ERROR] D_Array failed to realloc\n");
            return;
        }
        
        d_array->capacity = new_capacity;
        d_array->data = new_data;
    }

    
    void* target = d_array->data + ((d_array->size) * d_array->data_size);
    mempcpy(target, data, d_array->data_size);

    d_array->size ++;
}

void* d_array_pop(D_Array* d_array) {
    void* target = d_array->data + ((d_array->size - 1) * d_array->data_size);
    d_array->size --;

    return target;
}

void* d_array_get(D_Array* d_array, int index) {
    void* target = d_array->data + (index * d_array->data_size);

    return target;
}

void d_array_remove(D_Array* d_array, int index) {
    for (size_t i = index; i < d_array->size; i++) {
        void* new_value = d_array_get(d_array, i + 1);
        void* old_value = d_array_get(d_array, i);
        
        mempcpy(old_value, new_value, d_array->data_size);
    }

    d_array->size --;
}

void d_array_free(D_Array* d_array) {
    free(d_array->data);
    free(d_array);
}
