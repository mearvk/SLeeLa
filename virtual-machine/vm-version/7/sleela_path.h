#ifndef SLEELA_PATH_H
#define SLEELA_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef enum {
    SL_PATH_AUTO = 0,
    SL_PATH_LINUX = 1,
    SL_PATH_WINDOWS = 2,
    SL_PATH_MACOS = 3
} SLPathPlatform;

SLPathPlatform slpath_platform(void);
const char* slpath_platform_name(void);
int slpath_platform_is_available(SLPathPlatform platform);
char slpath_separator(void);

/* Platform-neutral path operations. */
int slpath_join(char* out, size_t out_size, const char* left, const char* right);
int slpath_normalize(char* out, size_t out_size, const char* path);
int slpath_absolute(char* out, size_t out_size, const char* path);
int slpath_current_directory(char* out, size_t out_size);

/* Basic filesystem predicates and mutations. */
int slpath_exists(const char* path);
int slpath_is_file(const char* path);
int slpath_is_directory(const char* path);
int slpath_make_directory(const char* path);
int slpath_remove(const char* path);
int slpath_rename(const char* old_path, const char* new_path);

#ifdef __cplusplus
}
#endif

#endif
