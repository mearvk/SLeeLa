/* ==========================================================================
 * sleela_java28.h -- Sleela-side C ABI for the Java 28 SecureJDK memory link.
 *
 * Model A (J28-MEM-0001): Sleela drives; the Java 28 SecureJDK owns the object
 * memory. Both channels expose the SAME call surface -- one request line in,
 * one response line out, per the §4 grammar -- so a Sleela program is agnostic
 * to whether it is bound over a port (socket) or JNI (local feedback).
 * ========================================================================== */
#ifndef SLEELA_JAVA28_H
#define SLEELA_JAVA28_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SLJava28Session SLJava28Session;

/* ---- Port channel (socket) ------------------------------------------------
 * Connect to a Java 28 SecureJDK memory server on 127.0.0.1:<port>. Returns
 * NULL on failure. */
SLJava28Session *sl_java28_port_open(int port);

/* Send one request line (without trailing newline); returns the response line.
 * The returned pointer is owned by the session and valid until the next call
 * or close. Returns NULL on transport failure. */
const char *sl_java28_call(SLJava28Session *s, const char *request_line);

/* Convenience helpers that build a request line and return the response. */
const char *sl_java28_hello(SLJava28Session *s);

/* Close and free the session (sends "bye" if the channel is a port). */
void sl_java28_close(SLJava28Session *s);

/* Last transport error string, or NULL. */
const char *sl_java28_error(SLJava28Session *s);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_JAVA28_H */
