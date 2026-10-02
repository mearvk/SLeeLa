#include "slvm.h"

#include <string.h>
#include <stdint.h>

static int push(slvm_t *vm, slvm_word_t value) {
    if (vm->stack_size >= vm->stack_capacity) return 0;
    vm->stack[vm->stack_size++] = value;
    return 1;
}

static int pop(slvm_t *vm, slvm_word_t *value) {
    if (vm->stack_size == 0) return 0;
    *value = vm->stack[--vm->stack_size];
    return 1;
}

static int read_u64(const slvm_t *vm, slvm_pc_t at, slvm_word_t *value) {
    if (at + sizeof(uint64_t) > vm->code_size) return 0;
    memcpy(value, vm->code + at, sizeof(uint64_t));
    return 1;
}

static int read_i32(const slvm_t *vm, slvm_pc_t at, int32_t *value) {
    if (at + sizeof(int32_t) > vm->code_size) return 0;
    memcpy(value, vm->code + at, sizeof(int32_t));
    return 1;
}

void slvm_init(slvm_t *vm, const uint8_t *code, size_t code_size,
               slvm_word_t *stack, size_t stack_capacity) {
    if (!vm) return;
    vm->code = code;
    vm->code_size = code_size;
    vm->stack = stack;
    vm->stack_capacity = stack_capacity;
    vm->stack_size = 0;
    vm->pc = 0;
    vm->halted = 0;
    gc_init(&vm->gc, 1024ULL * 1024ULL);
    vm->memory_limit = SLVM_DEFAULT_MEMORY_LIMIT;
}

void slvm_set_memory_limit(slvm_t *vm, size_t bytes) {
    if (!vm) return;
    vm->memory_limit = bytes ? bytes : SLVM_DEFAULT_MEMORY_LIMIT;
}

size_t slvm_get_memory_limit(const slvm_t *vm) {
    return vm ? vm->memory_limit : 0;
}

SLGCObject *slvm_gc_allocate(slvm_t *vm, size_t bytes,
                             SLGCMarkFn mark_children,
                             SLGCDestroyFn destroy,
                             void *context) {
    if (!vm) return NULL;
    const size_t used = gc_bytes(&vm->gc);
    if (bytes > vm->memory_limit || used > vm->memory_limit - bytes) return NULL;
    return gc_allocate(&vm->gc, bytes, mark_children, destroy, context);
}

size_t slvm_gc_collect(slvm_t *vm) {
    return vm ? gc_collect(&vm->gc) : 0;
}

size_t slvm_gc_bytes(const slvm_t *vm) {
    return vm ? gc_bytes(&vm->gc) : 0;
}



slvm_status_t slvm_step(slvm_t *vm) {
    if (!vm || !vm->code || vm->pc >= vm->code_size) return SLVM_ERROR;
    if (vm->halted) return SLVM_HALTED;

    const uint8_t opcode = vm->code[vm->pc++];

    switch ((slvm_opcode_t)opcode) {
        case SLVM_OP_NOP:
            return SLVM_OK;

        case SLVM_OP_HALT:
            vm->halted = 1;
            return SLVM_HALTED;

        case SLVM_OP_CONST: {
            slvm_word_t value;
            if (!read_u64(vm, vm->pc, &value)) return SLVM_ERROR;
            vm->pc += sizeof(uint64_t);
            return push(vm, value) ? SLVM_OK : SLVM_STACK_OVERFLOW;
        }

        case SLVM_OP_POP: {
            slvm_word_t ignored;
            return pop(vm, &ignored) ? SLVM_OK : SLVM_STACK_UNDERFLOW;
        }

        case SLVM_OP_DUP: {
            if (vm->stack_size == 0) return SLVM_STACK_UNDERFLOW;
            return push(vm, vm->stack[vm->stack_size - 1])
                ? SLVM_OK : SLVM_STACK_OVERFLOW;
        }

        case SLVM_OP_ADD:
        case SLVM_OP_SUB:
        case SLVM_OP_MUL:
        case SLVM_OP_DIV: {
            slvm_word_t rhs, lhs;
            if (!pop(vm, &rhs) || !pop(vm, &lhs)) return SLVM_STACK_UNDERFLOW;
            if (opcode == SLVM_OP_ADD) lhs += rhs;
            else if (opcode == SLVM_OP_SUB) lhs -= rhs;
            else if (opcode == SLVM_OP_MUL) lhs *= rhs;
            else {
                if (rhs == 0) return SLVM_ERROR;
                lhs /= rhs;
            }
            return push(vm, lhs) ? SLVM_OK : SLVM_STACK_OVERFLOW;
        }

        case SLVM_OP_NEG: {
            slvm_word_t value;
            if (!pop(vm, &value)) return SLVM_STACK_UNDERFLOW;
            return push(vm, (slvm_word_t)(-(int64_t)value))
                ? SLVM_OK : SLVM_STACK_OVERFLOW;
        }

        case SLVM_OP_EQ:
        case SLVM_OP_LT:
        case SLVM_OP_GT: {
            slvm_word_t rhs, lhs;
            if (!pop(vm, &rhs) || !pop(vm, &lhs)) return SLVM_STACK_UNDERFLOW;
            slvm_word_t result = 0;
            if (opcode == SLVM_OP_EQ) result = lhs == rhs;
            else if (opcode == SLVM_OP_LT) result = lhs < rhs;
            else result = lhs > rhs;
            return push(vm, result) ? SLVM_OK : SLVM_STACK_OVERFLOW;
        }

        case SLVM_OP_JUMP: {
            int32_t offset;
            if (!read_i32(vm, vm->pc, &offset)) return SLVM_ERROR;
            vm->pc += sizeof(int32_t);
            if (offset < 0 && (uint64_t)(-offset) > vm->pc) return SLVM_ERROR;
            if (offset > 0 && (uint64_t)offset > vm->code_size - vm->pc) return SLVM_ERROR;
            vm->pc = (slvm_pc_t)((int64_t)vm->pc + offset);
            return SLVM_OK;
        }

        case SLVM_OP_JUMP_IF_FALSE: {
            int32_t offset;
            slvm_word_t condition;
            if (!read_i32(vm, vm->pc, &offset)) return SLVM_ERROR;
            vm->pc += sizeof(int32_t);
            if (!pop(vm, &condition)) return SLVM_STACK_UNDERFLOW;
            if (!condition) {
                if (offset < 0 && (uint64_t)(-offset) > vm->pc) return SLVM_ERROR;
                if (offset > 0 && (uint64_t)offset > vm->code_size - vm->pc) return SLVM_ERROR;
                vm->pc = (slvm_pc_t)((int64_t)vm->pc + offset);
            }
            return SLVM_OK;
        }

        case SLVM_OP_RETURN:
            vm->halted = 1;
            return SLVM_HALTED;

        default:
            return SLVM_INVALID_OPCODE;
    }
}

slvm_status_t slvm_run(slvm_t *vm) {
    if (!vm) return SLVM_ERROR;
    for (;;) {
        const slvm_status_t status = slvm_step(vm);
        if (status != SLVM_OK) return status;
    }
}
