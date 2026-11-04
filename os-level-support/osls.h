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

typedef struct osls_environment_entry { char name[128]; char value[1024]; } osls_environment_entry;
typedef struct osls_cpu_info { char vendor[64]; char model[128]; uint32_t logical_cores; uint32_t physical_cores; uint32_t cache_line_size; uint64_t total_memory_bytes; } osls_cpu_info;
typedef struct osls_file_info { char path[OSLS_PATH_MAX]; uint64_t size_bytes; int64_t modified_epoch_seconds; int64_t created_epoch_seconds; int is_directory; int is_regular_file; int is_readable; int is_writable; int is_executable; } osls_file_info;
typedef struct osls_network_interface { char name[64]; char mac_address[32]; char ipv4[64]; char ipv6[64]; int is_up; int is_loopback; } osls_network_interface;
typedef struct osls_timezone_info { char id[128]; char abbreviation[32]; int offset_minutes_from_utc; int is_dst; } osls_timezone_info;

OSLS_API osls_status osls_get_system_info(osls_system_info *out_info);
/* Process inspection is limited to the caller's own process (pid 0 means self). */
OSLS_API osls_status osls_get_process_info(uint64_t pid, osls_process_info *out_info);
OSLS_API osls_status osls_enumerate_processes(osls_process_info *buffer, size_t buffer_count, size_t *out_count);
OSLS_API osls_status osls_get_environment_value(const char *name, char *value_buffer, size_t value_capacity);
OSLS_API osls_status osls_enumerate_environment(osls_environment_entry *buffer, size_t buffer_count, size_t *out_count);
OSLS_API osls_status osls_get_file_info(const char *path, osls_file_info *out_info);
/* Packed NUL-terminated names; out_written is bytes used. */
OSLS_API osls_status osls_list_directory(const char *path, char *entries, size_t entries_capacity, size_t *out_written);
OSLS_API osls_status osls_get_cpu_info(osls_cpu_info *out_info);
OSLS_API osls_status osls_get_timezone_info(osls_timezone_info *out_info);
OSLS_API osls_status osls_get_network_interfaces(osls_network_interface *buffer, size_t buffer_count, size_t *out_count);
OSLS_API osls_status osls_cron_validate(const char *expression);
OSLS_API osls_status osls_cron_next_after(const char *expression, int64_t after_epoch_seconds, int64_t *out_epoch_seconds);

#ifdef __cplusplus
}
#endif
#endif
