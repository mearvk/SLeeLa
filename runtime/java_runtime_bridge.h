#ifndef SLEELA_JAVA_RUNTIME_BRIDGE_H
#define SLEELA_JAVA_RUNTIME_BRIDGE_H

#include <stddef.h>
#include "java_runtime_probe.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *java_executable;
    const char *classpath;
    const char *main_class;
    const char *program_arguments;
    const char *sample_input;
} SleelaJavaProgramRequest;

typedef struct {
    int ready;
    char command[4096];
    char sample_input[4096];
    char sample_output_hint[4096];
} SleelaJavaProgramPlan;

int sleela_java_runtime_bridge_prepare(
    const SleelaJavaProgramRequest *request,
    SleelaJavaProgramPlan *plan);

/*
 * Main dispatch boundary: probe first, then prepare the normal local-JVM
 * handoff when Java is required and a local VM is available.
 * Returns 0 for native/ready decisions, 1 when installation is required,
 * and -1 for invalid arguments.
 */
int sleela_java_runtime_dispatch_source(
    const char *source, size_t length, const char *path_hint,
    const SleelaJavaProgramRequest *request,
    SleelaJavaRuntimeProbeResult *probe,
    SleelaJavaProgramPlan *plan);

#ifdef __cplusplus
}
#endif

#endif
