#include "java_runtime_bridge.h"

#include <stdio.h>
#include <string.h>

static void copy_text(char *dst, size_t cap, const char *src) {
    if (!dst || cap == 0) return;
    if (!src) src = "";
    snprintf(dst, cap, "%s", src);
}

int sleela_java_runtime_bridge_prepare(
    const SleelaJavaProgramRequest *request,
    SleelaJavaProgramPlan *plan) {
    if (!request || !plan || !request->java_executable ||
        !request->java_executable[0] || !request->main_class ||
        !request->main_class[0]) return -1;

    memset(plan, 0, sizeof(*plan));

    const char *classpath = request->classpath && request->classpath[0]
        ? request->classpath : ".";
    const char *args = request->program_arguments
        ? request->program_arguments : "";

    /* Process creation and platform-specific argument escaping remain with
       the existing launcher/provider; this layer does not invoke a shell. */
    if (args[0]) {
        snprintf(plan->command, sizeof(plan->command),
                 "%s -cp %s %s %s",
                 request->java_executable, classpath,
                 request->main_class, args);
    } else {
        snprintf(plan->command, sizeof(plan->command),
                 "%s -cp %s %s",
                 request->java_executable, classpath,
                 request->main_class);
    }

    copy_text(plan->sample_input, sizeof(plan->sample_input),
              request->sample_input ? request->sample_input : "");
    /* This is a caller-provided expected/illustrative output, not a claimed
       execution result. The provider fills real output after JVM execution. */
    copy_text(plan->sample_output_hint, sizeof(plan->sample_output_hint), "");
    plan->ready = 1;
    return 0;
}


int sleela_java_runtime_dispatch_source(
    const char *source, size_t length, const char *path_hint,
    const SleelaJavaProgramRequest *request,
    SleelaJavaRuntimeProbeResult *probe,
    SleelaJavaProgramPlan *plan) {

    if (!probe || !plan) return -1;
    if (sleela_java_runtime_probe_source(source, length, path_hint, probe) != 0)
        return -1;

    memset(plan, 0, sizeof(*plan));

    if (probe->action == SLEELA_JAVA_ACTION_CONTINUE)
        return 0;

    if (probe->action == SLEELA_JAVA_ACTION_PROMPT_INSTALL)
        return 1;

    if (!request) return -1;

    SleelaJavaProgramRequest handoff = *request;
    if (!handoff.java_executable || !handoff.java_executable[0])
        handoff.java_executable = probe->java_executable;

    if (sleela_java_runtime_bridge_prepare(&handoff, plan) != 0)
        return -1;

    return 0;
}
