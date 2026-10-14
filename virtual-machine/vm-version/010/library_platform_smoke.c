#include "sleela_library.h"

#include <stdio.h>

int main(void) {
    SLLibraryHandle library = 0;
    SLLibrarySymbol symbol;
    const char* target;

#ifdef _WIN32
    target = "kernel32.dll";
    symbol = NULL;
#else
    target = "libm.so.6";
    symbol = NULL;
#endif

    if (sllibrary_platform() == SL_LIBRARY_WINDOWS) {
        if (sllibrary_open(&library, target) != 0) return 1;
        symbol = sllibrary_symbol(library, "GetTickCount64");
    } else {
        if (sllibrary_open(&library, target) != 0) return 1;
        symbol = sllibrary_symbol(library, "cos");
    }

    if (!sllibrary_valid(library) || !symbol) {
        (void)sllibrary_close(library);
        return 1;
    }
    if (sllibrary_close(library) != 0) return 1;

    printf("native library loading platform: %s\n", sllibrary_platform_name());
    printf("native library loading smoke: PASS\n");
    return 0;
}
