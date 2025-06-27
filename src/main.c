#include "dynamic_array.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

typedef enum {
    LOG_INFO,
    LOG_ERROR,
    LOG_CRITICAL_ERROR,
} LOG_TYPE;

// stuff that lets gcc check if i am wrong, which i am most of the time
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

typedef struct InputFile {
    const char* filepath;
    FILE* file_ptr;
    size_t file_size;

} InputFile;

InputFile input_file = {0};
DArray parsed_chars = {0};
const char* valid_tokens = "><+-.,[]";

int init_file(const char* filepath) {
    input_file.filepath = filepath;
    log_msg(LOG_INFO, "filepath is: %s\n", input_file.filepath);

    input_file.file_ptr = fopen(input_file.filepath, "r");
    if (input_file.file_ptr == NULL) {
        log_msg(LOG_CRITICAL_ERROR, "Error opening file");
        return 1;
    }

    fseek(input_file.file_ptr, 0, SEEK_END);
    input_file.file_size = ftell(input_file.file_ptr);
    fseek(input_file.file_ptr, 0, SEEK_SET);
    log_msg(LOG_INFO, "filesize is: %ld\n", input_file.file_size);

    return 0;
}

int parse_chars() {
    parsed_chars = d_array_create(sizeof(char));
    for (size_t i = 0; i < input_file.file_size; i++) {
        char c = (char) fgetc(input_file.file_ptr);
        if (strchr(valid_tokens, c) != NULL) {
            d_array_append(&parsed_chars, &c);
        }
    }
    return 0; 
}

typedef enum {
    IR_DEC_DP = '>',
    IR_INC_DP = '<',
    IR_ADD = '+',
    IR_SUB = '-',
    IR_OUT = '.',
    IR_IN = ',',
    IR_JIZ = '[',
    IR_JNZ = ']',
} IR_Type;

typedef struct {
    IR_Type type;

    // this either describes the amount gathered by the IR operation of how often to execute the type
    // or the index of where to jump to if IR_JIZ or IR_JNZ
    int operation;
} IROperation;

DArray ir_operations = {0};


void handle_ir_operation(DArray* ir_operations, IROperation* prev_operation, IR_Type type) {
    if (prev_operation->type == type) {
        log_msg(LOG_INFO, "adding +1 to IRTYPE: %d\n", type);
        prev_operation->operation += 1;
        return;
    }
    
    *prev_operation = (IROperation) {
        .type = type,
        .operation = 1,
    };

    log_msg(LOG_INFO, "found new IRTYPE: %c\n", type);

    d_array_append(ir_operations, prev_operation);
}

int generate_ir() {
    ir_operations = d_array_create(sizeof(IROperation));
    IROperation prev_operation;

    for (size_t i = 0; i < parsed_chars.size; i++) {
        char c = *(char*)d_array_get(&parsed_chars, i);
        IR_Type t = (IR_Type) c;
        handle_ir_operation(&ir_operations, &prev_operation, t);
    } 
    return 0; 
}

int main(int argc, char** argv) {
    if (argc < 2) {
        log_msg(LOG_CRITICAL_ERROR, "insufficiant number of arguments provided}\n");
        return 1;
    }

    if (init_file(argv[1]) != 0) {
        return 1;
    }

    if (parse_chars() != 0) {
        return 1;
    }

    if (generate_ir() != 0) {
        return 1;
    }

    return 0;
}
