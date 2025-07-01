#include "d_array.h"
#include "log_util.h"
#include "file_handler.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define VERBOSE_OUTPUT 0
#define DEBUG 0

InputFile input_file = {0};
D_Array* parsed_chars;
const char* valid_tokens = "><+-.,[]";

typedef enum {
    IR_DEC_DP = '<',
    IR_INC_DP = '>',
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
} IRInstruction;

D_Array* ir_instructions;


typedef struct {
    D_Array* memory;
    // not a literal pointer just the index into the d_array memory
    size_t memory_ptr;
    size_t instruction_ptr;

} BF_Interpreter;

BF_Interpreter bf_interpreter = {0};

int init_arrays() {
    parsed_chars = d_array_new(sizeof(char));

    ir_instructions = d_array_new(sizeof(IRInstruction));

    return 0;
}

void dump_parsed_chars() {
    printf("PARSED CHARS DUMP DUMP: size: %zu\n", parsed_chars->size);
    for (size_t i = 0; i < parsed_chars->size; i++) {
        char* c = (char *) d_array_get(parsed_chars, i);
        log_msg(LOG_INFO,"char: %c\n", *c);
    }
}

int parse_chars() {
    for (size_t i = 0; i < input_file.file_size; i++) {
        char c = (char) fgetc(input_file.file_ptr);
        if (strchr(valid_tokens, c) != NULL) {
            d_array_append(parsed_chars, &c);
        }
    }
    return 0; 
}

void handle_ir_operation(IRInstruction* prev_instruction, IR_Type type) {
    if (type == IR_JNZ || type == IR_JIZ) {
        goto handle_new_instruction;
    }

    if (prev_instruction->type == type) {
        prev_instruction->operation += 1;
        return;
    }
    
handle_new_instruction:
    // checking if it is not the first instruction
    if (prev_instruction->operation != 0) {
        d_array_append(ir_instructions, prev_instruction);
    }

    *prev_instruction = (IRInstruction) {
        .type = type,
        .operation = 1,
    };

}

void backpatch() {
    log_msg(LOG_INFO, "Starting backpatch!\n");
    int jump_stack[parsed_chars->size];
    size_t jump_stack_size = 0;

    for (int i = 0; i < ir_instructions->size; i++) {
        IRInstruction* instruction = (IRInstruction*) d_array_get(ir_instructions, i);

        if (instruction->type == IR_JIZ) {
            jump_stack[jump_stack_size] = i;
            jump_stack_size ++;
        }
        if (instruction->type == IR_JNZ) {

            assert(jump_stack_size > 0);
            jump_stack_size --;
            int left_bracket_index = jump_stack[jump_stack_size];


            instruction->operation = left_bracket_index; // we dont need a +1 there cause we increase +1 after each instruction anyway
            IRInstruction* other_instruction = (IRInstruction*) d_array_get(ir_instructions, left_bracket_index);
            other_instruction->operation = i;
            log_msg(LOG_INFO,"Backpatch found closing bracket at %d with left bracket at index %d\n", i, left_bracket_index);
        }
    }
}

int generate_ir() {

    IRInstruction prev_instruction = {0};


    for (size_t i = 0; i < parsed_chars->size; i++) {
        char* c = (char *) d_array_get(parsed_chars, i);
        IR_Type t =  (IR_Type) *c;
        handle_ir_operation(&prev_instruction, t);
    }

    //push last ir_instruction to array
    d_array_append(ir_instructions, &prev_instruction);

    backpatch();
    
    return 0; 
}

void dump_ir() {
    log_msg(LOG_DUMP, "IR DUMP: size: %zu\n", ir_instructions->size);
    for (size_t i = 0; i < ir_instructions->size; i++) {
        IRInstruction* instruction = (IRInstruction*) d_array_get(ir_instructions, i);
        log_msg(LOG_DUMP,"%zu: Type: %c, %d | Operation: %d\n", i, instruction->type, instruction->type, instruction->operation);
    }
    printf("\n\n");
}

void dump_memory() {
    printf("----------------\n");
    for (size_t i = 0; i < bf_interpreter.memory->size; i++) {
        unsigned char* memory_cell = d_array_get(bf_interpreter.memory, i);
        if (bf_interpreter.memory_ptr == i) {
            printf("*");
        } else {
            printf(" ");
        }
        printf("%d |", *memory_cell);
    }
    printf("\n");
    printf("----------------\n");
}

void init_interpreter() {
    log_msg(LOG_INFO, "INITIALIZING INTERPRETER\n");
    bf_interpreter = (BF_Interpreter) {
        .memory = d_array_new(sizeof(unsigned char)),  
        .memory_ptr = 0,
        .instruction_ptr = 0,
    };
    
    // first memory cell
    unsigned char c = 0;
    d_array_append(bf_interpreter.memory, &c);
}

void start_interpreter() {
    int running = 1;
    while (running == 1) {
        IRInstruction* instruction = (IRInstruction*) d_array_get(ir_instructions, bf_interpreter.instruction_ptr);

        unsigned char* memory_cell = d_array_get(bf_interpreter.memory, bf_interpreter.memory_ptr);
        switch (instruction->type) {
            case IR_DEC_DP:
                bf_interpreter.memory_ptr -= instruction->operation;
                if (bf_interpreter.memory_ptr == -1) {
                    bf_interpreter.memory_ptr = 0;
                }
                log_msg(LOG_INTERP_DEBUG, "decrement memory ptr by %d\n", instruction->operation);
                break;
            case IR_INC_DP:
                // bf_interpreter.memory_ptr += instruction->operation;
                // log_msg(LOG_INFO, "Increased memory ptr by: %d\n", instruction->operation);
                // if ((bf_interpreter.memory->capacity-1) == bf_interpreter.memory_ptr) {
                //     // not enough memory
                //     log_msg(LOG_INFO, "had to increase memory array\n");
                //     unsigned char c = 0;
                //     d_array_append(bf_interpreter.memory, &c);
                // }

                for (size_t i = 0; i < instruction->operation; i++) {
                    if ((bf_interpreter.memory->capacity - 1) == bf_interpreter.memory_ptr + i) {
                        unsigned char c = 0;
                        d_array_append(bf_interpreter.memory, &c);
                    }
                }
                bf_interpreter.memory_ptr += instruction->operation;

                log_msg(LOG_INTERP_DEBUG, "increment memory ptr by %d\n", instruction->operation);
                break;
            case IR_ADD:
                *memory_cell += instruction->operation;
                log_msg(LOG_INTERP_DEBUG, "add %d to memory cell of index: %zu\n", instruction->operation, bf_interpreter.memory_ptr);
                break;
            case IR_SUB:
                *memory_cell -= instruction->operation;
                log_msg(LOG_INTERP_DEBUG, "sub %d to memory cell of index: %zu\n", instruction->operation, bf_interpreter.memory_ptr);
                break;
            case IR_OUT:
                for (size_t i = 0; i < instruction->operation; i++) {
                    if (VERBOSE_OUTPUT == 1) {
                        printf("%c | %d\n", (char)*memory_cell, (int)*memory_cell);
                    } else {
                        printf("%c", (char)*memory_cell);
                    }
                }
                break;
            case IR_IN:
                break;
            case IR_JIZ:
                if (*memory_cell == 0) {
                    bf_interpreter.instruction_ptr = instruction->operation;
                    log_msg(LOG_INTERP_DEBUG, "Jump if zero [SUCCESS] jump to memory cell of index: %d\n", instruction->operation);
                } else {
                    log_msg(LOG_INTERP_DEBUG, "Jump if zero [FAIL] jump to memory cell of index: %d\n", instruction->operation);
                }
                break;
            case IR_JNZ:
                if (*memory_cell != 0) {
                    bf_interpreter.instruction_ptr = instruction->operation;
                    log_msg(LOG_INTERP_DEBUG, "Jump if not zero [SUCCESS] jump to memory cell of index: %d\n", instruction->operation);
                } else {
                    log_msg(LOG_INTERP_DEBUG, "Jump if not zero [FAIL] jump to memory cell of index: %d\n", instruction->operation);
                }
                break;
        }

        bf_interpreter.instruction_ptr += 1;
        if (DEBUG) {
            dump_memory();
        }

        if (bf_interpreter.instruction_ptr >= ir_instructions->size) {
            running = 0;
            printf("\n");
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        log_msg(LOG_CRITICAL_ERROR, "insufficiant number of arguments provided}\n");
        return 1;
    }

    if (init_file(argv[1], &input_file) != 0) {
        return 1;
    }

    if (init_arrays() != 0) {
        return 1;
    }

    if (parse_chars() != 0) {
        return 1;
    }

    if (generate_ir() != 0) {
        return 1;
    }

    dump_ir();

    init_interpreter();
    start_interpreter();


    d_array_free(parsed_chars);
    d_array_free(ir_instructions);
    d_array_free(bf_interpreter.memory);

    return 0;
}
