#include "../native_loader.h"

uint32_t sl_native_abi_version(void) {
    return SL_NATIVE_ABI_VERSION;
}

/* Deliberately omits sl_native_run to exercise fail-closed loading. */
