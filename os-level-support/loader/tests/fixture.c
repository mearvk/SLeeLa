#include "../native_loader.h"

uint32_t sl_native_abi_version(void) {
    return SL_NATIVE_ABI_VERSION;
}

int sl_native_run(int argc, const char *const argv[]) {
    (void)argv;
    return argc;
}
