#ifndef SLVM9_NATIVE_H
#define SLVM9_NATIVE_H
#include "slvm9.h"
#include "slvm9_filesystem.h"
#include "slvm9_file.h"
typedef struct { slvm9_filesystem_t filesystem; uint64_t generation_token; uint8_t native_probe; uint8_t read_only; } slvm9_native_filesystem_t;
typedef struct { slvm9_file_t file; uint64_t native_identity; uint8_t native_probe; } slvm9_native_file_t;
int slvm9_native_probe_filesystem(const char *path, slvm9_native_filesystem_t *out);
int slvm9_native_probe_file(const char *path, slvm9_native_file_t *out);
int slvm9_native_validate_generation(const slvm9_native_filesystem_t *fs,const slvm9_filesystem_t *expected);
const char *slvm9_native_backend_name(void);
const char *slvm9_native_filesystem_name(void);
#endif
