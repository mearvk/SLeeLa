#ifndef SLEELA_LIBRARY_H
#define SLEELA_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef intptr_t SLLibraryHandle;
typedef void* SLLibrarySymbol;

typedef enum {
    SL_LIBRARY_AUTO = 0,
    SL_LIBRARY_LINUX = 1,
    SL_LIBRARY_WINDOWS = 2
} SLLibraryPlatform;

SLLibraryPlatform sllibrary_platform(void);
const char* sllibrary_platform_name(void);
int sllibrary_platform_is_available(SLLibraryPlatform platform);

int sllibrary_open(SLLibraryHandle* library, const char* path);
SLLibrarySymbol sllibrary_symbol(SLLibraryHandle library, const char* name);
int sllibrary_close(SLLibraryHandle library);
int sllibrary_valid(SLLibraryHandle library);
const char* sllibrary_last_error(void);

#ifdef __cplusplus
}
#endif

#endif
