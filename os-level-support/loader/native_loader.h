#ifndef SLEELA_NATIVE_LOADER_H
#define SLEELA_NATIVE_LOADER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SL_NATIVE_ABI_VERSION 1u
#define SL_NATIVE_TYPED_ABI_VERSION 1u
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

/* Stable tagged scalar/string value contract. Scalar fields use fixed-width
 * integer types; F64 uses the platform's 8-byte C double. String pointers are
 * borrowed UTF-8 byte spans, never NUL-terminated by contract, and may contain
 * embedded NUL bytes. Composite VM objects and raw VM handles are deliberately
 * not representable in this first ABI version. */
typedef enum SLNativeValueTag {
    SL_NATIVE_VALUE_NULL = 0,
    SL_NATIVE_VALUE_BOOL = 1,
    SL_NATIVE_VALUE_I64 = 2,
    SL_NATIVE_VALUE_F64 = 3,
    SL_NATIVE_VALUE_UTF8 = 4
} SLNativeValueTag;

typedef struct SLNativeValue {
    uint32_t tag;
    uint32_t reserved;
    union {
        int32_t boolean;
        int64_t i64;
        double f64;
        struct {
            const uint8_t *data;
            uint32_t length;
        } utf8;
    } as;
} SLNativeValue;

/* Optional typed method export. Return 0 for success and nonzero for a
 * plugin-defined failure. The result is initialized to NULL by the loader.
 * Any returned UTF-8 bytes must remain valid until the next call to this
 * module or module close; the host copies them before making another call. */
typedef int (*SLNativeTypedCall)(const char *method,
                                 uint32_t argc,
                                 const SLNativeValue *argv,
                                 SLNativeValue *out_result);

/* Keep the public module layout stable: the optional typed symbol is resolved
 * from handle at call time rather than stored in this struct. */
typedef struct SLNativeModule {
    void *handle;
    SLNativeEntry run;
} SLNativeModule;

typedef enum SLNativeStatus {
    SL_NATIVE_OK = 0,
    SL_NATIVE_INVALID_ARGUMENT = 1,
    SL_NATIVE_LOAD_ERROR = 2,
    SL_NATIVE_ABI_MISMATCH = 3,
    SL_NATIVE_ENTRY_NOT_FOUND = 4,
    SL_NATIVE_AUTHORIZATION_DENIED = 5,
    SL_NATIVE_TABLE_FULL = 6,
    SL_NATIVE_STALE_HANDLE = 7,
    SL_NATIVE_TYPED_ENTRY_NOT_FOUND = 8,
    SL_NATIVE_PLUGIN_ERROR = 9
} SLNativeStatus;

/* Plugins export these C-linkage functions; initialize module as {0}. */
SL_NATIVE_EXPORT uint32_t sl_native_abi_version(void);
SL_NATIVE_EXPORT int sl_native_run(int argc, const char *const argv[]);
/* Optional typed entry: int sl_native_call(const char*, uint32_t,
 * const SLNativeValue*, SLNativeValue*). */
SL_NATIVE_EXPORT int sl_native_call(const char *method, uint32_t argc,
                                    const SLNativeValue *argv,
                                    SLNativeValue *out_result);

/* Opens a trusted shared library (DLL, .dylib, or .so); loading executes native code. */
SLNativeStatus sl_native_open(SLNativeModule *module, const char *path);
SLNativeStatus sl_native_run_module(SLNativeModule *module, int argc,
                                    const char *const argv[], int *out_result);
SLNativeStatus sl_native_call_module(SLNativeModule *module, const char *method,
                                     uint32_t argc, const SLNativeValue *argv,
                                     SLNativeValue *out_result);
SLNativeStatus sl_native_close(SLNativeModule *module);
const char *sl_native_status_string(SLNativeStatus status);
const char *sl_native_loader_error(void);

#ifdef __cplusplus
}
#endif
#endif
