#ifndef OS_LEVEL_SUPPORT_OSLS_H
#define OS_LEVEL_SUPPORT_OSLS_H

#include <stddef.h>
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

#define OSLS_NAME_MAX 256
#define OSLS_PATH_MAX 4096
#define OSLS_COMMAND_MAX 2048

typedef enum osls_status {
    OSLS_OK = 0,
    OSLS_INVALID_ARGUMENT = 1,
    OSLS_BUFFER_TOO_SMALL = 2,
    OSLS_SYSTEM_ERROR = 3,
    OSLS_NOT_FOUND = 4,
    OSLS_UNSUPPORTED = 5
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
    char hostname[OSLS_NAME_MAX];
    char user_name[OSLS_NAME_MAX];
    char working_directory[1024];
} osls_system_info;

typedef struct osls_process_info {
    uint64_t pid;
    uint64_t parent_pid;
    uint64_t thread_count;
    int64_t start_epoch_seconds;
    char executable_path[OSLS_PATH_MAX];
    char command_line[OSLS_COMMAND_MAX];
    char current_directory[OSLS_PATH_MAX];
    char state[32];
    char user_name[OSLS_NAME_MAX];
} osls_process_info;

typedef struct osls_environment_entry {
    char name[128];
    char value[1024];
} osls_environment_entry;

typedef struct osls_cpu_info {
    char vendor[64];
    char model[128];
    uint32_t logical_cores;
    uint32_t physical_cores;
    uint32_t cache_line_size;
    uint64_t total_memory_bytes;
} osls_cpu_info;

typedef struct osls_file_info {
    char path[OSLS_PATH_MAX];
    uint64_t size_bytes;
    int64_t modified_epoch_seconds;
    int64_t created_epoch_seconds;
    int is_directory;
    int is_regular_file;
    int is_readable;
    int is_writable;
    int is_executable;
} osls_file_info;

typedef struct osls_network_interface {
    char name[64];
    char mac_address[32];
    char ipv4[64];
    char ipv6[64];
    int is_up;
    int is_loopback;
} osls_network_interface;

typedef struct osls_timezone_info {
    char id[128];
    char abbreviation[32];
    int offset_minutes_from_utc;
    int is_dst;
} osls_timezone_info;

typedef struct osls_memory_info {
    uint64_t total_bytes;
    uint64_t available_bytes;
    uint64_t used_bytes;
    uint64_t page_size_bytes;
} osls_memory_info;

typedef struct osls_disk_space {
    char path[OSLS_PATH_MAX];
    uint64_t total_bytes;
    uint64_t free_bytes;
    uint64_t available_bytes; /* free space usable by the caller */
} osls_disk_space;

typedef struct osls_volume_info {
    char path[OSLS_PATH_MAX];
    char filesystem[64];
    uint64_t total_bytes;
    uint64_t available_bytes;
    uint64_t block_size;
    int is_read_only;
} osls_volume_info;

typedef struct osls_system_load {
    /* Load averages scaled by 1000 (e.g. 1500 == 1.50). A value of -1 means the
     * platform does not expose that window. */
    int64_t load_average_1min_milli;
    int64_t load_average_5min_milli;
    int64_t load_average_15min_milli;
    uint32_t logical_cores;
} osls_system_load;

typedef struct osls_runtime_info {
    char backend[64];        /* "linux" | "windows" | "macos" | "unknown" */
    char abi_version[32];    /* OSLS ABI version string */
    uint32_t operation_count; /* registered VM operation count */
    uint64_t capabilities;    /* capability bits this backend can satisfy */
} osls_runtime_info;

typedef struct osls_native_error {
    int32_t code;            /* native errno / GetLastError value, 0 if none */
    char message[256];       /* human-readable native message, or "" */
} osls_native_error;

typedef struct osls_path_info {
    char input[OSLS_PATH_MAX];
    char normalized[OSLS_PATH_MAX];
    char root[64];           /* "/" on POSIX, e.g. "C:\\" on Windows */
    int is_absolute;
    int exists;              /* best-effort existence check */
} osls_path_info;

/* Read-only memory, storage and load inspection. */
OSLS_API osls_status osls_get_memory_info(osls_memory_info *out_info);
OSLS_API osls_status osls_get_available_memory(uint64_t *out_bytes);
OSLS_API osls_status osls_get_disk_space(const char *path, osls_disk_space *out_info);
OSLS_API osls_status osls_get_volume_info(const char *path, osls_volume_info *out_info);
OSLS_API osls_status osls_get_system_load(osls_system_load *out_info);

/* Backend capability and diagnostic discovery. */
OSLS_API osls_status osls_get_capabilities(uint64_t *out_capabilities);
OSLS_API osls_status osls_get_runtime_info(osls_runtime_info *out_info);
OSLS_API osls_status osls_get_native_error(osls_native_error *out_error);

/* Path handling. normalize/join are pure string computation; resolve and
 * path-info consult the filesystem read-only. */
OSLS_API osls_status osls_path_normalize(const char *path, char *out_buffer, size_t out_capacity);
OSLS_API osls_status osls_path_join(const char *left, const char *right, char *out_buffer, size_t out_capacity);
OSLS_API osls_status osls_path_resolve(const char *path, char *out_buffer, size_t out_capacity);
OSLS_API osls_status osls_get_path_info(const char *path, osls_path_info *out_info);

/* Read-only hostname/service resolution (DNS lookup; no connection is made). */
OSLS_API osls_status osls_network_resolve(const char *host, char *out_buffer, size_t out_capacity);

/* Read-only information about the current host and process. */
OSLS_API osls_status osls_get_system_info(osls_system_info *out_info);
/* Process inspection is limited to the caller's own process (pid 0 means self).
 * Enumeration likewise returns the caller's process only; no system-wide
 * process discovery or cross-process inspection is performed. */
OSLS_API osls_status osls_get_process_info(uint64_t pid, osls_process_info *out_info);
OSLS_API osls_status osls_enumerate_processes(osls_process_info *buffer, size_t buffer_count, size_t *out_count);
OSLS_API osls_status osls_get_environment_value(const char *name, char *value_buffer, size_t value_capacity);
OSLS_API osls_status osls_enumerate_environment(osls_environment_entry *buffer, size_t buffer_count, size_t *out_count);
OSLS_API osls_status osls_get_file_info(const char *path, osls_file_info *out_info);
/* Directory names are returned as NUL-terminated strings packed consecutively.
 * `out_written` receives bytes used; a too-small buffer returns BUFFER_TOO_SMALL. */
OSLS_API osls_status osls_list_directory(const char *path, char *entries, size_t entries_capacity, size_t *out_written);
OSLS_API osls_status osls_get_cpu_info(osls_cpu_info *out_info);
OSLS_API osls_status osls_get_timezone_info(osls_timezone_info *out_info);
OSLS_API osls_status osls_get_network_interfaces(osls_network_interface *buffer, size_t buffer_count, size_t *out_count);

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
