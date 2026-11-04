#include "osls.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <process.h>
#  include <direct.h>
#  include <Lmcons.h>
#else
#  include <sys/types.h>
#  include <unistd.h>
#  include <limits.h>
#  if defined(__APPLE__)
#    include <mach/mach_time.h>
#  else
#    include <time.h>
#  endif
#endif

typedef struct cron_field {
    uint64_t values;
    int minimum;
    int maximum;
    int wildcard;
} cron_field;

static int copy_text(char *dst, size_t capacity, const char *src) {
    size_t length;
    if (!dst || capacity == 0 || !src) return 0;
    length = strlen(src);
    if (length >= capacity) return 0;
    memcpy(dst, src, length + 1);
    return 1;
}

static void copy_env(char *dst, size_t capacity, const char *name) {
    const char *value = getenv(name);
    if (value) (void)copy_text(dst, capacity, value);
}

osls_status osls_get_system_info(osls_system_info *out_info) {
    if (!out_info) return OSLS_INVALID_ARGUMENT;
    memset(out_info, 0, sizeof(*out_info));
#if defined(_WIN32)
    {
        DWORD host_len = (DWORD)sizeof(out_info->hostname);
        DWORD user_len = (DWORD)sizeof(out_info->user_name);
        LARGE_INTEGER counter, frequency;
        char cwd[sizeof(out_info->working_directory)];
        if (!GetComputerNameA(out_info->hostname, &host_len)) return OSLS_SYSTEM_ERROR;
        if (!GetUserNameA(out_info->user_name, &user_len)) copy_env(out_info->user_name, sizeof(out_info->user_name), "USERNAME");
        if (!_getcwd(cwd, (int)sizeof(cwd)) || !copy_text(out_info->working_directory, sizeof(out_info->working_directory), cwd)) return OSLS_SYSTEM_ERROR;
        out_info->platform = OSLS_PLATFORM_WINDOWS;
        out_info->process_id = (uint64_t)_getpid();
        out_info->unix_time_seconds = (int64_t)time(NULL);
        if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&counter) || frequency.QuadPart <= 0) return OSLS_SYSTEM_ERROR;
        out_info->monotonic_time_nanoseconds = (uint64_t)((counter.QuadPart / frequency.QuadPart) * 1000000000ULL + ((counter.QuadPart % frequency.QuadPart) * 1000000000ULL) / frequency.QuadPart);
    }
#elif defined(__APPLE__) || defined(__linux__)
    {
        struct timespec ts;
        char cwd[sizeof(out_info->working_directory)];
        if (gethostname(out_info->hostname, sizeof(out_info->hostname) - 1) != 0) return OSLS_SYSTEM_ERROR;
        out_info->hostname[sizeof(out_info->hostname) - 1] = '\0';
        if (!getcwd(cwd, sizeof(cwd)) || !copy_text(out_info->working_directory, sizeof(out_info->working_directory), cwd)) return OSLS_SYSTEM_ERROR;
        copy_env(out_info->user_name, sizeof(out_info->user_name), "USER");
        if (!out_info->user_name[0]) copy_env(out_info->user_name, sizeof(out_info->user_name), "LOGNAME");
        out_info->platform =
#  if defined(__APPLE__)
            OSLS_PLATFORM_MACOS;
#  else
            OSLS_PLATFORM_LINUX;
#  endif
        out_info->process_id = (uint64_t)getpid();
        out_info->unix_time_seconds = (int64_t)time(NULL);
#  if defined(__APPLE__)
        {
            mach_timebase_info_data_t info;
            uint64_t ticks = mach_absolute_time();
            if (mach_timebase_info(&info) != KERN_SUCCESS || info.denom == 0) return OSLS_SYSTEM_ERROR;
            out_info->monotonic_time_nanoseconds = (ticks / info.denom) * info.numer + ((ticks % info.denom) * info.numer) / info.denom;
        }
#  else
        if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) return OSLS_SYSTEM_ERROR;
        out_info->monotonic_time_nanoseconds = (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
#  endif
    }
#else
    return OSLS_NOT_FOUND;
#endif
    return OSLS_OK;
}

static int parse_uint(const char **cursor, int *out) {
    const char *p = *cursor;
    int value = 0;
    if (!isdigit((unsigned char)*p)) return 0;
    while (isdigit((unsigned char)*p)) {
        int digit = *p - '0';
        if (value > (INT_MAX - digit) / 10) return 0;
        value = value * 10 + digit;
        ++p;
    }
    *cursor = p;
    *out = value;
    return 1;
}

static int parse_field(const char *text, int minimum, int maximum, int sunday_alias, cron_field *out) {
    const char *p = text;
    int is_wildcard = (text[0] == '*');
    uint64_t bits = 0;
    if (!text || !*text || !out) return 0;
    while (*p) {
        int start, end, step = 1, value;
        if (*p == '*') {
            start = minimum;
            end = maximum;
            ++p;
        } else {
            if (!parse_uint(&p, &start)) return 0;
            end = start;
            if (*p == '-') {
                ++p;
                if (!parse_uint(&p, &end)) return 0;
            }
        }
        if (*p == '/') {
            ++p;
            if (!parse_uint(&p, &step) || step <= 0) return 0;
        }
        if (start < minimum || end > maximum || start > end) return 0;
        for (value = start; value <= end; value += step) {
            int normalized = (sunday_alias && value == 7) ? 0 : value;
            bits |= (uint64_t)1 << normalized;
            if (end - value < step) break;
        }
        if (!*p) break;
        if (*p != ',') return 0;
        ++p;
        if (!*p) return 0;
    }
    if (!bits) return 0;
    out->values = bits;
    out->minimum = minimum;
    out->maximum = maximum;
    out->wildcard = is_wildcard;
    return 1;
}

static int parse_expression(const char *expression, cron_field fields[5]) {
    char buffer[256];
    char *cursor, *token;
    int index = 0;
    if (!expression) return 0;
    if (strlen(expression) >= sizeof(buffer)) return 0;
    memcpy(buffer, expression, strlen(expression) + 1);
    cursor = buffer;
    while ((token = strtok(cursor, " \t\r\n")) != NULL) {
        static const int mins[5] = {0, 0, 1, 1, 0};
        static const int maxs[5] = {59, 23, 31, 12, 7};
        cursor = NULL;
        if (index >= 5 || !parse_field(token, mins[index], maxs[index], index == 4, &fields[index])) return 0;
        if (index == 3 && fields[index].values & 1u) return 0;
        ++index;
    }
    return index == 5;
}

osls_status osls_cron_validate(const char *expression) {
    cron_field fields[5];
    return parse_expression(expression, fields) ? OSLS_OK : OSLS_INVALID_ARGUMENT;
}

static int local_time(time_t value, struct tm *out) {
#if defined(_WIN32)
    return localtime_s(out, &value) == 0;
#else
    return localtime_r(&value, out) != NULL;
#endif
}

static int field_has(const cron_field *field, int value) {
    return value >= 0 && value < 64 && ((field->values >> value) & 1u) != 0;
}

osls_status osls_cron_next_after(const char *expression, int64_t after_epoch_seconds, int64_t *out_epoch_seconds) {
    cron_field fields[5];
    int64_t candidate;
    const int64_t limit = (int64_t)5 * 366 * 24 * 60;
    int64_t i;
    if (!out_epoch_seconds || !parse_expression(expression, fields)) return OSLS_INVALID_ARGUMENT;
    candidate = after_epoch_seconds - (after_epoch_seconds % 60) + 60;
    if (after_epoch_seconds < 0 && after_epoch_seconds % 60) candidate = after_epoch_seconds - (after_epoch_seconds % 60);
    for (i = 0; i < limit; ++i, candidate += 60) {
        struct tm tm_value;
        int day_match, weekday_match;
        time_t instant = (time_t)candidate;
        if ((int64_t)instant != candidate || !local_time(instant, &tm_value)) return OSLS_SYSTEM_ERROR;
        if (!field_has(&fields[0], tm_value.tm_min) || !field_has(&fields[1], tm_value.tm_hour) || !field_has(&fields[3], tm_value.tm_mon + 1)) continue;
        day_match = field_has(&fields[2], tm_value.tm_mday);
        weekday_match = field_has(&fields[4], tm_value.tm_wday);
        if (fields[2].wildcard || fields[4].wildcard) {
            if (!day_match || !weekday_match) continue;
        } else if (!day_match && !weekday_match) continue;
        *out_epoch_seconds = candidate;
        return OSLS_OK;
    }
    return OSLS_NOT_FOUND;
}
