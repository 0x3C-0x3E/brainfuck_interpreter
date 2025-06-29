#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
    // ptr to start of data
    void* data;
    
    // theoretical capacity
    size_t capacity;

    // actual amout of elements 
    size_t size;

    // individual size of the element
    size_t data_size;
} D_Array;

// this returns a pointer to a heap-allocated array with size 0, so no 
// elements are inserted at creation and the array is empty
D_Array* d_array_new(size_t data_size);

// void* data gets copied into the array so there is no 
// need to check for livetimes
void d_array_append(D_Array* d_array, void* data);

void* d_array_pop(D_Array* d_array);

void* d_array_get(D_Array* d_array, int index);

void d_array_remove(D_Array* d_array, int index);

void d_array_free(D_Array* d_array);
