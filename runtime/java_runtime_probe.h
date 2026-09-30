#ifndef SLEELA_JAVA_RUNTIME_PROBE_H
#define SLEELA_JAVA_RUNTIME_PROBE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLEELA_JAVA_NONE = 0,
    SLEELA_JAVA_SE = 1,
    SLEELA_JAVA_AWT = 2,
    SLEELA_JAVA_SWING = 3,
    SLEELA_JAVA_FX = 4
} SleelaJavaRuntimeKind;

typedef enum {
    SLEELA_JAVA_ACTION_CONTINUE = 0,
    SLEELA_JAVA_ACTION_LOCAL_VM = 1,
    SLEELA_JAVA_ACTION_PROMPT_INSTALL = 2
} SleelaJavaRuntimeAction;

typedef struct {
    SleelaJavaRuntimeKind kind;
    SleelaJavaRuntimeAction action;
    int java_signals;
    int framework_signals;
    int java_vm_available;
    char java_executable[1024];
} SleelaJavaRuntimeProbeResult;

int sleela_java_runtime_probe_source(
    const char *source, size_t length, const char *path_hint,
    SleelaJavaRuntimeProbeResult *result);

int sleela_java_runtime_probe_local_vm(SleelaJavaRuntimeProbeResult *result);

const char *sleela_java_runtime_action_name(SleelaJavaRuntimeAction action);

#ifdef __cplusplus
}
#endif

#endif
