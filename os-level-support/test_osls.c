#include "osls.h"
#include <assert.h>
#include <string.h>

int main(void) {
    osls_system_info info;
    osls_process_info process;
    osls_file_info file;
    osls_cpu_info cpu;
    osls_timezone_info timezone;
    size_t count = 0, written = 0;
    char env[1024];
    char entries[8192];
    int64_t next = 0;
    assert(osls_get_system_info(&info) == OSLS_OK);
    assert(info.process_id != 0);
    assert(info.hostname[0] != '\0');
    assert(osls_get_process_info(0, &process) == OSLS_OK && process.pid != 0);
    assert(osls_enumerate_processes(&process, 1, &count) == OSLS_OK && count == 1);
    assert(osls_get_environment_value("PATH", env, sizeof(env)) == OSLS_OK);
    assert(osls_get_file_info(".", &file) == OSLS_OK && file.is_directory);
    assert(osls_list_directory(".", entries, sizeof(entries), &written) == OSLS_OK && written > 0);
    assert(osls_get_cpu_info(&cpu) == OSLS_OK && cpu.logical_cores > 0);
    assert(osls_get_timezone_info(&timezone) == OSLS_OK);
    assert(osls_cron_validate("*/15 9-17 * * 1-5") == OSLS_OK);
    assert(osls_cron_validate("60 * * * *") == OSLS_INVALID_ARGUMENT);
    assert(osls_cron_next_after("0 0 * * *", 1704067200, &next) == OSLS_OK);
    assert(next > 1704067200);
    return 0;
}
