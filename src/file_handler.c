#include "file_handler.h"


int init_file(const char* filepath, InputFile* input_file) {
    input_file->filepath = filepath;
    log_msg(LOG_INFO, "filepath is: %s\n", input_file->filepath);

    input_file->file_ptr = fopen(input_file->filepath, "r");
    if (input_file->file_ptr == NULL) {
        log_msg(LOG_CRITICAL_ERROR, "Error opening file");
        return 1;
    }

    fseek(input_file->file_ptr, 0, SEEK_END);
    input_file->file_size = ftell(input_file->file_ptr);
    fseek(input_file->file_ptr, 0, SEEK_SET);
    log_msg(LOG_INFO, "filesize is: %ld\n", input_file->file_size);

    return 0;
}
