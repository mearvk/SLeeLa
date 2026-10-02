#ifndef SLEELA_SLVM_H
#define SLEELA_SLVM_H

#include <stddef.h>
#include <stdint.h>
#include "garbage_collector.h"
#include "slvm_security.h"
#include "slvm_io_heuristic.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t slvm_word_t;
typedef uint64_t slvm_pc_t;

typedef enum {
    SLVM_OK = 0,
    SLVM_HALTED = 1,
    SLVM_ERROR = -1,
    SLVM_STACK_UNDERFLOW = -2,
    SLVM_STACK_OVERFLOW = -3,
    SLVM_INVALID_OPCODE = -4
} slvm_status_t;

typedef enum {
    SLVM_OP_NOP = 0x00,
    SLVM_OP_HALT = 0x01,
    SLVM_OP_CONST = 0x02,
    SLVM_OP_POP = 0x03,
    SLVM_OP_DUP = 0x04,
    SLVM_OP_ADD = 0x10,
    SLVM_OP_SUB = 0x11,
    SLVM_OP_MUL = 0x12,
    SLVM_OP_DIV = 0x13,
    SLVM_OP_NEG = 0x14,
    SLVM_OP_EQ = 0x20,
    SLVM_OP_LT = 0x21,
    SLVM_OP_GT = 0x22,
    SLVM_OP_JUMP = 0x30,
    SLVM_OP_JUMP_IF_FALSE = 0x31,
    SLVM_OP_RETURN = 0x40
} slvm_opcode_t;

typedef struct {
    const uint8_t *code;
    size_t code_size;
    slvm_word_t *stack;
    size_t stack_capacity;
    size_t stack_size;
    slvm_pc_t pc;
    int halted;
} slvm_t;

void slvm_init(slvm_t *vm, const uint8_t *code, size_t code_size,
               slvm_word_t *stack, size_t stack_capacity);
slvm_status_t slvm_step(slvm_t *vm);
slvm_status_t slvm_run(slvm_t *vm);

#ifdef __cplusplus
}
#endif

#endif
