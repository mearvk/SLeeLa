#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TMP="/tmp/sleela-java-runtime-probe.$$"
mkdir -p "$TMP"
trap 'rm -rf "$TMP"' EXIT

cat > "$TMP/probe.c" <<'EOF'
#include <stdio.h>
#include <string.h>
#include "java_runtime_probe.h"

static int check(int condition, const char *message) {
    if (!condition) { fprintf(stderr, "FAIL: %s\n", message); return 1; }
    return 0;
}

int main(void) {
    SleelaJavaRuntimeProbeResult r;

    const char *swing =
        "import javax.swing.JFrame;\n"
        "import java.awt.Button;\n"
        "class Main {}";
    if (sleela_java_runtime_probe_source(swing, strlen(swing), "Main.java", &r) != 0) return 1;
    if (check(r.kind == SLEELA_JAVA_SWING, "Swing classification")) return 1;
    if (check(r.framework_signals >= 2, "AWT and Swing signals")) return 1;

    const char *fx =
        "import javafx.application.Application;\n"
        "import javafx.stage.Stage;";
    if (sleela_java_runtime_probe_source(fx, strlen(fx), "Main.java", &r) != 0) return 1;
    if (check(r.kind == SLEELA_JAVA_FX, "JavaFX classification")) return 1;

    const char *plain = "class PlainSleela {}";
    if (sleela_java_runtime_probe_source(plain, strlen(plain), "Main.sleela", &r) != 0) return 1;
    if (check(r.kind == SLEELA_JAVA_NONE, "Native SLeeLa classification")) return 1;
    if (check(r.action == SLEELA_JAVA_ACTION_CONTINUE, "Native SLeeLa action")) return 1;

    const char *envelope =
        "String javaType = \"javafx.scene.Node\";\n"
        "String construct() { return \"java.construct:javafx.scene.Node\"; }";
    if (sleela_java_runtime_probe_source(envelope, strlen(envelope), "Node.sleela", &r) != 0) return 1;
    if (check(r.kind == SLEELA_JAVA_FX, "JavaFX envelope classification")) return 1;

    puts("PASS: SLeeLa Java runtime dry-layer probe");
    return 0;
}
EOF

cc -std=c11 -Wall -Wextra -Werror   -I"$ROOT/runtime"   "$ROOT/runtime/java_runtime_probe.c"   "$TMP/probe.c"   -o "$TMP/probe"

"$TMP/probe"
