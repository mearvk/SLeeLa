/* ==========================================================================
 * sleela_java28_bridge.c -- JNI (local feedback) channel for the Java 28
 * SecureJDK memory link (J28-MEM-0001 §5).
 *
 * In-process analogue of the port channel: the Sleela-side native driver
 * (runNativeDemo) issues request lines and calls back into the Java runtime's
 * dispatch(String), which funnels to the shared SleelaMemoryServer.handleLine.
 * Same grammar, same object memory model, no socket.
 * ========================================================================== */
#define _POSIX_C_SOURCE 200809L
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Cached method id for Java28JniRuntime.dispatch(String)String. */
static jmethodID g_dispatch = 0;

/* Call dispatch(line) on the runtime object; returns a malloc'd C string
 * (caller frees) or NULL. */
static char *call_dispatch(JNIEnv *env, jobject self, const char *line) {
    if (!g_dispatch) {
        jclass cls = (*env)->GetObjectClass(env, self);
        g_dispatch = (*env)->GetMethodID(env, cls, "dispatch",
                                         "(Ljava/lang/String;)Ljava/lang/String;");
        if (!g_dispatch) return NULL;
    }
    jstring jreq = (*env)->NewStringUTF(env, line);
    jstring jresp = (jstring)(*env)->CallObjectMethod(env, self, g_dispatch, jreq);
    (*env)->DeleteLocalRef(env, jreq);
    if ((*env)->ExceptionCheck(env)) { (*env)->ExceptionClear(env); return NULL; }
    if (!jresp) return NULL;
    const char *utf = (*env)->GetStringUTFChars(env, jresp, NULL);
    char *copy = utf ? strdup(utf) : NULL;
    if (utf) (*env)->ReleaseStringUTFChars(env, jresp, utf);
    (*env)->DeleteLocalRef(env, jresp);
    return copy;
}

static int fail_count = 0;

/* Issue one request via local feedback, print it, track OK/ERR. */
static char *step(JNIEnv *env, jobject self, const char *label, const char *req) {
    char *r = call_dispatch(env, self, req);
    if (!r) { printf("  %-24s -> <dispatch error>\n", label); fail_count++; return NULL; }
    printf("  %-24s -> %s\n", label, r);
    if (strncmp(r, "OK", 2) != 0) fail_count++;
    return r;
}

static long handle_of(const char *reply) {
    if (!reply) return 0;
    const char *p = strstr(reply, "h:");
    return p ? strtol(p + 2, NULL, 10) : 0;
}

JNIEXPORT jint JNICALL
Java_com_mearvk_sleela_java28_Java28JniRuntime_runNativeDemo(JNIEnv *env, jobject self) {
    fail_count = 0;
    char buf[256];
    char *r;

    r = call_dispatch(env, self, "hello i:1");
    printf("  %-24s -> %s\n", "hello", r ? r : "<null>");
    free(r);

    r = step(env, self, "new Counter i:10", "new Counter i:10");
    long counter = handle_of(r); free(r);

    snprintf(buf, sizeof buf, "call h:%ld add i:5", counter);
    free(step(env, self, "counter.add(5)", buf));
    snprintf(buf, sizeof buf, "call h:%ld add i:100", counter);
    free(step(env, self, "counter.add(100)", buf));
    snprintf(buf, sizeof buf, "get h:%ld value", counter);
    free(step(env, self, "counter.value", buf));
    snprintf(buf, sizeof buf, "call h:%ld isPositive", counter);
    free(step(env, self, "counter.isPositive()", buf));
    snprintf(buf, sizeof buf, "call h:%ld label s:5:count", counter);
    free(step(env, self, "counter.label(\"count\")", buf));

    r = step(env, self, "new Ledger d:19.99", "new Ledger d:19.99");
    long ledger = handle_of(r); free(r);
    snprintf(buf, sizeof buf, "call h:%ld credit d:5.01", ledger);
    free(step(env, self, "ledger.credit(5.01)", buf));
    snprintf(buf, sizeof buf, "call h:%ld cents", ledger);
    r = step(env, self, "ledger.cents() -> handle", buf);
    long cents = handle_of(r); free(r);
    snprintf(buf, sizeof buf, "get h:%ld value", cents);
    free(step(env, self, "cents.value", buf));

    /* Security posture: disallowed class + freed handle both ERR (expected). */
    free(step(env, self, "new File (blocked)", "new File s:4:/tmp"));
    snprintf(buf, sizeof buf, "free h:%ld", counter);
    free(step(env, self, "free counter", buf));
    snprintf(buf, sizeof buf, "get h:%ld value", counter);
    free(step(env, self, "get freed (ERR)", buf));

    free(step(env, self, "stats", "stats"));

    /* Exactly two intentional ERR posture checks are expected. */
    if (fail_count == 0 || fail_count == 2) return 0;
    return 1;
}
