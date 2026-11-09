#include "../native_loader.h"
#include <assert.h>
#include <stddef.h>

int main(int argc, char **argv) {
    SLNativeModule module = {0};
    assert(argc == 3);
    assert(sl_native_open(NULL, argv[1]) == SL_NATIVE_INVALID_ARGUMENT);
    assert(sl_native_open(&module, "") == SL_NATIVE_INVALID_ARGUMENT);

    assert(sl_native_open(&module, argv[1]) == SL_NATIVE_ABI_MISMATCH);
    assert(module.handle == NULL && module.run == NULL);

    assert(sl_native_open(&module, argv[2]) == SL_NATIVE_ENTRY_NOT_FOUND);
    assert(module.handle == NULL && module.run == NULL);

    assert(sl_native_close(&module) == SL_NATIVE_OK);
    return 0;
}
