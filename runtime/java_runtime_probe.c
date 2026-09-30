/*
 * SLeeLa Java Runtime Probe
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Dry layer only: no JVM launch, installation, or VM process-model change.
 */
#include "java_runtime_probe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <io.h>
#define SLEELA_ACCESS _access
#define SLEELA_X_OK 0
#define SLEELA_PATH_SEP '\\'
#else
#include <unistd.h>
#define SLEELA_ACCESS access
#define SLEELA_X_OK X_OK
#define SLEELA_PATH_SEP '/'
#endif

static void reset_result(SleelaJavaRuntimeProbeResult *r) {
    memset(r, 0, sizeof(*r));
    r->kind = SLEELA_JAVA_NONE;
    r->action = SLEELA_JAVA_ACTION_CONTINUE;
}

static int contains(const char *s, size_t n, const char *needle) {
    size_t m = strlen(needle);
    if (!s || !needle || m == 0 || n < m) return 0;
    for (size_t i = 0; i + m <= n; ++i)
        if (memcmp(s + i, needle, m) == 0) return 1;
    return 0;
}

static void copy_string(char *dst, size_t cap, const char *src) {
    if (!dst || cap == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    snprintf(dst, cap, "%s", src);
}

static int executable_file(const char *path) {
    return path && path[0] != '\0' && SLEELA_ACCESS(path, SLEELA_X_OK) == 0;
}

static int probe_path_command(const char *command, char *out, size_t out_cap) {
    const char *path = getenv("PATH");
    if (!path || !command || !out || out_cap == 0) return 0;

    char *copy = (char *)malloc(strlen(path) + 1);
    if (!copy) return 0;
    strcpy(copy, path);

    char *cursor = copy;
    while (cursor) {
        char *next = strchr(cursor, ':');
#if defined(_WIN32)
        if (!next) next = strchr(cursor, ';');
#endif
        if (next) *next = '\0';

        if (cursor[0] != '\0') {
            char candidate[1024];
            snprintf(candidate, sizeof(candidate), "%s%c%s",
                     cursor, SLEELA_PATH_SEP, command);
            if (executable_file(candidate)) {
                copy_string(out, out_cap, candidate);
                free(copy);
                return 1;
            }
        }
        if (!next) break;
        cursor = next + 1;
    }

    free(copy);
    return 0;
}

int sleela_java_runtime_probe_local_vm(SleelaJavaRuntimeProbeResult *result) {
    if (!result) return -1;
    reset_result(result);

    const char *configured = getenv("SLEELA_JAVA");
    if (configured && executable_file(configured)) {
        copy_string(result->java_executable, sizeof(result->java_executable), configured);
        result->java_vm_available = 1;
        result->action = SLEELA_JAVA_ACTION_LOCAL_VM;
        return 1;
    }

    const char *home = getenv("JAVA_HOME");
    if (home && home[0]) {
        char candidate[1024];
#if defined(_WIN32)
        snprintf(candidate, sizeof(candidate), "%s\\bin\\java.exe", home);
#else
        snprintf(candidate, sizeof(candidate), "%s/bin/java", home);
#endif
        if (executable_file(candidate)) {
            copy_string(result->java_executable, sizeof(result->java_executable), candidate);
            result->java_vm_available = 1;
            result->action = SLEELA_JAVA_ACTION_LOCAL_VM;
            return 1;
        }
    }

    char path_java[1024];
    if (probe_path_command("java", path_java, sizeof(path_java))) {
        copy_string(result->java_executable, sizeof(result->java_executable), path_java);
        result->java_vm_available = 1;
        result->action = SLEELA_JAVA_ACTION_LOCAL_VM;
        return 1;
    }

    result->action = SLEELA_JAVA_ACTION_PROMPT_INSTALL;
    return 0;
}

int sleela_java_runtime_probe_source(
    const char *source, size_t length, const char *path_hint,
    SleelaJavaRuntimeProbeResult *result) {

    if (!source || !result) return -1;
    reset_result(result);

    int java = 0, awt = 0, swing = 0, fx = 0;

    if (contains(source, length, "import java.") ||
        contains(source, length, "package java.") ||
        contains(source, length, "javaType = \"java.") ||
        contains(source, length, "java.construct:") ||
        contains(source, length, "java.invoke:") ||
        contains(source, length, "java.static:"))
        java = 1;

    if (contains(source, length, "java.awt.") ||
        contains(source, length, "import java.awt"))
        awt = java = 1;

    if (contains(source, length, "javax.swing.") ||
        contains(source, length, "import javax.swing"))
        swing = java = 1;

    if (contains(source, length, "javafx.") ||
        contains(source, length, "import javafx"))
        fx = java = 1;

    if (path_hint) {
        size_t n = strlen(path_hint);
        if (n >= 5 && strcmp(path_hint + n - 5, ".java") == 0) java = 1;
    }

    result->java_signals = java;
    result->framework_signals = awt + swing + fx;

    if (fx) result->kind = SLEELA_JAVA_FX;
    else if (swing) result->kind = SLEELA_JAVA_SWING;
    else if (awt) result->kind = SLEELA_JAVA_AWT;
    else if (java) result->kind = SLEELA_JAVA_SE;

    if (java) {
        SleelaJavaRuntimeProbeResult local;
        sleela_java_runtime_probe_local_vm(&local);
        result->java_vm_available = local.java_vm_available;
        copy_string(result->java_executable, sizeof(result->java_executable),
                    local.java_executable);
        result->action = local.java_vm_available
            ? SLEELA_JAVA_ACTION_LOCAL_VM
            : SLEELA_JAVA_ACTION_PROMPT_INSTALL;
    }

    return 0;
}

const char *sleela_java_runtime_action_name(SleelaJavaRuntimeAction action) {
    switch (action) {
        case SLEELA_JAVA_ACTION_LOCAL_VM: return "local-java-vm";
        case SLEELA_JAVA_ACTION_PROMPT_INSTALL: return "prompt-install-java-vm";
        default: return "continue-sleela-vm";
    }
}
