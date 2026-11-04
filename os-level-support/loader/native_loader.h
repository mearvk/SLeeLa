#ifndef SLEELA_NATIVE_LOADER_H
#define SLEELA_NATIVE_LOADER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SL_NATIVE_ABI_VERSION 1u
#define SL_NATIVE_MAX_UNITS 9u

#if defined(_WIN32) && defined(SL_NATIVE_PLUGIN_BUILD)
#  define SL_NATIVE_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#  define SL_NATIVE_EXPORT __attribute__((visibility("default")))
#else
#  define SL_NATIVE_EXPORT
#endif

typedef int (*SLNativeEntry)(int argc, const char *const argv[]);
typedef uint32_t (*SLNativeAbiVersion)(void);

typedef struct SLNativeModule {
    void *handle;
    SLNativeEntry run;
} SLNativeModule;

typedef enum SLNativeStatus {
    SL_NATIVE_OK = 0,
    SL_NATIVE_INVALID_ARGUMENT = 1,
    SL_NATIVE_LOAD_ERROR = 2,
    SL_NATIVE_ABI_MISMATCH = 3,
    SL_NATIVE_ENTRY_NOT_FOUND = 4
} SLNativeStatus;

/* Plugins export these C-linkage functions; initialize module as {0}. */
SL_NATIVE_EXPORT uint32_t sl_native_abi_version(void);
SL_NATIVE_EXPORT int sl_native_run(int argc, const char *const argv[]);

/* Opens a trusted shared library (DLL, .dylib, or .so); loading executes native code. */
SLNativeStatus sl_native_open(SLNativeModule *module, const char *path);
SLNativeStatus sl_native_run_module(SLNativeModule *module, int argc,
                                    const char *const argv[], int *out_result);
SLNativeStatus sl_native_close(SLNativeModule *module);
const char *sl_native_status_string(SLNativeStatus status);
const char *sl_native_loader_error(void);

#ifdef __cplusplus
}
#endif
#endif
