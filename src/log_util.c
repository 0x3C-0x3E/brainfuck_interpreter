#include "log_util.h"


__attribute__((format(printf, 2, 3)))
void log_msg(LOG_TYPE type, const char* format, ...) {
    char* log_msg;
    switch (type) {
        case LOG_INFO: log_msg = "INFO"; break;
        case LOG_ERROR: log_msg = "ERROR"; break;
        case LOG_CRITICAL_ERROR: log_msg = "CRIT_ERROR"; break;
    }
    printf("[%s] ", log_msg);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

}
