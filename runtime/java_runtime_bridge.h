#ifndef SLEELA_JAVA_RUNTIME_BRIDGE_H
#define SLEELA_JAVA_RUNTIME_BRIDGE_H

#include <stddef.h>

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

#ifdef __cplusplus
}
#endif

#endif
