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

    /* Read-only inspection functions added in revision 1.1. */
    {
        osls_memory_info mem;
        osls_disk_space disk;
        osls_volume_info vol;
        osls_system_load load;
        osls_runtime_info rt;
        osls_native_error nerr;
        osls_path_info pinfo;
        uint64_t avail = 0, caps = 0;
        char pbuf[4096];
        assert(osls_get_memory_info(&mem) == OSLS_OK && mem.total_bytes > 0);
        assert(osls_get_available_memory(&avail) == OSLS_OK);
        assert(osls_get_disk_space(".", &disk) == OSLS_OK && disk.total_bytes > 0);
        assert(osls_get_volume_info(".", &vol) == OSLS_OK);
        assert(osls_get_system_load(&load) == OSLS_OK);
        assert(osls_get_capabilities(&caps) == OSLS_OK && caps != 0);
        assert(osls_get_runtime_info(&rt) == OSLS_OK && rt.operation_count >= 25 && rt.backend[0] != '\0');
        assert(osls_get_native_error(&nerr) == OSLS_OK);
        /* Pure path computation. */
        assert(osls_path_normalize("/a/b/../c/./d", pbuf, sizeof(pbuf)) == OSLS_OK);
#if defined(_WIN32)
        /* Normalized paths use the host platform's separator. */
        assert(strcmp(pbuf, "\\a\\c\\d") == 0);
#else
        assert(strcmp(pbuf, "/a/c/d") == 0);
#endif
        assert(osls_path_normalize("a/b/../../../c", pbuf, sizeof(pbuf)) == OSLS_OK);
#if defined(_WIN32)
        assert(strcmp(pbuf, "..\\c") == 0);
#else
        assert(strcmp(pbuf, "../c") == 0);
#endif
        assert(osls_path_join("/usr/local", "bin/x", pbuf, sizeof(pbuf)) == OSLS_OK);
#if defined(_WIN32)
        assert(strcmp(pbuf, "\\usr\\local\\bin\\x") == 0);
#else
        assert(strcmp(pbuf, "/usr/local/bin/x") == 0);
#endif
        assert(osls_path_join("/usr", "/abs", pbuf, sizeof(pbuf)) == OSLS_OK);
#if defined(_WIN32)
        assert(strcmp(pbuf, "\\abs") == 0);
#else
        assert(strcmp(pbuf, "/abs") == 0);
#endif
        assert(osls_path_resolve(".", pbuf, sizeof(pbuf)) == OSLS_OK && pbuf[0] != '\0');
        assert(osls_get_path_info(".", &pinfo) == OSLS_OK);
        assert(osls_get_memory_info(NULL) == OSLS_INVALID_ARGUMENT);
        assert(osls_get_disk_space(NULL, &disk) == OSLS_INVALID_ARGUMENT);
    }
    return 0;
}
