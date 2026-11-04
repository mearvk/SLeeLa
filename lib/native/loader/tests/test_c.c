#include "../native_loader.h"
#include <assert.h>

int main(int argc, char **argv) {
    SLNativeModule module = {0};
    const char *args[] = {"alpha", "beta"};
    int result = -1;
    assert(argc == 2);
    assert(sl_native_open(&module, argv[1]) == SL_NATIVE_OK);
    assert(sl_native_run_module(&module, 2, args, &result) == SL_NATIVE_OK);
    assert(result == 2);
    assert(sl_native_close(&module) == SL_NATIVE_OK);
    return 0;
}
