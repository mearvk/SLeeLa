#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TMP="/tmp/sleela-java-runtime-bridge.$$"
mkdir -p "$TMP"
trap 'rm -rf "$TMP"' EXIT

cat > "$TMP/bridge.c" <<'EOF'
#include <stdio.h>
#include <string.h>
#include "java_runtime_bridge.h"

static int check(int condition, const char *message) {
    if (!condition) { fprintf(stderr, "FAIL: %s\n", message); return 1; }
    return 0;
}

int main(void) {
    SleelaJavaRuntimeProbeResult probe;
    SleelaJavaProgramRequest request = {
        "/opt/java/bin/java",
        "build/classes",
        "example.Hello",
        NULL,
        "one two",
        "hello\n"
    };
    SleelaJavaProgramPlan plan;

    const char *source = "import javax.swing.JFrame; class Hello {}";
    if (sleela_java_runtime_dispatch_source(source, strlen(source), "Hello.java",
        &request, &probe, &plan) != 0) return 1;
    if (check(probe.action == SLEELA_JAVA_ACTION_LOCAL_VM ||
              probe.action == SLEELA_JAVA_ACTION_PROMPT_INSTALL,
              "Java dispatch decision")) return 1;

    if (probe.action == SLEELA_JAVA_ACTION_PROMPT_INSTALL) {
        puts("PASS: SLeeLa Java runtime bridge prompt path");
        return 0;
    }

    if (sleela_java_runtime_bridge_prepare(&request, &plan) != 0) return 1;
    if (check(plan.ready, "bridge plan ready")) return 1;
    if (check(strstr(plan.command,
        "/opt/java/bin/java -cp build/classes example.Hello one two") != NULL,
        "normal JVM invocation")) return 1;
    if (check(strcmp(plan.sample_input, "hello\n") == 0,
        "sample input contract")) return 1;
    if (check(plan.sample_output_hint[0] == '\\0',
        "provider-supplied output contract")) return 1;

    puts("PASS: SLeeLa Java runtime bridge plan");
    return 0;
}
EOF

cc -std=c11 -Wall -Wextra -Werror \
  -I"$ROOT/runtime" \
  "$ROOT/runtime/java_runtime_bridge.c" \
  "$TMP/bridge.c" \
  -o "$TMP/bridge"

"$TMP/bridge"
