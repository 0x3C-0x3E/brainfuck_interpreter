#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    LOG_INFO,
    LOG_ERROR,
    LOG_CRITICAL_ERROR,
} LOG_TYPE;

// stuff that lets gcc check if i am wrong, which i am most of the time
__attribute__((format(printf, 2, 3)))
void log_msg(LOG_TYPE type, const char* format, ...);
