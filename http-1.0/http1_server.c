/* http1_server.c — SLeeLa HTTP/1.0 portable server implementation
 *
 * Platforms: Linux, macOS, Windows 10+
 *
 * Uses the sleela_net portable socket layer (impl/core/sleela_net.{c,h}) so
 * all platform-specific socket code lives in one place.  The resolver and
 * preferred-router policy hooks are checked on every connection just as the
 * negotiation module already requires.
 */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include "http1_server.h"
#include "http_negotiation.h"
#include "../impl/core/sleela_net.h"
#include "../preferred-routers/preferred_router.h"
#include "../resolver/resolver.h"

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <poll.h>
#include <unistd.h>
#endif

/* ── signal-safe stop flag ──────────────────────────────────────────── */

static volatile int g_stop = 0;

void http1_server_stop(void)
{
    g_stop = 1;
}

#ifndef _WIN32
static void http1_signal_handler(int sig)
{
    (void)sig;
    g_stop = 1;
}
#endif

/* ── helpers ────────────────────────────────────────────────────────── */

static void str_lower(char *s)
{
    for (; *s; ++s)
        *s = (char)tolower((unsigned char)*s);
}

static const char *str_trim_left(const char *s)
{
    while (*s == ' ' || *s == '\t') ++s;
    return s;
}

static const char *reason_phrase(int code)
{
    switch (code) {
    case 200: return "OK";
    case 204: return "No Content";
    case 206: return "Partial Content";
    case 304: return "Not Modified";
    case 400: return "Bad Request";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 413: return "Content Too Large";
    case 416: return "Range Not Satisfiable";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    default:  return "Error";
    }
}

/* Read deadlines prevent a single slow peer from monopolising the serial server. */
#define HTTP1_READ_TIMEOUT_SECONDS 10
static int portable_poll_readable(SLNetHandle fd, int timeout_ms);

/* Strict token/value helpers used at the untrusted HTTP boundary. */
static int http1_token_char(unsigned char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) return 1;
    switch (c) {
    case '!': case '#': case '$': case '%': case '&': case 39: case '*':
    case '+': case '-': case '.': case '^': case '_': case 96: case '|':
    case '~': return 1;
    default: return 0;
    }
}
static int http1_safe_value(const char *s) {
    if (!s) return 0;
    for (; *s; ++s) { unsigned char c = (unsigned char)*s; if ((c < 32 && c != 9) || c == 127) return 0; }
    return 1;
}
static int http1_parse_size(const char *s, size_t *out) {
    size_t n = 0;
    if (!s || !*s || !out) return -1;
    for (; *s; ++s) { unsigned char c = (unsigned char)*s; if (c < '0' || c > '9') return -1; if (n > (SIZE_MAX - (size_t)(c - '0')) / 10) return -1; n = n * 10 + (size_t)(c - '0'); }
    *out = n; return 0;
}

/* ── request parsing ────────────────────────────────────────────────── */

/*
 * Read from the socket until we have the full header block (\r\n\r\n) or
 * hit the header-size limit.  Returns the number of bytes in buf on success,
 * or -1 on error / timeout.
 */
static int recv_headers(SLNetHandle fd, char *buf, size_t cap)
{
    size_t total = 0;
    time_t deadline = time(NULL) + HTTP1_READ_TIMEOUT_SECONDS;

    while (total < cap) {
        if (time(NULL) >= deadline) return -1;
        if (!portable_poll_readable(fd, 1000)) continue;
        SLNetCount n = slnet_read(fd, buf + total, cap - total);
        if (n <= 0)
            return total > 0 ? (int)total : -1;
        total += (size_t)n;
        /* Check for the end-of-headers sentinel. */
        if (total >= 4) {
            const char *end = buf + (total >= HTTP1_MAX_HEADER_SIZE
                                     ? HTTP1_MAX_HEADER_SIZE : total);
            for (const char *p = buf + (total - n > 3 ? total - n - 3 : 0);
                 p + 3 < end; ++p) {
                if (p[0] == '\r' && p[1] == '\n' &&
                    p[2] == '\r' && p[3] == '\n')
                    return (int)total;
            }
        }
    }
    return -1;  /* headers exceeded cap without terminating */
}

static int find_header(const http1_request *req, const char *name)
{
    for (int i = 0; i < req->header_count; ++i) {
        /* headers are stored lowered */
        if (strcmp(req->headers[i].name, name) == 0)
            return i;
    }
    return -1;
}

static const char *get_header(const http1_request *req, const char *name)
{
    int i = find_header(req, name);
    return i >= 0 ? req->headers[i].value : NULL;
}

/*
 * Parse a raw header block into an http1_request.  Returns 0 on success.
 * The body (if any, per Content-Length) must be read separately.
 */
static int parse_request(const char *raw, size_t raw_len, http1_request *req,
                         const char **body_start, size_t *header_bytes)
{
    const char *p = raw;
    const char *end = raw + raw_len;
    const char *line_end;

    if (!raw || !req || !body_start || !header_bytes || raw_len > HTTP1_MAX_HEADER_SIZE || memchr(raw, 0, raw_len) != NULL) return -1;
    memset(req, 0, sizeof(*req));

    /* ---- request line: METHOD SP URI SP HTTP/x.y CRLF ---- */
    line_end = strstr(p, "\r\n");
    if (!line_end) return -1;

    /* method */
    {
        const char *sp = memchr(p, ' ', (size_t)(line_end - p));
        if (!sp || (size_t)(sp - p) >= sizeof(req->method)) return -1;
        if (sp == p) return -1;
        for (const char *q = p; q < sp; ++q) if (!http1_token_char((unsigned char)*q)) return -1;
        memcpy(req->method, p, (size_t)(sp - p));
        p = sp + 1;
    }

    /* URI */
    {
        const char *sp = memchr(p, ' ', (size_t)(line_end - p));
        if (!sp || (size_t)(sp - p) >= sizeof(req->uri)) return -1;
        if (sp == p) return -1;
        for (const char *q = p; q < sp; ++q) { unsigned char c = (unsigned char)*q; if (c <= 32 || c == 127) return -1; }
        memcpy(req->uri, p, (size_t)(sp - p));
        p = sp + 1;
    }

    /* version */
    {
        size_t vlen = (size_t)(line_end - p);
        if (vlen >= sizeof(req->version)) return -1;
        memcpy(req->version, p, vlen); req->version[vlen] = 0;
        if (strcmp(req->version, "HTTP/1.0") != 0 && strcmp(req->version, "HTTP/1.1") != 0) return -1;
    }

    p = line_end + 2; /* skip CRLF */

    /* ---- headers ---- */
    int content_length_seen = 0, transfer_encoding_seen = 0;
    while (p + 1 < end && !(p[0] == '\r' && p[1] == '\n')) {
        line_end = strstr(p, "\r\n");
        if (!line_end) break;

        const char *colon = memchr(p, ':', (size_t)(line_end - p));
        if (!colon || colon == p || *p == ' ' || *p == 9) return -1;
        for (const char *q = p; q < colon; ++q) if (!http1_token_char((unsigned char)*q)) return -1;

        if (req->header_count >= HTTP1_MAX_HEADERS) return -1;

        http1_header *h = &req->headers[req->header_count];
        size_t nlen = (size_t)(colon - p);
        if (nlen >= sizeof(h->name)) return -1;
        memcpy(h->name, p, nlen);
        h->name[nlen] = '\0';
        str_lower(h->name);
        if (find_header(req, h->name) >= 0) return -1; /* reject ambiguous duplicate fields */

        const char *val = str_trim_left(colon + 1);
        size_t vlen = (size_t)(line_end - val);
        if (vlen >= sizeof(h->value)) return -1;
        memcpy(h->value, val, vlen);
        h->value[vlen] = '\0';
        if (!http1_safe_value(h->value)) return -1;
        if (strcmp(h->name, "content-length") == 0) { size_t ignored; if (content_length_seen++ || http1_parse_size(h->value, &ignored) != 0) return -1; }
        if (strcmp(h->name, "transfer-encoding") == 0) transfer_encoding_seen = 1;

        ++req->header_count;
        p = line_end + 2;
    }

    if (transfer_encoding_seen) return -1;
    if (p + 1 >= end || p[0] != '\r' || p[1] != '\n') return -1;
    p += 2; /* skip blank line */

    *header_bytes = (size_t)(p - raw);
    *body_start   = p;

    /* ---- SLeeLa application headers (HTTP.NEGOTIATION.md) ---- */
    {
        const char *v;

        v = get_header(req, "x-sleela-version");
        if (v && strlen(v) >= sizeof(req->sleela_version)) return -1;
        if (v) snprintf(req->sleela_version, sizeof(req->sleela_version), "%s", v);

        v = get_header(req, "x-sleela-accept");
        if (v && strlen(v) >= sizeof(req->sleela_accept)) return -1;
        if (v) snprintf(req->sleela_accept, sizeof(req->sleela_accept), "%s", v);

        v = get_header(req, "x-sleela-port");
        if (v && strlen(v) >= sizeof(req->sleela_port)) return -1;
        if (v) snprintf(req->sleela_port, sizeof(req->sleela_port), "%s", v);

        v = get_header(req, "x-sleela-service-id");
        if (v && strlen(v) >= sizeof(req->service_id)) return -1;
        if (v) snprintf(req->service_id, sizeof(req->service_id), "%s", v);

        v = get_header(req, "x-sleela-op-id");
        if (v && strlen(v) >= sizeof(req->op_id)) return -1;
        if (v) snprintf(req->op_id, sizeof(req->op_id), "%s", v);
    }

    return 0;
}

/* ── response serialisation ─────────────────────────────────────────── */

int http1_response_add_header(http1_response *resp,
                              const char     *name,
                              const char     *value)
{
    if (!resp || !name || !value || !*name || !http1_safe_value(value)) return -1;
    for (const char *p = name; *p; ++p) if (!http1_token_char((unsigned char)*p)) return -1;
    if (resp->header_count < 0 || resp->header_count >= HTTP1_MAX_HEADERS) return -1;
    if (strlen(name) >= sizeof(resp->headers[0].name) || strlen(value) >= sizeof(resp->headers[0].value)) return -1;
    http1_header *h = &resp->headers[resp->header_count++];
    memcpy(h->name, name, strlen(name) + 1); memcpy(h->value, value, strlen(value) + 1);
    return 0;
}

static int send_all(SLNetHandle fd, const char *data, size_t len)
{
    while (len > 0) {
        SLNetCount n = slnet_write(fd, data, len);
        if (n <= 0) return -1;
        data += n;
        len  -= (size_t)n;
    }
    return 0;
}

static int send_response(SLNetHandle fd, const http1_request *req,
                         const http1_response *resp, const char *server_name)
{
    char buf[HTTP1_MAX_HEADER_SIZE];
    int  off = 0;

    const char *reason = resp->reason ? resp->reason
                                      : reason_phrase(resp->status_code);
    /* Validate every handler-controlled field and bound the total header block
       before any snprintf arithmetic can advance beyond buf. */
    if (resp->header_count < 0 || resp->header_count > HTTP1_MAX_HEADERS ||
        resp->status_code < 100 || resp->status_code > 599 ||
        !http1_safe_value(reason) || !http1_safe_value(server_name) ||
        (resp->content_type && !http1_safe_value(resp->content_type)) ||
        resp->body_length > HTTP1_MAX_BODY_SIZE ||
        (resp->body_length && !resp->body))
        return -1;
    size_t estimate = 256 + strlen(reason) + strlen(server_name);
    if (resp->content_type) estimate += strlen(resp->content_type) + 32;
    for (int i = 0; i < resp->header_count; ++i) {
        if (!resp->headers[i].name[0] || !http1_safe_value(resp->headers[i].value)) return -1;
        for (const char *q = resp->headers[i].name; *q; ++q)
            if (!http1_token_char((unsigned char)*q)) return -1;
        estimate += strlen(resp->headers[i].name) + strlen(resp->headers[i].value) + 4;
        if (estimate >= sizeof(buf)) return -1;
    }

    /* Status line */
    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "%s %d %s\r\n",
                    req->version[0] ? req->version : "HTTP/1.0",
                    resp->status_code, reason);

    /* Server header */
    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "Server: %s\r\n", server_name);

    /* Content headers */
    if (resp->content_type)
        off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                        "Content-Type: %s\r\n", resp->content_type);

    size_t wire_body_length = resp->body_length;
    int suppress_body = (strcmp(req->method, "HEAD") == 0 ||
                         resp->status_code == 204 || resp->status_code == 304);
    if (resp->status_code == 204 || resp->status_code == 304)
        wire_body_length = 0;
    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "Content-Length: %zu\r\n", wire_body_length);

    /* Application headers from the handler */
    for (int i = 0; i < resp->header_count; ++i)
        off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                        "%s: %s\r\n",
                        resp->headers[i].name,
                        resp->headers[i].value);

    /* SLeeLa version advertisement (HTTP.NEGOTIATION.md) */
    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "X-SLeeLa-Version: 1.0\r\n"
                    "X-SLeeLa-Accept: 1.0, 1.1\r\n");

    /* HTTP/1.0: connection closes after each response */
    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "Connection: close\r\n\r\n");

    if (off <= 0 || (size_t)off >= sizeof(buf)) return -1;

    if (send_all(fd, buf, (size_t)off) != 0) return -1;

    if (!suppress_body && resp->body && wire_body_length > 0)
        return send_all(fd, resp->body, wire_body_length);

    return 0;
}

/* ── download-mode helpers (README.md / FLOW.md) ───────────────────── */

int http1_download_should_resume(uint64_t content_length)
{
    return content_length > HTTP1_DOWNLOAD_THRESHOLD ? 1 : 0;
}

static int http1_download_field_safe(const char *s, size_t cap, int filename)
{
    if (!s || !*s || strlen(s) >= cap) return 0;
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
        if (*p < 32 || *p == 127 || *p == '|') return 0;
        if (filename && (*p == '/' || *p == '\\' || *p == ':')) return 0;
    }
    if (filename && (strstr(s, "..") != NULL || strcmp(s, ".") == 0)) return 0;
    return 1;
}

int http1_download_should_resume(uint64_t content_length)
{
    return content_length > HTTP1_DOWNLOAD_THRESHOLD ? 1 : 0;
}

int http1_download_serialise(const http1_download_state *st,
                             char *buf, size_t buf_size)
{
    if (!st || !buf || buf_size < 512 ||
        !http1_download_field_safe(st->session_id, sizeof(st->session_id), 0) ||
        !http1_download_field_safe(st->datetime, sizeof(st->datetime), 0) ||
        !http1_download_field_safe(st->file_id, sizeof(st->file_id), 0) ||
        !http1_download_field_safe(st->file_name, sizeof(st->file_name), 1) ||
        st->offset > st->total_size)
        return -1;
    int n = snprintf(buf, buf_size, "%s | %s | %s | %s | %llu | %llu | %llu",
                     st->session_id, st->datetime, st->file_id, st->file_name,
                     (unsigned long long)st->index, (unsigned long long)st->offset,
                     (unsigned long long)st->total_size);
    return (n > 0 && (size_t)n < buf_size) ? 0 : -1;
}

int http1_download_parse(const char *line, http1_download_state *out)
{
    if (!line || !out || strlen(line) >= 2048) return -1;
    memset(out, 0, sizeof(*out));
    char copy[2048];
    memcpy(copy, line, strlen(line) + 1);
    char *fields[7];
    int count = 0;
    char *cursor = copy;
    fields[count++] = cursor;
    for (char *p = copy; *p; ++p) {
        if (*p == '|') {
            *p = 0;
            if (count >= 7) return -1;
            fields[count++] = p + 1;
        }
    }
    if (count != 7) return -1;
    for (int i = 0; i < 7; ++i) {
        while (*fields[i] == ' ' || *fields[i] == '\\t') ++fields[i];
        size_t n = strlen(fields[i]);
        while (n && (fields[i][n-1] == ' ' || fields[i][n-1] == 9)) fields[i][--n] = 0;
    }
    if (!http1_download_field_safe(fields[0], sizeof(out->session_id), 0) ||
        !http1_download_field_safe(fields[1], sizeof(out->datetime), 0) ||
        !http1_download_field_safe(fields[2], sizeof(out->file_id), 0) ||
        !http1_download_field_safe(fields[3], sizeof(out->file_name), 1))
        return -1;
    size_t index_value, offset_value, total_value;
    if (http1_parse_size(fields[4], &index_value) != 0 ||
        http1_parse_size(fields[5], &offset_value) != 0 ||
        http1_parse_size(fields[6], &total_value) != 0 ||
        offset_value > total_value)
        return -1;
    snprintf(out->session_id, sizeof(out->session_id), "%s", fields[0]);
    snprintf(out->datetime, sizeof(out->datetime), "%s", fields[1]);
    snprintf(out->file_id, sizeof(out->file_id), "%s", fields[2]);
    snprintf(out->file_name, sizeof(out->file_name), "%s", fields[3]);
    out->index = (uint64_t)index_value;
    out->offset = (uint64_t)offset_value;
    out->total_size = (uint64_t)total_value;
    return 0;
}

/* ── built-in route handlers ────────────────────────────────────────── */

static int builtin_health(const http1_request *req, http1_response *resp)
{
    (void)req;
    static const char body[] = "SLeeLa HTTP/1.0 OK\n";
    resp->status_code  = 200;
    resp->content_type = "text/plain; charset=utf-8";
    resp->body         = body;
    resp->body_length  = sizeof(body) - 1;
    return 0;
}

static int builtin_status(const http1_request *req, http1_response *resp)
{
    (void)req;
    static char status_buf[512];
    int n = snprintf(status_buf, sizeof(status_buf),
        "{\"server\":\"SLeeLa\","
         "\"http_generation\":\"1.0\","
         "\"vm_generation\":0,"
         "\"vm_name\":\"Core\","
         "\"vm_path\":\"/impl\","
         "\"platform\":\"%s\","
         "\"route_data_schema\":\"1.0\","
         "\"lower_http_compatibility\":true}\n",
        slnet_platform_name());

    resp->status_code  = 200;
    resp->content_type = "application/json";
    resp->body         = status_buf;
    resp->body_length  = (size_t)n;
    return 0;
}

static int builtin_negotiate(const http1_request *req, http1_response *resp,
                             int allow_fallback)
{
    const char *negotiated = sleela_http_negotiate(
        req->sleela_version[0] ? req->sleela_version : NULL,
        req->sleela_accept[0]  ? req->sleela_accept  : NULL,
        allow_fallback);

    static char neg_buf[256];
    int n;
    if (negotiated) {
        n = snprintf(neg_buf, sizeof(neg_buf),
                     "{\"negotiated\":\"%s\",\"ok\":true}\n", negotiated);
        resp->status_code = 200;
    } else {
        n = snprintf(neg_buf, sizeof(neg_buf),
                     "{\"negotiated\":null,\"ok\":false}\n");
        resp->status_code = 400;
    }
    resp->content_type = "application/json";
    resp->body         = neg_buf;
    resp->body_length  = (size_t)n;
    return 0;
}

/* ── connection handler ─────────────────────────────────────────────── */

static void handle_connection(SLNetHandle fd, const http1_server_config *cfg)
{
    /* Heap-allocate the receive buffer — it can be up to ~8 MB, too large
     * for the default thread stack on most platforms. */
    size_t raw_cap = HTTP1_MAX_HEADER_SIZE + 4096; /* bounded header phase */
    char  *raw = malloc(raw_cap + 1);
    if (!raw) { slnet_close(fd); return; }
    http1_request *req = (http1_request *)calloc(1, sizeof(*req));
    http1_response *resp = (http1_response *)calloc(1, sizeof(*resp));
    if (!req || !resp) { free(req); free(resp); free(raw); slnet_close(fd); return; }

    int got = recv_headers(fd, raw, raw_cap);
    if (got <= 0) {
        free(req->body); free(req); free(resp); free(raw);
 slnet_close(fd);
 return;
    }
    raw[got] = '\0';

    const char    *body_start = NULL;
    size_t         header_bytes = 0;

    memset(resp, 0, sizeof(*resp));
    resp->status_code = 200;

    /* recv may coalesce body bytes with headers; parse only through CRLFCRLF. */
    size_t header_block_len = 0;
    for (size_t i = 0; i + 3 < (size_t)got; ++i) {
        if (raw[i] == 13 && raw[i+1] == 10 &&
            raw[i+2] == 13 && raw[i+3] == 10) {
            header_block_len = i + 4;
            break;
        }
    }
    if (header_block_len == 0 || header_block_len > HTTP1_MAX_HEADER_SIZE ||
        parse_request(raw, header_block_len, req, &body_start, &header_bytes) != 0) {
        resp->status_code  = 400;
        resp->content_type = "text/plain";
        resp->body         = "Bad Request\n";
        resp->body_length  = 12;

        memset(req, 0, sizeof(*req));
        memcpy(req->version, "HTTP/1.0", 9);
        send_response(fd, req, resp, cfg->server_name);
        free(req->body); free(req); free(resp); free(raw);

        slnet_close(fd);

        return;
    }

    /* Read body if Content-Length is present. */
    {
        const char *cl = get_header(req, "content-length");
        if (cl) {
            size_t content_len = 0;
            if (http1_parse_size(cl, &content_len) != 0) { resp->status_code = 400; resp->content_type = "text/plain"; resp->body = "Bad Content-Length\n"; resp->body_length = strlen(resp->body); send_response(fd, req, resp, cfg->server_name); free(req->body); free(req); free(resp); free(raw); slnet_close(fd); return; }
            if (content_len > HTTP1_MAX_BODY_SIZE) {
                resp->status_code  = 413;
                resp->content_type = "text/plain";
                resp->body         = "Content Too Large\n";
                resp->body_length  = 18;
                send_response(fd, req, resp, cfg->server_name);
                free(req->body); free(req); free(resp); free(raw);

                slnet_close(fd);

                return;
            }
            size_t already = (size_t)(got) - header_bytes;
            req->body = malloc(content_len + 1);
            if (!req->body) {
                resp->status_code  = 500;
                resp->content_type = "text/plain";
                resp->body         = "Internal Server Error\n";
                resp->body_length  = 22;
                send_response(fd, req, resp, cfg->server_name);
                free(req->body); free(req); free(resp); free(raw);

                slnet_close(fd);

                return;
            }
            size_t filled = already < content_len ? already : content_len;
            if (filled > 0)
                memcpy(req->body, body_start, filled);
            size_t remaining = content_len - filled;
            time_t body_deadline = time(NULL) + HTTP1_READ_TIMEOUT_SECONDS;
            while (remaining > 0) {
                if (time(NULL) >= body_deadline) break;
                if (!portable_poll_readable(fd, 1000)) continue;
                SLNetCount n = slnet_read(fd, req->body + filled, remaining);
                if (n <= 0) break;
                filled    += (size_t)n;
                remaining -= (size_t)n;
            }
            if (remaining != 0) { free(req->body); req->body = NULL; resp->status_code = 400; resp->content_type = "text/plain"; resp->body = "Incomplete Request Body\n"; resp->body_length = strlen(resp->body); send_response(fd, req, resp, cfg->server_name); free(req->body); free(req); free(resp); free(raw); slnet_close(fd); return; }
            req->body_length = filled;
            req->body[filled] = '\0';
        }
    }

    /* Header buffer no longer needed. */
    free(raw);
    raw = NULL;

    /* ---- built-in routes ---- */
    int handled = 0;

    if (strcmp(req->uri, "/__sleela/health") == 0) {
        builtin_health(req, resp);
        handled = 1;
    } else if (strcmp(req->uri, "/__sleela/status") == 0) {
        builtin_status(req, resp);
        handled = 1;
    } else if (strcmp(req->uri, "/__sleela/negotiate") == 0) {
        builtin_negotiate(req, resp, cfg->allow_negotiation_fallback);
        handled = 1;
    }

    /* ---- application handler ---- */
    if (!handled && cfg->handler) {
        int rc = cfg->handler(req, resp, cfg->user_data);
        if (rc != 0) {
            resp->status_code  = 500;
            resp->content_type = "text/plain";
            resp->body         = "Internal Server Error\n";
            resp->body_length  = 22;
        }
    } else if (!handled) {
        resp->status_code  = 404;
        resp->content_type = "text/plain";
        resp->body         = "Not Found\n";
        resp->body_length  = 10;
    }

    send_response(fd, req, resp, cfg->server_name);

    free(req->body);
    free(req); free(resp);
    slnet_close(fd);
}

/* ── portable poll (abstract over poll / WSAPoll) ───────────────────── */

static int portable_poll_readable(SLNetHandle fd, int timeout_ms)
{
#ifdef _WIN32
    WSAPOLLFD pfd;
    pfd.fd      = (SOCKET)fd;
    pfd.events  = POLLIN;
    pfd.revents = 0;
    return WSAPoll(&pfd, 1, timeout_ms) > 0 && (pfd.revents & POLLIN);
#else
    struct pollfd pfd;
    pfd.fd      = (int)fd;
    pfd.events  = POLLIN;
    pfd.revents = 0;
    return poll(&pfd, 1, timeout_ms) > 0 && (pfd.revents & POLLIN);
#endif
}

/* ── server entry point ─────────────────────────────────────────────── */

void http1_server_config_init(http1_server_config *cfg)
{
    if (!cfg) return;
    memset(cfg, 0, sizeof(*cfg));
    cfg->port        = 8080;
    cfg->backlog     = 128;
    cfg->server_name = "SLeeLa-HTTP/1.0";
    cfg->allow_negotiation_fallback = 1;
}

int http1_server_run(const http1_server_config *cfg)
{
    if (!cfg) return -1;

    g_stop = 0;

    /* Initialise the portable socket layer. */
    if (slnet_startup() != 0) {
        fprintf(stderr, "http1: slnet_startup failed\n");
        return -1;
    }

    /* Policy checks (preferred-router + resolver). */
    if (!sleela_preferred_router_packet_policy("HTTP/1.0+", 1))
        fprintf(stderr, "http1: preferred-router policy unavailable, "
                        "continuing with built-in defaults\n");
    if (!sleela_resolver_event_policy("HTTP/1.0", 0))
        fprintf(stderr, "http1: resolver event policy check returned false, "
                        "continuing\n");

    sleela_preferred_router_startup(NULL, "HTTP/1.0");

    /* Listen. */
    SLNetHandle listener = slnet_listen(cfg->port, cfg->backlog);
    if (!slnet_valid(listener)) {
        fprintf(stderr, "http1: failed to listen on port %u\n",
                (unsigned)cfg->port);
        slnet_shutdown();
        return -1;
    }

    fprintf(stderr, "SLeeLa HTTP/1.0 listening on port %u  [%s]\n",
            (unsigned)cfg->port, slnet_platform_name());

#ifndef _WIN32
    signal(SIGINT,  http1_signal_handler);
    signal(SIGTERM, http1_signal_handler);
#endif

    /* Accept loop — one connection at a time (HTTP/1.0 model). */
    while (!g_stop) {
        if (!portable_poll_readable(listener, 500))
            continue;

        SLNetHandle client = slnet_accept(listener);
        if (!slnet_valid(client))
            continue;

        handle_connection(client, cfg);
        /* client is closed inside handle_connection */
    }

    slnet_close(listener);
    slnet_shutdown();
    fprintf(stderr, "SLeeLa HTTP/1.0 stopped.\n");
    return 0;
}

/* ── self-test ──────────────────────────────────────────────────────── */

int http1_self_test(void)
{
    int failures = 0;

    /* 1. Request parse round-trip */
    {
        const char raw[] =
            "GET /__sleela/health HTTP/1.0\r\n"
            "Host: localhost\r\n"
            "X-SLeeLa-Version: 1.0\r\n"
            "X-SLeeLa-Accept: 1.0, 1.1\r\n"
            "X-SLeeLa-Port: 80\r\n"
            "X-SLeeLa-Service-ID: health-check\r\n"
            "X-SLeeLa-Op-ID: ping\r\n"
            "\r\n";

        http1_request req;
        const char *body_start;
        size_t header_bytes;

        if (parse_request(raw, sizeof(raw) - 1, &req,
                          &body_start, &header_bytes) != 0) {
            fprintf(stderr, "FAIL: parse_request returned error\n");
            ++failures;
        } else {
            if (strcmp(req.method, "GET") != 0) {
                fprintf(stderr, "FAIL: method = '%s', expected 'GET'\n", req.method);
                ++failures;
            }
            if (strcmp(req.uri, "/__sleela/health") != 0) {
                fprintf(stderr, "FAIL: uri = '%s'\n", req.uri);
                ++failures;
            }
            if (strcmp(req.version, "HTTP/1.0") != 0) {
                fprintf(stderr, "FAIL: version = '%s'\n", req.version);
                ++failures;
            }
            if (strcmp(req.sleela_version, "1.0") != 0) {
                fprintf(stderr, "FAIL: sleela_version = '%s'\n", req.sleela_version);
                ++failures;
            }
            if (strcmp(req.sleela_port, "80") != 0) {
                fprintf(stderr, "FAIL: sleela_port = '%s'\n", req.sleela_port);
                ++failures;
            }
            if (strcmp(req.service_id, "health-check") != 0) {
                fprintf(stderr, "FAIL: service_id = '%s'\n", req.service_id);
                ++failures;
            }
            if (strcmp(req.op_id, "ping") != 0) {
                fprintf(stderr, "FAIL: op_id = '%s'\n", req.op_id);
                ++failures;
            }
        }
    }

    /* 2. POST with Content-Length */
    {
        const char raw[] =
            "POST /api/data HTTP/1.0\r\n"
            "Content-Length: 5\r\n"
            "\r\n"
            "hello";

        http1_request req;
        const char *body_start;
        size_t header_bytes;

        const char *header_end = strstr(raw, "\r\n\r\n");
        size_t only_headers = header_end ? (size_t)(header_end - raw) + 4 : 0;
        if (only_headers == 0 || parse_request(raw, only_headers, &req,
                          &body_start, &header_bytes) != 0) {
            fprintf(stderr, "FAIL: POST parse failed\n");
            ++failures;
        } else {
            if (strcmp(req.method, "POST") != 0) {
                fprintf(stderr, "FAIL: POST method = '%s'\n", req.method);
                ++failures;
            }
            if (header_bytes == 0 || body_start == NULL) {
                fprintf(stderr, "FAIL: body_start not set\n");
                ++failures;
            }
        }
    }

    /* 3. Reject ambiguous and malformed framing before body allocation. */
    {
        const char *bad[] = {
            "POST / HTTP/1.0\r\nContent-Length: 1\r\nContent-Length: 1\r\n\r\nX",
            "POST / HTTP/1.0\r\nTransfer-Encoding: chunked\r\n\r\n0\r\n\r\n",
            "POST / HTTP/1.0\r\nContent-Length: +1\r\n\r\nX",
            "GET / HTTP/1.0\r\nX-Test: safe\001bad\r\n\r\n"
        };
        for (size_t i = 0; i < sizeof(bad) / sizeof(bad[0]); ++i) {
            http1_request req;
            const char *body_start = NULL;
            size_t header_bytes = 0;
            if (parse_request(bad[i], strlen(bad[i]), &req, &body_start, &header_bytes) == 0) {
                fprintf(stderr, "FAIL: malformed framing test %zu accepted\n", i);
                ++failures;
            }
        }
    }

    /* 4. Download state round-trip */

    {
        http1_download_state st = {0};
        snprintf(st.session_id, sizeof(st.session_id), "sess-001");
        snprintf(st.datetime,   sizeof(st.datetime),   "2026-10-15T00:00:00Z");
        snprintf(st.file_id,    sizeof(st.file_id),    "file-abc");
        snprintf(st.file_name,  sizeof(st.file_name),  "bigfile.bin");
        st.index      = 3;
        st.offset     = 157286400ULL;
        st.total_size = 524288000ULL;

        char buf[512];
        if (http1_download_serialise(&st, buf, sizeof(buf)) != 0) {
            fprintf(stderr, "FAIL: download_serialise\n");
            ++failures;
        } else {
            http1_download_state parsed;
            if (http1_download_parse(buf, &parsed) != 0) {
                fprintf(stderr, "FAIL: download_parse\n");
                ++failures;
            } else {
                if (strcmp(parsed.session_id, st.session_id) != 0 ||
                    strcmp(parsed.file_id, st.file_id) != 0 ||
                    parsed.index != st.index ||
                    parsed.offset != st.offset ||
                    parsed.total_size != st.total_size) {
                    fprintf(stderr, "FAIL: download round-trip mismatch\n");
                    ++failures;
                }
            }
        }
    }

    /* 5. Reject unsafe and inconsistent download metadata. */
    {
        http1_download_state parsed;
        if (http1_download_parse("sess | 2026-10-15T00:00:00Z | file | ../secret | 0 | 0 | 10", &parsed) == 0) {
            fprintf(stderr, "FAIL: unsafe download filename accepted\n");
            ++failures;
        }
        if (http1_download_parse("sess | date | file | safe.bin | 0 | 11 | 10", &parsed) == 0) {
            fprintf(stderr, "FAIL: download offset past end accepted\n");
            ++failures;
        }
        if (http1_download_parse("sess | date | file | safe.bin | 184467440737095516160 | 0 | 1", &parsed) == 0) {
            fprintf(stderr, "FAIL: overflowing download index accepted\n");
            ++failures;
        }
    }

    /* 6. Download threshold */
    {
        if (http1_download_should_resume(49 * 1024 * 1024)) {
            fprintf(stderr, "FAIL: 49 MB should not trigger resume\n");
            ++failures;
        }
        if (!http1_download_should_resume(51ULL * 1024 * 1024)) {
            fprintf(stderr, "FAIL: 51 MB should trigger resume\n");
            ++failures;
        }
    }

    /* 7. Negotiation smoke */
    {
        const char *r = sleela_http_negotiate("1.0", "1.0, 1.1", 1);
        if (!r || strcmp(r, "1.0") != 0) {
            fprintf(stderr, "FAIL: negotiate('1.0', '1.0, 1.1') = '%s'\n",
                    r ? r : "(null)");
            ++failures;
        }
        r = sleela_http_negotiate("2.0", "1.0, 1.1", 1);
        if (!r || strcmp(r, "1.1") != 0) {
            fprintf(stderr, "FAIL: negotiate fallback = '%s'\n", r ? r : "(null)");
            ++failures;
        }
        r = sleela_http_negotiate("2.0", "1.0, 1.1", 0);
        if (r != NULL) {
            fprintf(stderr, "FAIL: negotiate no-fallback should be NULL\n");
            ++failures;
        }
    }

    /* 8. Response header builder */
    {
        http1_response resp;
        memset(&resp, 0, sizeof(resp));
        http1_response_add_header(&resp, "X-Test", "value1");
        http1_response_add_header(&resp, "X-Other", "value2");
        if (http1_response_add_header(&resp, "X-Bad", "ok\r\nInjected: yes") == 0) {
            fprintf(stderr, "FAIL: response header CRLF injection accepted\n");
            ++failures;
        }
        if (resp.header_count != 2) {
            fprintf(stderr, "FAIL: header_count = %d\n", resp.header_count);
            ++failures;
        }
    }

    if (failures == 0)
        fprintf(stderr, "http1_self_test: all tests passed\n");
    else
        fprintf(stderr, "http1_self_test: %d test(s) FAILED\n", failures);

    return failures;
}
