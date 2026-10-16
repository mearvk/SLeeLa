/* http3_server.c — SLeeLa HTTP 3.0 portable server implementation
 *
 * Platforms: Linux, macOS, Windows 10+
 *
 * Transport: HTTP/1.x over sleela_net portable sockets.
 * Application: SLeeLa HTTP 3.0 envelope/pipeline core — including the
 * integrity gate (keyed-MAC DIGEST, INTACTX tamper check, NONCE replay
 * guard) — rides inside the HTTP request body at /__sleela/h3 (POST).
 */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include "http3_server.h"
#include "http_negotiation.h"
#include "../impl/core/sleela_net.h"
#include "../preferred-routers/preferred_router.h"
#include "../resolver/resolver.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <poll.h>
#include <unistd.h>
#endif

/* ── signal-safe stop flag ──────────────────────────────────────────── */

static volatile int g_stop = 0;

void h3s_server_stop(void) { g_stop = 1; }

#ifndef _WIN32
static void h3s_signal_handler(int sig) { (void)sig; g_stop = 1; }
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
    case 400: return "Bad Request";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 413: return "Content Too Large";
    case 500: return "Internal Server Error";
    default:  return "Error";
    }
}

/* ── request parsing ────────────────────────────────────────────────── */

static int recv_headers(SLNetHandle fd, char *buf, size_t cap)
{
    size_t total = 0;
    while (total < cap) {
        SLNetCount n = slnet_read(fd, buf + total, cap - total);
        if (n <= 0) return total > 0 ? (int)total : -1;
        total += (size_t)n;
        if (total >= 4) {
            const char *end = buf + (total >= H3S_MAX_HEADER
                                     ? H3S_MAX_HEADER : total);
            for (const char *p = buf + (total - n > 3 ? total - n - 3 : 0);
                 p + 3 < end; ++p) {
                if (p[0] == '\r' && p[1] == '\n' &&
                    p[2] == '\r' && p[3] == '\n')
                    return (int)total;
            }
        }
    }
    return -1;
}

static int find_header(const h3s_request *req, const char *name)
{
    for (int i = 0; i < req->header_count; ++i)
        if (strcmp(req->headers[i].name, name) == 0) return i;
    return -1;
}

static const char *get_header(const h3s_request *req, const char *name)
{
    int i = find_header(req, name);
    return i >= 0 ? req->headers[i].value : NULL;
}

static int parse_request(const char *raw, size_t raw_len,
                         h3s_request *req,
                         const char **body_start, size_t *header_bytes)
{
    const char *p = raw, *end = raw + raw_len, *line_end;
    memset(req, 0, sizeof(*req));

    line_end = strstr(p, "\r\n");
    if (!line_end) return -1;

    { const char *sp = memchr(p, ' ', (size_t)(line_end - p));
      if (!sp || (size_t)(sp - p) >= sizeof(req->method)) return -1;
      memcpy(req->method, p, (size_t)(sp - p)); p = sp + 1; }
    { const char *sp = memchr(p, ' ', (size_t)(line_end - p));
      if (!sp || (size_t)(sp - p) >= sizeof(req->uri)) return -1;
      memcpy(req->uri, p, (size_t)(sp - p)); p = sp + 1; }
    { size_t vl = (size_t)(line_end - p);
      if (vl >= sizeof(req->version)) return -1;
      memcpy(req->version, p, vl); }

    p = line_end + 2;

    while (p + 1 < end && !(p[0] == '\r' && p[1] == '\n')) {
        line_end = strstr(p, "\r\n");
        if (!line_end) break;
        const char *colon = memchr(p, ':', (size_t)(line_end - p));
        if (!colon || colon == p) return -1;
        if (req->header_count >= H3S_MAX_HEADERS) return -1;
        h3s_header *h = &req->headers[req->header_count];
        size_t nl = (size_t)(colon - p);
        if (nl >= sizeof(h->name)) return -1;
        memcpy(h->name, p, nl); h->name[nl] = '\0'; str_lower(h->name);
        const char *val = str_trim_left(colon + 1);
        size_t vl = (size_t)(line_end - val);
        if (vl >= sizeof(h->value)) return -1;
        memcpy(h->value, val, vl); h->value[vl] = '\0';
        ++req->header_count; p = line_end + 2;
    }
    if (p + 1 < end && p[0] == '\r' && p[1] == '\n') p += 2;
    *header_bytes = (size_t)(p - raw); *body_start = p;

    { const char *v;
      v = get_header(req, "x-sleela-version");
      if (v) snprintf(req->sleela_version, sizeof(req->sleela_version), "%s", v);
      v = get_header(req, "x-sleela-accept");
      if (v) snprintf(req->sleela_accept, sizeof(req->sleela_accept), "%s", v);
      v = get_header(req, "x-sleela-port");
      if (v) snprintf(req->sleela_port, sizeof(req->sleela_port), "%s", v);
      v = get_header(req, "x-sleela-service-id");
      if (v) snprintf(req->service_id, sizeof(req->service_id), "%s", v);
      v = get_header(req, "x-sleela-op-id");
      if (v) snprintf(req->op_id, sizeof(req->op_id), "%s", v);
    }
    return 0;
}

/* ── response serialisation ─────────────────────────────────────────── */

int h3s_response_add_header(h3s_response *resp,
                            const char *name, const char *value)
{
    if (!resp || !name || !value) return -1;
    if (resp->header_count >= H3S_MAX_HEADERS) return -1;
    h3s_header *h = &resp->headers[resp->header_count++];
    snprintf(h->name,  sizeof(h->name),  "%s", name);
    snprintf(h->value, sizeof(h->value), "%s", value);
    return 0;
}

static int send_all(SLNetHandle fd, const char *data, size_t len)
{
    while (len > 0) {
        SLNetCount n = slnet_write(fd, data, len);
        if (n <= 0) return -1;
        data += n; len -= (size_t)n;
    }
    return 0;
}

static int send_response(SLNetHandle fd, const h3s_request *req,
                         const h3s_response *resp, const char *server_name)
{
    char buf[H3S_MAX_HEADER];
    int off = 0;
    const char *reason = resp->reason ? resp->reason
                                      : reason_phrase(resp->status_code);

    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "%s %d %s\r\n",
                    req->version[0] ? req->version : "HTTP/1.0",
                    resp->status_code, reason);
    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "Server: %s\r\n", server_name);
    if (resp->content_type)
        off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                        "Content-Type: %s\r\n", resp->content_type);
    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "Content-Length: %zu\r\n", resp->body_length);
    for (int i = 0; i < resp->header_count; ++i)
        off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                        "%s: %s\r\n",
                        resp->headers[i].name, resp->headers[i].value);
    off += snprintf(buf + off, sizeof(buf) - (size_t)off,
                    "X-SLeeLa-Version: 3.0\r\n"
                    "X-SLeeLa-Accept: 3.0, 2.1, 2.0, 1.1, 1.0\r\n"
                    "Connection: close\r\n\r\n");
    if (off <= 0 || (size_t)off >= sizeof(buf)) return -1;
    if (send_all(fd, buf, (size_t)off) != 0) return -1;
    if (resp->body && resp->body_length > 0)
        return send_all(fd, resp->body, resp->body_length);
    return 0;
}

/* ── download helpers ───────────────────────────────────────────────── */

int h3s_download_should_resume(uint64_t cl) { return cl > H3S_DOWNLOAD_THRESHOLD; }

int h3s_download_serialise(const h3s_download_state *st,
                           char *buf, size_t cap)
{
    if (!st || !buf || cap < 512) return -1;
    int n = snprintf(buf, cap, "%s | %s | %s | %s | %llu | %llu | %llu",
                     st->session_id, st->datetime, st->file_id, st->file_name,
                     (unsigned long long)st->index,
                     (unsigned long long)st->offset,
                     (unsigned long long)st->total_size);
    return (n > 0 && (size_t)n < cap) ? 0 : -1;
}

int h3s_download_parse(const char *line, h3s_download_state *out)
{
    if (!line || !out) return -1;
    memset(out, 0, sizeof(*out));
    char copy[2048]; snprintf(copy, sizeof(copy), "%s", line);
    char *fields[7]; int count = 0;
    char *tok = strtok(copy, "|");
    while (tok && count < 7) {
        while (*tok == ' ' || *tok == '\t') ++tok;
        size_t len = strlen(tok);
        while (len > 0 && (tok[len-1] == ' ' || tok[len-1] == '\t'))
            tok[--len] = '\0';
        fields[count++] = tok; tok = strtok(NULL, "|");
    }
    if (count != 7) return -1;
    snprintf(out->session_id, sizeof(out->session_id), "%s", fields[0]);
    snprintf(out->datetime,   sizeof(out->datetime),   "%s", fields[1]);
    snprintf(out->file_id,    sizeof(out->file_id),    "%s", fields[2]);
    snprintf(out->file_name,  sizeof(out->file_name),  "%s", fields[3]);
    out->index      = (uint64_t)strtoull(fields[4], NULL, 10);
    out->offset     = (uint64_t)strtoull(fields[5], NULL, 10);
    out->total_size = (uint64_t)strtoull(fields[6], NULL, 10);
    return 0;
}

/* ── built-in route handlers ────────────────────────────────────────── */

static int builtin_health(const h3s_request *req, h3s_response *resp)
{
    (void)req;
    static const char body[] = "SLeeLa HTTP/3.0 OK\n";
    resp->status_code = 200; resp->content_type = "text/plain; charset=utf-8";
    resp->body = body; resp->body_length = sizeof(body) - 1;
    return 0;
}

static int builtin_status(const h3s_request *req, h3s_response *resp,
                          const http3_pipeline_t *pipe)
{
    (void)req;
    static char buf[1024];
    int n = snprintf(buf, sizeof(buf),
        "{\"server\":\"SLeeLa\","
         "\"http_generation\":\"3.0\","
         "\"vm_generation\":3,"
         "\"vm_name\":\"Specialist\","
         "\"vm_path\":\"/3\","
         "\"platform\":\"%s\","
         "\"route_data_schema\":\"1.0\","
         "\"envelope_version\":%u,"
         "\"naming\":true,"
         "\"pipeline\":true,"
         "\"integrity_gate\":true,"
         "\"digest\":\"SipHash-2-4\","
         "\"intactx\":true,"
         "\"nonce_replay_guard\":true,"
         "\"basket\":true,"
         "\"requests_handled\":%llu,"
         "\"digest_rejects\":%llu,"
         "\"tamper_resets\":%llu,"
         "\"replays_rejected\":%llu,"
         "\"carrier_certainty\":%.3f,"
         "\"lower_http_compatibility\":true}\n",
        slnet_platform_name(), HTTP3_ENVELOPE_VERSION,
        pipe ? (unsigned long long)pipe->requests_handled : 0ULL,
        pipe ? (unsigned long long)pipe->digest_rejects : 0ULL,
        pipe ? (unsigned long long)pipe->tamper_resets : 0ULL,
        pipe ? (unsigned long long)pipe->replays_rejected : 0ULL,
        pipe ? http3_pipeline_carrier_certainty(pipe) : 0.0);
    resp->status_code = 200; resp->content_type = "application/json";
    resp->body = buf; resp->body_length = (size_t)n;
    return 0;
}

static int builtin_negotiate(const h3s_request *req, h3s_response *resp,
                             int allow_fallback)
{
    const char *negotiated = sleela_http_negotiate(
        req->sleela_version[0] ? req->sleela_version : NULL,
        req->sleela_accept[0]  ? req->sleela_accept  : NULL,
        allow_fallback);
    static char buf[256]; int n;
    if (negotiated) {
        n = snprintf(buf, sizeof(buf),
                     "{\"negotiated\":\"%s\",\"ok\":true}\n", negotiated);
        resp->status_code = 200;
    } else {
        n = snprintf(buf, sizeof(buf), "{\"negotiated\":null,\"ok\":false}\n");
        resp->status_code = 400;
    }
    resp->content_type = "application/json";
    resp->body = buf; resp->body_length = (size_t)n;
    return 0;
}

/* ── H3 envelope endpoint ───────────────────────────────────────────── */

static int builtin_h3_envelope(const h3s_request *req, h3s_response *resp,
                               http3_pipeline_t *pipeline)
{
    if (!req->body || req->body_length == 0) {
        resp->status_code = 400; resp->content_type = "text/plain";
        resp->body = "Envelope body required\n"; resp->body_length = 22;
        return 0;
    }
    static char result[HTTP3_ENVELOPE_MAX_PAYLOAD + 512];
    size_t written = 0;
    int rc = http3_pipeline_handle_wire(pipeline,
                                         (const uint8_t *)req->body,
                                         req->body_length,
                                         result, sizeof(result), &written);
    if (rc != 0) {
        resp->status_code = 500; resp->content_type = "text/plain";
        resp->body = "Pipeline error\n"; resp->body_length = 15;
        return 0;
    }
    resp->status_code = 200; resp->content_type = "application/x-sleela-h3";
    resp->body = result; resp->body_length = written;
    return 0;
}

/* ── connection handler ─────────────────────────────────────────────── */

static void handle_connection(SLNetHandle fd, const h3s_server_config *cfg)
{
    size_t raw_cap = H3S_MAX_HEADER + 4096;
    char *raw = malloc(raw_cap);
    if (!raw) { slnet_close(fd); return; }

    int got = recv_headers(fd, raw, raw_cap);
    if (got <= 0) { free(raw); slnet_close(fd); return; }

    h3s_request req; h3s_response resp;
    const char *body_start = NULL; size_t header_bytes = 0;
    memset(&resp, 0, sizeof(resp)); resp.status_code = 200;

    if (parse_request(raw, (size_t)got, &req, &body_start, &header_bytes) != 0) {
        resp.status_code = 400; resp.content_type = "text/plain";
        resp.body = "Bad Request\n"; resp.body_length = 12;
        h3s_request bad; memset(&bad, 0, sizeof(bad));
        memcpy(bad.version, "HTTP/1.0", 9);
        send_response(fd, &bad, &resp, cfg->server_name);
        free(raw); slnet_close(fd); return;
    }

    /* Read body if Content-Length present. */
    {
        const char *cl = get_header(&req, "content-length");
        if (cl) {
            size_t content_len = (size_t)strtoull(cl, NULL, 10);
            if (content_len > H3S_MAX_BODY) {
                resp.status_code = 413; resp.content_type = "text/plain";
                resp.body = "Content Too Large\n"; resp.body_length = 18;
                send_response(fd, &req, &resp, cfg->server_name);
                free(raw); slnet_close(fd); return;
            }
            size_t already = (size_t)got - header_bytes;
            req.body = malloc(content_len + 1);
            if (!req.body) {
                resp.status_code = 500; resp.content_type = "text/plain";
                resp.body = "Internal Server Error\n"; resp.body_length = 22;
                send_response(fd, &req, &resp, cfg->server_name);
                free(raw); slnet_close(fd); return;
            }
            if (already > 0 && already <= content_len)
                memcpy(req.body, body_start, already);
            size_t remaining = already < content_len ? content_len - already : 0;
            size_t filled = already < content_len ? already : content_len;
            while (remaining > 0) {
                SLNetCount n = slnet_read(fd, req.body + filled, remaining);
                if (n <= 0) break;
                filled += (size_t)n; remaining -= (size_t)n;
            }
            req.body_length = filled; req.body[filled] = '\0';
        }
    }
    free(raw); raw = NULL;

    /* ---- built-in routes ---- */
    int handled = 0;
    if (strcmp(req.uri, "/__sleela/health") == 0) {
        builtin_health(&req, &resp); handled = 1;
    } else if (strcmp(req.uri, "/__sleela/status") == 0) {
        builtin_status(&req, &resp, cfg->pipeline); handled = 1;
    } else if (strcmp(req.uri, "/__sleela/negotiate") == 0) {
        builtin_negotiate(&req, &resp, cfg->allow_negotiation_fallback);
        handled = 1;
    } else if (strcmp(req.uri, "/__sleela/h3") == 0 &&
               strcmp(req.method, "POST") == 0 && cfg->pipeline) {
        builtin_h3_envelope(&req, &resp, cfg->pipeline); handled = 1;
    }

    if (!handled && cfg->handler) {
        int rc = cfg->handler(&req, &resp, cfg->user_data);
        if (rc != 0) {
            resp.status_code = 500; resp.content_type = "text/plain";
            resp.body = "Internal Server Error\n"; resp.body_length = 22;
        }
    } else if (!handled) {
        resp.status_code = 404; resp.content_type = "text/plain";
        resp.body = "Not Found\n"; resp.body_length = 10;
    }

    send_response(fd, &req, &resp, cfg->server_name);
    free(req.body); slnet_close(fd);
}

/* ── portable poll ──────────────────────────────────────────────────── */

static int portable_poll_readable(SLNetHandle fd, int timeout_ms)
{
#ifdef _WIN32
    WSAPOLLFD pfd; pfd.fd = (SOCKET)fd; pfd.events = POLLIN; pfd.revents = 0;
    return WSAPoll(&pfd, 1, timeout_ms) > 0 && (pfd.revents & POLLIN);
#else
    struct pollfd pfd; pfd.fd = (int)fd; pfd.events = POLLIN; pfd.revents = 0;
    return poll(&pfd, 1, timeout_ms) > 0 && (pfd.revents & POLLIN);
#endif
}

/* ── server entry point ─────────────────────────────────────────────── */

void h3s_server_config_init(h3s_server_config *cfg)
{
    if (!cfg) return;
    memset(cfg, 0, sizeof(*cfg));
    cfg->port        = 8082;
    cfg->backlog     = 128;
    cfg->server_name = "SLeeLa-HTTP/3.0";
    cfg->allow_negotiation_fallback = 1;
}

int h3s_server_run(const h3s_server_config *cfg)
{
    if (!cfg) return -1;
    g_stop = 0;

    if (slnet_startup() != 0) {
        fprintf(stderr, "h3s: slnet_startup failed\n"); return -1;
    }
    if (!sleela_preferred_router_packet_policy("HTTP/3.0", 1))
        fprintf(stderr, "h3s: preferred-router policy unavailable, "
                        "continuing with built-in defaults\n");
    if (!sleela_resolver_event_policy("HTTP/3.0", 0))
        fprintf(stderr, "h3s: resolver event policy check returned false, "
                        "continuing\n");
    sleela_preferred_router_startup(NULL, "HTTP/3.0");

    SLNetHandle listener = slnet_listen(cfg->port, cfg->backlog);
    if (!slnet_valid(listener)) {
        fprintf(stderr, "h3s: failed to listen on port %u\n",
                (unsigned)cfg->port);
        slnet_shutdown(); return -1;
    }
    fprintf(stderr, "SLeeLa HTTP/3.0 listening on port %u  [%s]\n",
            (unsigned)cfg->port, slnet_platform_name());

#ifndef _WIN32
    signal(SIGINT,  h3s_signal_handler);
    signal(SIGTERM, h3s_signal_handler);
#endif

    while (!g_stop) {
        if (!portable_poll_readable(listener, 500)) continue;
        SLNetHandle client = slnet_accept(listener);
        if (!slnet_valid(client)) continue;
        handle_connection(client, cfg);
    }

    slnet_close(listener); slnet_shutdown();
    fprintf(stderr, "SLeeLa HTTP/3.0 stopped.\n");
    return 0;
}

/* ── test echo handler ──────────────────────────────────────────────── */

static http3_status_t test_echo_handler(const uint8_t *payload,
                                        size_t         payload_len,
                                        uint8_t       *result,
                                        size_t         result_cap,
                                        size_t        *result_len,
                                        void          *ctx)
{
    (void)ctx;
    size_t n = payload_len < result_cap ? payload_len : result_cap;
    memcpy(result, payload, n);
    *result_len = n;
    return HTTP3_STATUS_OK;
}

/* ── self-test ──────────────────────────────────────────────────────── */

int h3s_self_test(void)
{
    int failures = 0;

    /* 1. HTTP request parse */
    {
        const char raw[] =
            "POST /__sleela/h3 HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 0\r\n"
            "X-SLeeLa-Version: 3.0\r\n"
            "X-SLeeLa-Accept: 3.0, 2.1, 1.1, 1.0\r\n"
            "\r\n";
        h3s_request req; const char *bs; size_t hb;
        if (parse_request(raw, sizeof(raw)-1, &req, &bs, &hb) != 0) {
            fprintf(stderr, "FAIL: HTTP parse\n"); ++failures;
        } else if (strcmp(req.sleela_version, "3.0") != 0) {
            fprintf(stderr, "FAIL: sleela_version = '%s'\n",
                    req.sleela_version); ++failures;
        }
    }

    /* 2. Envelope textual round-trip with MAC */
    {
        const uint8_t key[HTTP3_MAC_KEY_BYTES] = {
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        const uint8_t payload[] = "test-payload";
        http3_envelope_t env, env2;

        if (http3_envelope_init(&env, 42, 7, 123ULL, HTTP3_FLAG_NONE,
                                1, 100, key, payload,
                                sizeof(payload)-1) != 0) {
            fprintf(stderr, "FAIL: envelope_init\n"); ++failures;
        } else {
            if (!http3_envelope_verify_digest(&env, key)) {
                fprintf(stderr, "FAIL: digest verify\n"); ++failures;
            }
            char buf[1024]; size_t written;
            if (http3_envelope_pack_text(&env, buf, sizeof(buf),
                                         &written) != 0) {
                fprintf(stderr, "FAIL: pack_text\n"); ++failures;
            } else if (http3_envelope_unpack_text(buf, written, &env2) != 0) {
                fprintf(stderr, "FAIL: unpack_text\n"); ++failures;
            } else {
                if (env2.service_id != 42 || env2.op_id != 7 ||
                    env2.request_id != 123 || env2.nonce != 1 ||
                    env2.payload_len != sizeof(payload)-1) {
                    fprintf(stderr, "FAIL: text round-trip\n"); ++failures;
                }
                if (!http3_envelope_verify_digest(&env2, key)) {
                    fprintf(stderr, "FAIL: text digest verify\n"); ++failures;
                }
            }
        }
    }

    /* 3. Envelope binary round-trip */
    {
        const uint8_t key[HTTP3_MAC_KEY_BYTES] = {
            0xA,0xB,0xC,0xD,0xE,0xF,1,2,3,4,5,6,7,8,9,0};
        const uint8_t payload[] = "binary";
        http3_envelope_t env, env2;
        http3_envelope_init(&env, 100, 3, 999ULL, HTTP3_FLAG_BINARY,
                            5, 200, key, payload, sizeof(payload)-1);
        uint8_t buf[512]; size_t written;
        if (http3_envelope_pack_binary(&env, buf, sizeof(buf),
                                        &written) != 0) {
            fprintf(stderr, "FAIL: binary pack\n"); ++failures;
        } else if (http3_envelope_unpack_binary(buf, written, &env2) != 0) {
            fprintf(stderr, "FAIL: binary unpack\n"); ++failures;
        } else if (env2.service_id != 100 || env2.nonce != 5) {
            fprintf(stderr, "FAIL: binary round-trip\n"); ++failures;
        }
    }

    /* 4. Pipeline register + dispatch with integrity */
    {
        const uint8_t key[HTTP3_MAC_KEY_BYTES] = {
            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        http3_pipeline_t pipe;
        http3_pipeline_init(&pipe);
        http3_pipeline_set_mac_key(&pipe, key);
        http3_pipeline_reset_replay_window(&pipe, 0);

        uint32_t svc_id = http3_pipeline_register_service(&pipe, "echo", NULL);
        uint32_t op_id  = http3_pipeline_register_op(&pipe, "echo", "echo",
                                                     HTTP3_RETRY_READ,
                                                     test_echo_handler);
        if (svc_id == 0 || op_id == 0) {
            fprintf(stderr, "FAIL: register svc=%u op=%u\n",
                    svc_id, op_id); ++failures;
        } else {
            /* Good packet. */
            const uint8_t payload[] = "ping";
            http3_envelope_t env;
            http3_envelope_init(&env, svc_id, op_id, 42ULL,
                                HTTP3_FLAG_NONE, 1, 0, key,
                                payload, sizeof(payload)-1);
            http3_response_t resp;
            if (http3_pipeline_dispatch(&pipe, &env, &resp) != 0) {
                fprintf(stderr, "FAIL: dispatch\n"); ++failures;
            } else if (resp.status != HTTP3_STATUS_OK ||
                       resp.request_id != 42 ||
                       resp.result_len != 4 ||
                       memcmp(resp.result, "ping", 4) != 0) {
                fprintf(stderr, "FAIL: dispatch result\n"); ++failures;
            }

            /* Wire round-trip through pipeline. */
            char wire[1024]; size_t wire_len;
            http3_envelope_pack_text(&env, wire, sizeof(wire), &wire_len);
            char out[1024]; size_t out_len;
            if (http3_pipeline_handle_wire(&pipe,
                    (const uint8_t *)wire, wire_len,
                    out, sizeof(out), &out_len) != 0) {
                fprintf(stderr, "FAIL: handle_wire\n"); ++failures;
            } else {
                http3_response_t pr;
                if (http3_response_unpack_text(out, out_len, &pr) != 0) {
                    fprintf(stderr, "FAIL: response unpack\n"); ++failures;
                } else if (pr.status != HTTP3_STATUS_OK) {
                    fprintf(stderr, "FAIL: wire status = %d (%s)\n",
                            (int)pr.status, http3_status_name(pr.status));
                    ++failures;
                }
            }
        }

        /* Retry class query. */
        if (http3_pipeline_retry_class(&pipe, svc_id, op_id)
            != HTTP3_RETRY_READ) {
            fprintf(stderr, "FAIL: retry_class\n"); ++failures;
        }
    }

    /* 5. Download state round-trip */
    {
        h3s_download_state st = {0};
        snprintf(st.session_id, sizeof(st.session_id), "h3-sess-001");
        snprintf(st.datetime,   sizeof(st.datetime),   "2026-10-15T00:00:00Z");
        snprintf(st.file_id,    sizeof(st.file_id),    "file-h3");
        snprintf(st.file_name,  sizeof(st.file_name),  "huge.dat");
        st.index = 7; st.offset = 524288000ULL; st.total_size = 1048576000ULL;
        char buf[512];
        if (h3s_download_serialise(&st, buf, sizeof(buf)) != 0) {
            fprintf(stderr, "FAIL: download serialise\n"); ++failures;
        } else {
            h3s_download_state p2;
            if (h3s_download_parse(buf, &p2) != 0 || p2.index != 7 ||
                p2.offset != 524288000ULL) {
                fprintf(stderr, "FAIL: download round-trip\n"); ++failures;
            }
        }
    }

    /* 6. Negotiation */
    {
        const char *r = sleela_http_negotiate("3.0", "3.0, 1.1", 1);
        if (!r || strcmp(r, "3.0") != 0) {
            fprintf(stderr, "FAIL: negotiate 3.0\n"); ++failures;
        }
        r = sleela_http_negotiate("4.0", "3.0, 1.1", 1);
        if (!r || strcmp(r, "1.1") != 0) {
            fprintf(stderr, "FAIL: negotiate fallback\n"); ++failures;
        }
    }

    if (failures == 0)
        fprintf(stderr, "h3s_self_test: all tests passed\n");
    else
        fprintf(stderr, "h3s_self_test: %d test(s) FAILED\n", failures);
    return failures;
}
