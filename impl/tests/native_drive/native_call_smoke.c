#include "../../core/sleela_core.h"
#include <stdio.h>
#include <string.h>

static int authorize_exact_path(const char *path, void *context) {
    const char *approved = (const char *)context;
    return path && approved && strcmp(path, approved) == 0;
}

int main(int argc, char **argv) {
    SLVM *vm;
    SLValue result;
    if (argc != 3) {
        fprintf(stderr, "usage: native_call_smoke <compiled-artifact> <trusted-plugin-path>\\n");
        return 2;
    }

    /* This test intentionally loads compiler-produced bytecode rather than
     * assembling opcodes in C. It exercises source -> artifact -> VM -> plugin. */
    vm = slvm_load_file(argv[1]);
    if (!vm) {
        fprintf(stderr, "could not load compiled Native Drive artifact: %s\\n", argv[1]);
        return 2;
    }
    if (slvm_set_native_authorizer(vm, authorize_exact_path, argv[2]) != 0) {
        fprintf(stderr, "could not install native authorization callback\\n");
        slvm_free(vm);
        return 2;
    }

    if (slvm_run(vm) != SLR_OK) {
        fprintf(stderr, "compiled Native Drive execution failed: %s\\n",
                slvm_error(vm) ? slvm_error(vm) : "unknown VM error");
        slvm_free(vm);
        return 1;
    }
    result = slvm_result(vm);
    slvm_free(vm);
    if (result.type != SL_INT || result.as.i != 63) {
        fprintf(stderr, "expected 63 from compiler-produced native dispatch + bytecode fallback\\n");
        return 1;
    }
    puts("compiled source -> artifact -> authorized native dispatch -> bytecode fallback: PASS");
    return 0;
}
