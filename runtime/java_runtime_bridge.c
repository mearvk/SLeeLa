#include "java_runtime_bridge.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void copy_text(char *dst, size_t cap, const char *src) {
    if (!dst || cap == 0) return;
    if (!src) src = "";
    snprintf(dst, cap, "%s", src);
}

/*
 * Reject any caller-controlled field that could turn the flat command line
 * into more than one command, or into extra arguments, once a downstream
 * launcher consumes it. The planned command is a single space-joined string;
 * even though this layer never calls a shell itself, the only safe contract
 * is to refuse input that WOULD inject if the string ever reached one.
 *
 * Disallowed in every field:
 *   - control characters, including NUL-adjacent bytes, CR, LF, and TAB
 *     (newline injection splits one line into two commands);
 *   - the shell metacharacters that chain, redirect, substitute, or quote:
 *     ; & | < > ` $ ( ) { } [ ] ! * ? ~ # \ ' " and the backslash.
 *
 * `allow_space` is true only for `program_arguments`, where interior spaces
 * are legitimate argument separators; it stays false for a single executable
 * path, classpath entry, or main-class name, so an embedded space there (a
 * classic argument-injection trick) is refused.
 *
 * Returns 1 if the field is safe to concatenate, 0 otherwise. NULL/empty is
 * treated as safe (the caller decides whether empty is allowed).
 */
static int field_is_safe(const char *s, int allow_space) {
    if (!s) return 1;
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
        unsigned char c = *p;
        if (c < 0x20 || c == 0x7f) return 0;          /* control chars */
        if (c == ' ') { if (!allow_space) return 0; continue; }
        switch (c) {
            case ';': case '&': case '|':
            case '<': case '>': case '`':
            case '$': case '(': case ')':
            case '{': case '}': case '[': case ']':
            case '!': case '*': case '?': case '~':
            case '#': case '\\': case '\'': case '"':
                return 0;
            default:
                break;
        }
    }
    return 1;
}

/* An executable must be an explicit path, not a bare name a crafted PATH
 * could redirect to an attacker binary. Require a path separator. */
static int executable_path_is_explicit(const char *exe) {
    if (!exe || !exe[0]) return 0;
    if (strchr(exe, '/')) return 1;
#if defined(_WIN32)
    if (strchr(exe, '\\')) return 1;
#endif
    return 0;
}

int sleela_java_runtime_bridge_prepare(
    const SleelaJavaProgramRequest *request,
    SleelaJavaProgramPlan *plan) {
    if (!request || !plan || !request->java_executable ||
        !request->java_executable[0] ||
        ((!request->main_class || !request->main_class[0]) &&
         (!request->source_file || !request->source_file[0]))) return -1;

    memset(plan, 0, sizeof(*plan));

    const char *classpath = request->classpath && request->classpath[0]
        ? request->classpath : ".";
    const char *args = request->program_arguments
        ? request->program_arguments : "";

    /* Fail closed on any field that could inject a second command or a crafted
       extra argument once this flat command line reaches a launcher. The
       executable must be an explicit path (no PATH-redirect via a bare name);
       classpath, main class, and source file are single tokens (no spaces);
       only program_arguments may carry interior spaces. */
    if (!executable_path_is_explicit(request->java_executable) ||
        !field_is_safe(request->classpath, 0) ||
        !field_is_safe(request->main_class, 0) ||
        !field_is_safe(request->source_file, 0) ||
        !field_is_safe(request->program_arguments, 1)) {
        memset(plan, 0, sizeof(*plan));
        return -1;
    }

    /* Process creation and platform-specific argument escaping remain with
       the existing launcher/provider; this layer does not invoke a shell. */
    int cmd_len;
    if (request->source_file && request->source_file[0]) {
        if (args[0]) {
            cmd_len = snprintf(plan->command, sizeof(plan->command),
                     "%s %s %s",
                     request->java_executable, request->source_file, args);
        } else {
            cmd_len = snprintf(plan->command, sizeof(plan->command),
                     "%s %s",
                     request->java_executable, request->source_file);
        }
    } else if (args[0]) {
        cmd_len = snprintf(plan->command, sizeof(plan->command),
                 "%s -cp %s %s %s",
                 request->java_executable, classpath,
                 request->main_class, args);
    } else {
        cmd_len = snprintf(plan->command, sizeof(plan->command),
                 "%s -cp %s %s",
                 request->java_executable, classpath,
                 request->main_class);
    }
    /* Reject a truncated command line rather than returning a silently
       malformed one (snprintf returns the length it would have written). */
    if (cmd_len < 0 || (size_t)cmd_len >= sizeof(plan->command)) {
        memset(plan, 0, sizeof(*plan));
        return -1;
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
