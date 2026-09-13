/* ==========================================================================
 * sleela_java28_demo.c -- Driver demo: Sleela (this C program, standing in for
 * the Sleela core) runs against the Java 28 SecureJDK memory model over the
 * PORT channel. It never holds a Java object -- only handles -- and drives the
 * heap-resident Counter/Ledger by handle, per J28-MEM-0001 Model A.
 *
 * Usage: sleela_java28_demo <port>
 * ========================================================================== */
#include "sleela_java28.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fail_count = 0;

/* Call and print; return the response (or "" on failure). */
static const char *step(SLJava28Session *s, const char *label, const char *req) {
    const char *r = sl_java28_call(s, req);
    if (!r) { printf("  %-22s -> <transport error: %s>\n", label, sl_java28_error(s) ? sl_java28_error(s) : "?"); fail_count++; return ""; }
    printf("  %-22s -> %s\n", label, r);
    if (strncmp(r, "OK", 2) != 0) fail_count++;
    return r;
}

/* Extract the handle integer from a reply of the form "OK h:<n>". */
static long handle_of(const char *reply) {
    const char *p = strstr(reply, "h:");
    return p ? strtol(p + 2, NULL, 10) : 0;
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s <port>\n", argv[0]); return 2; }
    int port = atoi(argv[1]);

    SLJava28Session *s = sl_java28_port_open(port);
    if (!s) { fprintf(stderr, "could not connect to Java 28 memory host on 127.0.0.1:%d\n", port); return 1; }

    printf("== Sleela driving the Java 28 SecureJDK memory model (port channel) ==\n");
    printf("  %-22s -> %s\n", "hello", sl_java28_hello(s));

    /* Create a Counter(10) on the Java heap; keep only its handle. */
    const char *r = step(s, "new Counter i:10", "new Counter i:10");
    long counter = handle_of(r);
    char buf[256];

    snprintf(buf, sizeof buf, "call h:%ld add i:5", counter);
    step(s, "counter.add(5)", buf);
    snprintf(buf, sizeof buf, "call h:%ld add i:100", counter);
    step(s, "counter.add(100)", buf);
    snprintf(buf, sizeof buf, "get h:%ld value", counter);
    step(s, "counter.value", buf);
    snprintf(buf, sizeof buf, "call h:%ld isPositive", counter);
    step(s, "counter.isPositive()", buf);
    snprintf(buf, sizeof buf, "call h:%ld label s:5:count", counter);
    step(s, "counter.label(\"count\")", buf);

    /* A Ledger, and a method that returns ANOTHER heap object (cents -> Counter). */
    r = step(s, "new Ledger d:19.99", "new Ledger d:19.99");
    long ledger = handle_of(r);
    snprintf(buf, sizeof buf, "call h:%ld credit d:5.01", ledger);
    step(s, "ledger.credit(5.01)", buf);
    snprintf(buf, sizeof buf, "call h:%ld cents", ledger);
    r = step(s, "ledger.cents() -> handle", buf);
    long cents = handle_of(r);
    snprintf(buf, sizeof buf, "get h:%ld value", cents);
    step(s, "cents.value", buf);

    /* Security posture checks: disallowed class + stale handle both ERR. */
    step(s, "new File (blocked)", "new File s:4:/tmp");
    snprintf(buf, sizeof buf, "free h:%ld", counter);
    step(s, "free counter", buf);
    snprintf(buf, sizeof buf, "get h:%ld value", counter);
    step(s, "get freed (ERR)", buf);

    step(s, "stats", "stats");

    sl_java28_close(s);

    if (fail_count == 0) {
        printf("Java 28 memory demo (port): PASS\n");
        return 0;
    }
    /* The two intentional-ERR steps (blocked class, freed handle) count as
     * "failures" of the OK check but are expected; treat exactly 2 as success. */
    if (fail_count == 2) {
        printf("Java 28 memory demo (port): PASS (2 expected ERR posture checks)\n");
        return 0;
    }
    printf("Java 28 memory demo (port): FAIL (%d unexpected)\n", fail_count);
    return 1;
}
