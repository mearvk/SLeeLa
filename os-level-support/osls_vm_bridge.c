#define _POSIX_C_SOURCE 200809L
#include "osls_vm_bridge.h"
#include "osls.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct { char *p; size_t cap, used; int failed; } json_out;
static void raw(json_out *j, const char *s) {
    size_t n = s ? strlen(s) : 0;
    if (j->failed || n >= j->cap - j->used) { j->failed = 1; return; }
    memcpy(j->p + j->used, s, n); j->used += n; j->p[j->used] = 0;
}
static void fmt(json_out *j, const char *f, ...) {
    va_list ap; int n;
    if (j->failed || j->used >= j->cap) { j->failed = 1; return; }
    va_start(ap, f); n = vsnprintf(j->p + j->used, j->cap - j->used, f, ap); va_end(ap);
    if (n < 0 || (size_t)n >= j->cap - j->used) { j->failed = 1; return; }
    j->used += (size_t)n;
}
static void str(json_out *j, const char *s) {
    const unsigned char *p = (const unsigned char *)(s ? s : "");
    raw(j, "\"");
    while (*p && !j->failed) {
        char b[7];
        switch (*p) {
            case '"': raw(j, "\\\""); break;
            case '\\': raw(j, "\\\\"); break;
            case '\n': raw(j, "\\n"); break;
            case '\r': raw(j, "\\r"); break;
            case '\t': raw(j, "\\t"); break;
            default:
                if (*p < 0x20) { snprintf(b, sizeof(b), "\\u%04x", (unsigned)*p); raw(j, b); }
                else { b[0] = (char)*p; b[1] = 0; raw(j, b); }
        }
        ++p;
    }
    raw(j, "\"");
}
static void status(json_out *j, osls_status s) { fmt(j, "{\"status\":%d", (int)s); }
static void field_string(json_out *j, const char *key, const char *value) {
    raw(j, ","); str(j, key); raw(j, ":"); str(j, value);
}
static void field_u64(json_out *j, const char *key, uint64_t value) {
    raw(j, ","); str(j, key); fmt(j, ":%llu", (unsigned long long)value);
}
static void field_i64(json_out *j, const char *key, int64_t value) {
    raw(j, ","); str(j, key); fmt(j, ":%lld", (long long)value);
}
static void field_bool(json_out *j, const char *key, int value) {
    raw(j, ","); str(j, key); raw(j, value ? ":true" : ":false");
}
static void finish(json_out *j) { raw(j, "}"); }

static const osls_vm_binding bindings[] = {
    {0,"oslsSystemInfo","osls_get_system_info",0,"()","JSON system record"},
    {1,"oslsProcessInfo","osls_get_process_info",1,"(pid:int64; 0=self)","JSON process record"},
    {2,"oslsEnumerateProcesses","osls_enumerate_processes",0,"()","JSON process array; self only"},
    {3,"oslsGetEnvironmentValue","osls_get_environment_value",1,"(name:utf8)","JSON status/value"},
    {4,"oslsEnumerateEnvironment","osls_enumerate_environment",0,"()","JSON environment array"},
    {5,"oslsGetFileInfo","osls_get_file_info",1,"(path:utf8)","JSON file record"},
    {6,"oslsListDirectory","osls_list_directory",1,"(path:utf8)","JSON entry array"},
    {7,"oslsGetCPUInfo","osls_get_cpu_info",0,"()","JSON CPU record"},
    {8,"oslsGetTimezoneInfo","osls_get_timezone_info",0,"()","JSON timezone record"},
    {9,"oslsGetNetworkInterfaces","osls_get_network_interfaces",0,"()","JSON interface array"},
    {10,"oslsCronValidate","osls_cron_validate",1,"(expression:utf8)","JSON status"},
    {11,"oslsCronNextAfter","osls_cron_next_after",2,"(expression:utf8, epoch:int64)","JSON status/epoch"}
};
const osls_vm_binding *osls_vm_binding_table(size_t *out_count) {
    if (out_count) *out_count = sizeof(bindings) / sizeof(bindings[0]);
    return bindings;
}

int osls_vm_call(int op, const char *arg, int64_t number, char *out, size_t cap) {
    json_out j;
    osls_status rc = OSLS_UNSUPPORTED;
    if (!out || cap < 32) return -1;
    out[0] = 0; j.p = out; j.cap = cap; j.used = 0; j.failed = 0;
    if (op < 0 || op >= OSLS_VM_OPERATION_COUNT) return -1;
    switch (op) {
    case OSLS_VM_SYSTEM_INFO: {
        osls_system_info v; rc = osls_get_system_info(&v); status(&j, rc);
        if (rc == OSLS_OK) {
            field_i64(&j,"platform",v.platform); field_u64(&j,"processId",v.process_id);
            field_i64(&j,"unixTimeSeconds",v.unix_time_seconds);
            field_u64(&j,"monotonicTimeNanoseconds",v.monotonic_time_nanoseconds);
            field_string(&j,"hostname",v.hostname); field_string(&j,"userName",v.user_name);
            field_string(&j,"workingDirectory",v.working_directory);
        }
        finish(&j); break;
    }
    case OSLS_VM_PROCESS_INFO: {
        osls_process_info v; rc = osls_get_process_info(number < 0 ? UINT64_MAX : (uint64_t)number, &v);
        status(&j, rc);
        if (rc == OSLS_OK) {
            field_u64(&j,"pid",v.pid); field_u64(&j,"parentPid",v.parent_pid);
            field_u64(&j,"threadCount",v.thread_count); field_i64(&j,"startEpochSeconds",v.start_epoch_seconds);
            field_string(&j,"executablePath",v.executable_path); field_string(&j,"commandLine",v.command_line);
            field_string(&j,"currentDirectory",v.current_directory); field_string(&j,"state",v.state);
            field_string(&j,"userName",v.user_name);
        }
        finish(&j); break;
    }
    case OSLS_VM_ENUMERATE_PROCESSES: {
        osls_process_info v[1]; size_t n = 0; rc = osls_enumerate_processes(v, 1, &n);
        status(&j, rc); fmt(&j, ",\"count\":%llu,\"items\":[", (unsigned long long)n);
        if (rc == OSLS_OK && n) {
            fmt(&j, "{\"pid\":%llu,\"parentPid\":%llu,\"threadCount\":%llu,",
                (unsigned long long)v[0].pid,(unsigned long long)v[0].parent_pid,(unsigned long long)v[0].thread_count);
            raw(&j,"\"executablePath\":"); str(&j,v[0].executable_path);
            raw(&j,",\"commandLine\":"); str(&j,v[0].command_line);
            raw(&j,",\"currentDirectory\":"); str(&j,v[0].current_directory);
            raw(&j,",\"state\":"); str(&j,v[0].state); raw(&j,"}");
        }
        raw(&j,"]"); finish(&j); break;
    }
    case OSLS_VM_GET_ENVIRONMENT_VALUE: {
        char value[1024] = {0}; rc = osls_get_environment_value(arg, value, sizeof(value));
        status(&j, rc); if (rc == OSLS_OK) field_string(&j,"value",value); finish(&j); break;
    }
    case OSLS_VM_ENUMERATE_ENVIRONMENT: {
        osls_environment_entry v[128]; size_t n = 0; rc = osls_enumerate_environment(v, 128, &n);
        status(&j, rc); fmt(&j,",\"count\":%llu,\"items\":[",(unsigned long long)n);
        if (rc == OSLS_OK) for (size_t i=0;i<n;i++) {
            if (i) raw(&j,",");
            raw(&j,"{\"name\":"); str(&j,v[i].name);
            raw(&j,",\"value\":"); str(&j,v[i].value); raw(&j,"}");
        }
        raw(&j,"]"); finish(&j); break;
    }
    case OSLS_VM_GET_FILE_INFO: {
        osls_file_info v; rc = osls_get_file_info(arg, &v); status(&j, rc);
        if (rc == OSLS_OK) {
            field_string(&j,"path",v.path); field_u64(&j,"sizeBytes",v.size_bytes);
            field_i64(&j,"modifiedEpochSeconds",v.modified_epoch_seconds);
            field_i64(&j,"createdEpochSeconds",v.created_epoch_seconds);
            field_bool(&j,"isDirectory",v.is_directory); field_bool(&j,"isRegularFile",v.is_regular_file);
            field_bool(&j,"isReadable",v.is_readable); field_bool(&j,"isWritable",v.is_writable);
            field_bool(&j,"isExecutable",v.is_executable);
        }
        finish(&j); break;
    }
    case OSLS_VM_LIST_DIRECTORY: {
        char names[32768]; size_t n = 0; rc = osls_list_directory(arg, names, sizeof(names), &n);
        status(&j, rc); raw(&j,",\"entries\":[");
        if (rc == OSLS_OK) {
            size_t pos = 0; int first = 1;
            while (pos < n) { size_t len = strlen(names + pos); if (!first) raw(&j,","); str(&j,names + pos); first = 0; pos += len + 1; }
        }
        raw(&j,"]"); finish(&j); break;
    }
    case OSLS_VM_GET_CPU_INFO: {
        osls_cpu_info v; rc = osls_get_cpu_info(&v); status(&j, rc);
        if (rc == OSLS_OK) {
            field_string(&j,"vendor",v.vendor); field_string(&j,"model",v.model);
            field_u64(&j,"logicalCores",v.logical_cores); field_u64(&j,"physicalCores",v.physical_cores);
            field_u64(&j,"cacheLineSize",v.cache_line_size); field_u64(&j,"totalMemoryBytes",v.total_memory_bytes);
        }
        finish(&j); break;
    }
    case OSLS_VM_GET_TIMEZONE_INFO: {
        osls_timezone_info v; rc = osls_get_timezone_info(&v); status(&j, rc);
        if (rc == OSLS_OK) {
            field_string(&j,"id",v.id); field_string(&j,"abbreviation",v.abbreviation);
            field_i64(&j,"offsetMinutesFromUtc",v.offset_minutes_from_utc); field_bool(&j,"isDst",v.is_dst);
        }
        finish(&j); break;
    }
    case OSLS_VM_GET_NETWORK_INTERFACES: {
        osls_network_interface v[64]; size_t n = 0; rc = osls_get_network_interfaces(v, 64, &n);
        status(&j, rc); fmt(&j,",\"count\":%llu,\"items\":[",(unsigned long long)n);
        if (rc == OSLS_OK) for (size_t i=0;i<n;i++) {
            if (i) raw(&j,",");
            raw(&j,"{\"name\":"); str(&j,v[i].name);
            raw(&j,",\"macAddress\":"); str(&j,v[i].mac_address);
            raw(&j,",\"ipv4\":"); str(&j,v[i].ipv4); raw(&j,",\"ipv6\":"); str(&j,v[i].ipv6);
            field_bool(&j,"isUp",v[i].is_up); field_bool(&j,"isLoopback",v[i].is_loopback); raw(&j,"}");
        }
        raw(&j,"]"); finish(&j); break;
    }
    case OSLS_VM_CRON_VALIDATE:
        rc = osls_cron_validate(arg); status(&j, rc); finish(&j); break;
    case OSLS_VM_CRON_NEXT_AFTER: {
        int64_t next = 0; rc = osls_cron_next_after(arg, number, &next); status(&j, rc);
        if (rc == OSLS_OK) field_i64(&j,"nextEpochSeconds",next); finish(&j); break;
    }
    default: return -1;
    }
    return j.failed ? -1 : (int)j.used;
}
