#ifndef OS_LEVEL_SUPPORT_OSLS_H
#define OS_LEVEL_SUPPORT_OSLS_H

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && defined(OSLS_SHARED)
#  if defined(OSLS_BUILD)
#    define OSLS_API __declspec(dllexport)
#  else
#    define OSLS_API __declspec(dllimport)
#  endif
#else
#  define OSLS_API
#endif

typedef enum osls_status {
    OSLS_OK = 0,
    OSLS_INVALID_ARGUMENT = 1,
    OSLS_BUFFER_TOO_SMALL = 2,
    OSLS_SYSTEM_ERROR = 3,
    OSLS_NOT_FOUND = 4
} osls_status;

typedef enum osls_platform {
    OSLS_PLATFORM_UNKNOWN = 0,
    OSLS_PLATFORM_LINUX = 1,
    OSLS_PLATFORM_MACOS = 2,
    OSLS_PLATFORM_WINDOWS = 3
} osls_platform;

typedef struct osls_system_info {
    osls_platform platform;
    uint64_t process_id;
    int64_t unix_time_seconds;
    uint64_t monotonic_time_nanoseconds;
    char hostname[256];
    char user_name[256];
    char working_directory[1024];
} osls_system_info;

/* Read-only information about the current host and process. */
OSLS_API osls_status osls_get_system_info(osls_system_info *out_info);

/* Validate a five-field cron expression in the process local timezone. */
OSLS_API osls_status osls_cron_validate(const char *expression);

/* Find the next matching local-time minute strictly after `after_epoch_seconds`.
 * Returns OSLS_NOT_FOUND if no match is found within five years. */
OSLS_API osls_status osls_cron_next_after(const char *expression,
                                          int64_t after_epoch_seconds,
                                          int64_t *out_epoch_seconds);

#ifdef __cplusplus
}
#endif

#endif
