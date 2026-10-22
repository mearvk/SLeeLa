/* http1_server.h — SLeeLa HTTP/1.0 portable server (Linux · macOS · Windows 10+)
 *
 * This is the transport layer that turns the negotiation spec, logical-port
 * routing model, and resumable-download contract described in FLOW.md,
 * HTTP.NEGOTIATION.md, and README.md into a running, internet-facing server.
 *
 * Uses the portable sleela_net (impl/core/sleela_net.{c,h}) socket abstraction
 * so the same source compiles on all three platforms without #ifdef in the
 * server logic.
 */
#ifndef SLEELA_HTTP1_SERVER_H
#define SLEELA_HTTP1_SERVER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── limits ─────────────────────────────────────────────────────────── */

#define HTTP1_MAX_HEADER_SIZE   (64 * 1024)
#define HTTP1_MAX_BODY_SIZE     (8 * 1024 * 1024)
#define HTTP1_MAX_HEADERS       64
#define HTTP1_MAX_METHOD_LEN    16
#define HTTP1_MAX_URI_LEN       8192
#define HTTP1_MAX_VERSION_LEN   16
#define HTTP1_MAX_HEADER_NAME   128
#define HTTP1_MAX_HEADER_VALUE  2048

/* ── download-mode threshold (FLOW.md / README.md) ─────────────────── */

#define HTTP1_DOWNLOAD_THRESHOLD (50ULL * 1024 * 1024)  /* 50 MB */

/* ── request / response structures ─────────────────────────────────── */

typedef struct {
    char name[HTTP1_MAX_HEADER_NAME];
    char value[HTTP1_MAX_HEADER_VALUE];
} http1_header;

typedef struct {
    char            method[HTTP1_MAX_METHOD_LEN];
    char            uri[HTTP1_MAX_URI_LEN];
    char            version[HTTP1_MAX_VERSION_LEN];
    http1_header    headers[HTTP1_MAX_HEADERS];
    int             header_count;
    char           *body;           /* heap-allocated if present, caller frees */
    size_t          body_length;

    /* SLeeLa routing fields (parsed from X-SLeeLa-* headers or URI). */
    char            sleela_port[64];
    char            service_id[128];
    char            op_id[128];

    /* SLeeLa version negotiation (HTTP.NEGOTIATION.md). */
    char            sleela_version[32];
    char            sleela_accept[256];
} http1_request;

typedef struct {
    int             status_code;
    const char     *reason;
    http1_header    headers[HTTP1_MAX_HEADERS];
    int             header_count;
    const char     *body;
    size_t          body_length;
    const char     *content_type;
} http1_response;

/* ── resumable download state (README.md / FLOW.md) ────────────────── */

typedef struct {
    char        session_id[64];
    char        datetime[32];
    char        file_id[128];
    char        file_name[256];
    uint64_t    index;
    uint64_t    offset;
    uint64_t    total_size;
} http1_download_state;

/* ── route handler callback ────────────────────────────────────────── */

/*
 * Application code provides a handler that receives a parsed request and
 * fills in the response.  Return 0 for success, non-zero to send 500.
 */
typedef int (*http1_route_handler)(const http1_request *req,
                                   http1_response      *resp,
                                   void                *user_data);

/* ── server configuration ──────────────────────────────────────────── */

typedef struct {
    uint16_t            port;           /* TCP listen port (default 8080)      */
    const char         *bind_address;   /* NULL or "0.0.0.0" for any           */
    int                 backlog;        /* listen backlog    (default 128)     */
    http1_route_handler handler;        /* application route handler           */
    void               *user_data;      /* opaque context for the handler      */
    const char         *server_name;    /* Server header     (default "SLeeLa-HTTP/1.0") */
    int                 allow_negotiation_fallback; /* negotiate down to 1.0   */
} http1_server_config;

/* ── public API ────────────────────────────────────────────────────── */

/*
 * Initialise a config struct with sensible defaults.
 */
void http1_server_config_init(http1_server_config *cfg);

/*
 * Run the server.  Blocks until http1_server_stop() is called from a signal
 * handler or another thread.  Returns 0 on clean shutdown.
 */
int http1_server_run(const http1_server_config *cfg);

/*
 * Request a running server to stop (signal-safe).
 */
void http1_server_stop(void);

/* Parse one bounded request header block; raw_len must stop at CRLFCRLF. */
int http1_request_parse_headers(const char *raw, size_t raw_len,
                                http1_request *req, size_t *header_bytes);

/*
 * Convenience: add a header to a response being built by a handler.
 */
int http1_response_add_header(http1_response *resp,
                              const char     *name,
                              const char     *value);

/*
 * Parse / serialise resumable-download state.
 * download_parse:     reads a "SESSION-ID | … | TOTAL-SIZE" line.
 * download_serialise: writes one into buf (must be ≥512 bytes).
 * download_should_resume: returns 1 when Content-Length > 50 MB.
 */
int  http1_download_parse(const char *line, http1_download_state *out);
int  http1_download_serialise(const http1_download_state *state,
                              char *buf, size_t buf_size);
int  http1_download_should_resume(uint64_t content_length);

/*
 * Built-in self-test (parse round-trip, negotiation, download state).
 * Returns 0 on pass.
 */
int http1_self_test(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_HTTP1_SERVER_H */
