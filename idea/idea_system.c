/*
 * idea_system.c -- Idea System(TM) native core, C implementation.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Implements the signal-distributed System described in idea_system.h: it
 * receives an Author-controlled hex code from a known internet resource
 * (github.com), folds it into a forward, linear solution Series (<= 30h,
 * <= 9 seemly decisions), and maintains itself so that the presented State
 * IS the System.  Time is ultimate: idea_tick advances it and never reverses.
 *
 * No network I/O lives here.  Receiving the signal is routed through a
 * pluggable idea_fetch_fn so the System is deterministic and testable
 * offline; the Sleela runnable (or the host) supplies the real fetcher.
 */
/* popen/pclose and POSIX networking need the POSIX feature set exposed even
 * under strict -std=c11 -Wpedantic. */
#ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
#endif

#include "idea_system.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Networking (Option 2: HTTP in the core).  POSIX sockets for http://; the
 * system curl is used for https:// since the core carries no TLS stack. */
#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#  define IDEA_HAVE_POSIX_NET 1
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netdb.h>
#  include <unistd.h>
#else
#  define IDEA_HAVE_POSIX_NET 0
#endif

/* ---- internal limits -------------------------------------------------- */
#define IDEA_SIGNAL_CAP     4096   /* max decoded signal bytes we retain    */
#define IDEA_URL_CAP        1024
#define IDEA_OBJ_TEXT_CAP   128

struct idea_system {
    char            url[IDEA_URL_CAP];
    idea_fetch_fn   fetch;
    void           *fetch_user;

    unsigned char   signal[IDEA_SIGNAL_CAP];
    size_t          signal_len;

    /* Parsed payload header (B): what the Author's hex encodes. */
    int             payload_valid;    /* begins with 'I''D' magic            */
    int             payload_version;
    int             p_hours;          /* 0 if not supplied by payload        */
    int             p_decisions;      /* 0 if not supplied by payload        */
    int             p_depth;          /* 0 if not supplied by payload        */
    int             p_scheme;         /* -1 if not supplied by payload       */
    int             p_authored;       /* IDEA_FLAG_AUTHORED resolved         */

    /* The Series, sized/seeded from the signal. */
    int             series_built;
    long            total_seconds;     /* <= IDEA_MAX_SECONDS                */
    int             total_decisions;   /* <= IDEA_MAX_DECISIONS              */
    int             total_depth;       /* <= IDEA_MAX_SERIES_DEPTH           */

    /* Forward Time and derived position. */
    long            elapsed;           /* 0..total_seconds                   */
    int             decisions_taken;
    int             depth;
    long            last_delta;        /* used to detect seeming loop/retry  */

    idea_state_kind state;
    idea_scheme     scheme;

    char            objectives[IDEA_MAX_DECISIONS][IDEA_OBJ_TEXT_CAP];
    int             objective_count;
};

/* ---- small helpers ---------------------------------------------------- */

static int hex_nibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

/*
 * Decode a hex code into out (capacity cap).  Tolerates ASCII whitespace and
 * optional "0x"/"0X" prefixes between byte groups.  Returns decoded length,
 * or -1 if a non-hex character is found or there is a dangling nibble.
 */
static long hex_decode(const char *hex, unsigned char *out, size_t cap) {
    size_t n = 0;
    int hi = -1;
    if (!hex) return -1;
    for (const char *p = hex; *p; ++p) {
        char c = *p;
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == ',' || c == ':') {
            if (hi >= 0) return -1; /* whitespace splitting a byte */
            continue;
        }
        /* allow a 0x / 0X prefix when we are byte-aligned */
        if (hi < 0 && c == '0' && (p[1] == 'x' || p[1] == 'X')) { p++; continue; }
        int v = hex_nibble(c);
        if (v < 0) return -1;
        if (hi < 0) {
            hi = v;
        } else {
            if (n >= cap) return (long)n; /* keep what fits; signal is bounded */
            out[n++] = (unsigned char)((hi << 4) | v);
            hi = -1;
        }
    }
    if (hi >= 0) return -1; /* dangling nibble */
    return (long)n;
}

/* Fold the signal bytes into a stable 64-bit seed (order matters: linear). */
static uint64_t signal_seed(const unsigned char *b, size_t n) {
    uint64_t h = 1469598103934665603ULL; /* FNV-1a offset basis */
    for (size_t i = 0; i < n; ++i) {
        h ^= b[i];
        h *= 1099511628211ULL;
    }
    return h ? h : 1ULL;
}

/* Clamp helper. */
static long clampl(long v, long lo, long hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

/* ---- lifecycle -------------------------------------------------------- */

idea_system *idea_create(void) {
    idea_system *sys = (idea_system *)calloc(1, sizeof(*sys));
    if (!sys) return NULL;
    strncpy(sys->url, IDEA_DEFAULT_SOURCE_URL, IDEA_URL_CAP - 1);
    sys->url[IDEA_URL_CAP - 1] = '\0';
    sys->state  = IDEA_STATE_LOADING;
    sys->scheme = IDEA_SCHEME_CONCEPT;
    sys->p_scheme = -1;
    sys->p_authored = 1;
    /* Option 2: install the built-in network fetcher by default so the System
     * really receives its signal from the internet resource.  A test / offline
     * caller can override it with idea_set_fetcher. */
    sys->fetch = idea_http_fetch;
    sys->fetch_user = NULL;
    return sys;
}

void idea_destroy(idea_system *sys) {
    if (sys) free(sys);
}

idea_status idea_set_source_url(idea_system *sys, const char *url) {
    if (!sys || !url || !url[0]) return IDEA_INVALID;
    if (strlen(url) >= IDEA_URL_CAP) return IDEA_LIMIT;
    strncpy(sys->url, url, IDEA_URL_CAP - 1);
    sys->url[IDEA_URL_CAP - 1] = '\0';
    return IDEA_OK;
}

const char *idea_source_url(const idea_system *sys) {
    return sys ? sys->url : "";
}

idea_status idea_set_fetcher(idea_system *sys, idea_fetch_fn fetch, void *user) {
    if (!sys) return IDEA_INVALID;
    sys->fetch = fetch;
    sys->fetch_user = user;
    return IDEA_OK;
}

/* ---- signal input ----------------------------------------------------- */

/* Parse the Idea(TM) payload header (B) from the decoded signal bytes. */
static void parse_payload(idea_system *sys) {
    const unsigned char *b = sys->signal;
    size_t n = sys->signal_len;

    /* Defaults: legacy raw signal (authored, nothing else supplied). */
    sys->payload_valid   = 0;
    sys->payload_version = 0;
    sys->p_hours = sys->p_decisions = sys->p_depth = 0;
    sys->p_scheme = -1;
    sys->p_authored = 1;

    if (n >= IDEA_PAYLOAD_HEADER &&
        b[0] == IDEA_PAYLOAD_MAGIC0 && b[1] == IDEA_PAYLOAD_MAGIC1) {
        sys->payload_valid   = 1;
        sys->payload_version = b[2];
        sys->p_hours     = b[3];
        sys->p_decisions = b[4];
        sys->p_scheme    = b[5];
        sys->p_depth     = (int)((b[6] << 8) | b[7]);
        sys->p_authored  = (b[8] & IDEA_FLAG_AUTHORED) ? 1 : 0;
        if (sys->p_scheme < IDEA_SCHEME_CONCEPT || sys->p_scheme > IDEA_SCHEME_SOCIETY)
            sys->p_scheme = -1;
    }
}

idea_status idea_feed_hex(idea_system *sys, const char *hex) {
    if (!sys || !hex) return IDEA_INVALID;
    long n = hex_decode(hex, sys->signal, IDEA_SIGNAL_CAP);
    if (n < 0) return IDEA_INPUT_MALFORMED;
    if (n == 0) return IDEA_INPUT_MALFORMED; /* an importance must carry bytes */
    sys->signal_len = (size_t)n;
    parse_payload(sys);
    if (sys->state == IDEA_STATE_LOADING) sys->state = IDEA_STATE_RECEIVING;
    return IDEA_OK;
}

int idea_payload_valid(const idea_system *sys)   { return sys ? sys->payload_valid : 0; }
int idea_payload_version(const idea_system *sys) { return sys ? sys->payload_version : 0; }
int idea_is_authored(const idea_system *sys)     { return sys ? sys->p_authored : 1; }

idea_status idea_receive_signal(idea_system *sys) {
    char raw[IDEA_SIGNAL_CAP * 2 + 1];
    long got;
    if (!sys) return IDEA_INVALID;
    if (!sys->fetch) return IDEA_INPUT_UNAVAILABLE;
    got = sys->fetch(sys->url, raw, sizeof(raw) - 1, sys->fetch_user);
    if (got < 0) return IDEA_INPUT_UNAVAILABLE;
    raw[got] = '\0';
    return idea_feed_hex(sys, raw);
}

size_t idea_signal_len(const idea_system *sys) {
    return sys ? sys->signal_len : 0;
}

/* ---- build the Series ------------------------------------------------- */

idea_status idea_build_series(idea_system *sys) {
    uint64_t seed;
    if (!sys) return IDEA_INVALID;
    if (sys->signal_len == 0) return IDEA_INVALID; /* no signal received yet */

    seed = signal_seed(sys->signal, sys->signal_len);

    /*
     * Size the Series deterministically from the signal, within the seemly
     * design-goal bounds (law is careful: we clamp rather than reject, and
     * the clamp itself is the recorded bounded exception).
     *   - duration: 1 .. 30 hours of content
     *   - decisions: 1 .. 9 seemly, important choices
     *   - depth: 1 .. IDEA_MAX_SERIES_DEPTH (arbitrarily deep)
     */
    /*
     * If the Author's payload supplies a field (B), it is authoritative and
     * merely clamped to the seemly bounds.  Otherwise the field is derived
     * deterministically from the signal seed (legacy behaviour).
     */
    if (sys->p_hours > 0)
        sys->total_seconds = clampl((long)sys->p_hours * 3600L, 1, IDEA_MAX_SECONDS);
    else
        sys->total_seconds = clampl((long)(seed % IDEA_MAX_SECONDS) + 1, 1, IDEA_MAX_SECONDS);

    if (sys->p_decisions > 0)
        sys->total_decisions = sys->p_decisions > IDEA_MAX_DECISIONS
                             ? IDEA_MAX_DECISIONS : sys->p_decisions;
    else
        sys->total_decisions = (int)((seed >> 17) % IDEA_MAX_DECISIONS) + 1;

    if (sys->p_depth > 0)
        sys->total_depth = sys->p_depth > IDEA_MAX_SERIES_DEPTH
                         ? IDEA_MAX_SERIES_DEPTH : sys->p_depth;
    else
        sys->total_depth = (int)((seed >> 31) % IDEA_MAX_SERIES_DEPTH) + 1;

    /* The payload may also choose the organizing Scheme. */
    if (sys->p_scheme >= 0)
        sys->scheme = (idea_scheme)sys->p_scheme;

    sys->elapsed         = 0;
    sys->decisions_taken = 0;
    sys->depth           = 0;
    sys->last_delta      = 0;
    sys->series_built    = 1;
    sys->state           = IDEA_STATE_SOLVING;
    return IDEA_OK;
}

/* ---- forward Time: the single maintenance step ------------------------ */

idea_status idea_tick(idea_system *sys, long delta_seconds) {
    double frac;
    if (!sys) return IDEA_INVALID;
    if (!sys->series_built) return IDEA_INVALID;
    if (delta_seconds < 0) return IDEA_INVALID; /* Time is forward only */

    if (sys->elapsed >= sys->total_seconds) {
        sys->state = IDEA_STATE_FINAL;
        return IDEA_EXHAUSTED;
    }

    /* Advance Time; never past the end of the Series. */
    sys->elapsed = clampl(sys->elapsed + delta_seconds, 0, sys->total_seconds);
    sys->last_delta = delta_seconds;

    /* System in Maintenance of System and then State: recompute position. */
    frac = (double)sys->elapsed / (double)sys->total_seconds;

    /* Depth and decisions grow monotonically with Time (linear Series). */
    sys->depth = (int)(frac * (double)sys->total_depth);
    if (sys->depth > sys->total_depth) sys->depth = sys->total_depth;

    sys->decisions_taken = (int)(frac * (double)sys->total_decisions);
    if (sys->decisions_taken > sys->total_decisions)
        sys->decisions_taken = sys->total_decisions;

    /* Determine the presented State -- what the System presents. */
    if (sys->elapsed >= sys->total_seconds) {
        sys->state = IDEA_STATE_FINAL; /* Finality */
    } else if (delta_seconds == 0) {
        /* No Time passed: it SEEMS to loop/retry, but is consistent. */
        sys->state = IDEA_STATE_LOOPING;
    } else if (sys->decisions_taken > 0 &&
               (int)(frac * (double)sys->total_decisions * 10.0) % 10 == 0) {
        /* Landed squarely on a seemly, important decision boundary. */
        sys->state = IDEA_STATE_DECIDING;
    } else {
        sys->state = IDEA_STATE_SOLVING;
    }
    return IDEA_OK;
}

/* ---- presentation ----------------------------------------------------- */

idea_state_kind idea_present_state(const idea_system *sys) {
    return sys ? sys->state : IDEA_STATE_LOADING;
}

long idea_elapsed_seconds(const idea_system *sys) {
    return sys ? sys->elapsed : 0;
}

double idea_progress(const idea_system *sys) {
    if (!sys || sys->total_seconds <= 0) return 0.0;
    return (double)sys->elapsed / (double)sys->total_seconds;
}

int idea_decisions_taken(const idea_system *sys) {
    return sys ? sys->decisions_taken : 0;
}

int idea_series_depth(const idea_system *sys) {
    return sys ? sys->depth : 0;
}

/*
 * Not all is present/capable in a given State.  The System knows the merits,
 * calls, lengths, and signals; a capability is present only in the States
 * where it is meaningful.
 */
int idea_is_capable(const idea_system *sys, const char *capability) {
    if (!sys || !capability) return 0;

    if (strcmp(capability, "receive") == 0)
        return sys->state == IDEA_STATE_LOADING ||
               sys->state == IDEA_STATE_RECEIVING;
    if (strcmp(capability, "solve") == 0)
        return sys->series_built && sys->state != IDEA_STATE_FINAL;
    if (strcmp(capability, "decide") == 0)
        return sys->state == IDEA_STATE_DECIDING &&
               sys->decisions_taken < sys->total_decisions;
    if (strcmp(capability, "present") == 0)
        return 1; /* the System always presents its State */
    if (strcmp(capability, "retry") == 0)
        return sys->state != IDEA_STATE_FINAL; /* may seem to loop */
    if (strcmp(capability, "finalize") == 0)
        return sys->state == IDEA_STATE_FINAL;
    return 0;
}

long idea_present(const idea_system *sys, char *buf, size_t cap) {
    int n;
    if (!sys || !buf || cap == 0) return IDEA_INVALID;
    n = snprintf(buf, cap,
                 "%s [%s] t=%lds/%lds (%.1f%%) depth=%d/%d decisions=%d/%d scheme=%s",
                 IDEA_SYSTEM_NAME,
                 idea_state_name(sys->state),
                 sys->elapsed, sys->total_seconds,
                 idea_progress(sys) * 100.0,
                 sys->depth, sys->total_depth,
                 sys->decisions_taken, sys->total_decisions,
                 idea_scheme_name(sys->scheme));
    if (n < 0) return IDEA_CORRUPT;
    return (long)n;
}

/* ---- organization ----------------------------------------------------- */

idea_status idea_set_scheme(idea_system *sys, idea_scheme scheme) {
    if (!sys) return IDEA_INVALID;
    if (scheme < IDEA_SCHEME_CONCEPT || scheme > IDEA_SCHEME_SOCIETY)
        return IDEA_INVALID;
    sys->scheme = scheme;
    return IDEA_OK;
}

idea_scheme idea_scheme_of(const idea_system *sys) {
    return sys ? sys->scheme : IDEA_SCHEME_CONCEPT;
}

idea_status idea_add_objective(idea_system *sys, const char *objective) {
    if (!sys || !objective || !objective[0]) return IDEA_INVALID;
    if (sys->objective_count >= IDEA_MAX_DECISIONS) return IDEA_LIMIT;
    strncpy(sys->objectives[sys->objective_count], objective, IDEA_OBJ_TEXT_CAP - 1);
    sys->objectives[sys->objective_count][IDEA_OBJ_TEXT_CAP - 1] = '\0';
    sys->objective_count++;
    return IDEA_OK;
}

int idea_objective_count(const idea_system *sys) {
    return sys ? sys->objective_count : 0;
}

/* ---- names ------------------------------------------------------------ */

const char *idea_state_name(idea_state_kind k) {
    switch (k) {
        case IDEA_STATE_LOADING:   return "LOADING";
        case IDEA_STATE_RECEIVING: return "RECEIVING";
        case IDEA_STATE_SOLVING:   return "SOLVING";
        case IDEA_STATE_DECIDING:  return "DECIDING";
        case IDEA_STATE_LOOPING:   return "LOOPING";
        case IDEA_STATE_PRESENT:   return "PRESENT";
        case IDEA_STATE_FINAL:     return "FINAL";
        default:                   return "UNKNOWN";
    }
}

const char *idea_status_name(idea_status s) {
    switch (s) {
        case IDEA_OK:                return "OK";
        case IDEA_INVALID:           return "INVALID";
        case IDEA_LIMIT:             return "LIMIT";
        case IDEA_NOT_FOUND:         return "NOT_FOUND";
        case IDEA_INPUT_UNAVAILABLE: return "INPUT_UNAVAILABLE";
        case IDEA_INPUT_MALFORMED:   return "INPUT_MALFORMED";
        case IDEA_EXHAUSTED:         return "EXHAUSTED";
        case IDEA_CORRUPT:           return "CORRUPT";
        default:                     return "UNKNOWN";
    }
}

const char *idea_scheme_name(idea_scheme s) {
    switch (s) {
        case IDEA_SCHEME_CONCEPT:  return "Concepts";
        case IDEA_SCHEME_FINALITY: return "Finality";
        case IDEA_SCHEME_SIGNAL:   return "Signal";
        case IDEA_SCHEME_SOCIETY:  return "Society";
        default:                   return "Scheme";
    }
}

/* ---- Canvas / frame clock (Hz) ---------------------------------------- */

double idea_sim_timestep(void) {
    return 1.0 / IDEA_CANVAS_SIM_HZ;   /* 120 Hz -> ~0.008333 s */
}

int idea_fixed_steps(double frame_dt_seconds, double *accumulator) {
    const double step = idea_sim_timestep();
    int steps = 0;
    if (!accumulator) return 0;
    if (frame_dt_seconds < 0.0) frame_dt_seconds = 0.0;
    /* Clamp a long stall so a paused window can't explode the simulation,
     * matching the DRAW-API frame clock's guard. */
    if (frame_dt_seconds > 0.25) frame_dt_seconds = 0.25;
    *accumulator += frame_dt_seconds;
    while (*accumulator >= step) {
        *accumulator -= step;
        steps++;
    }
    return steps;
}

/* ---- Networking (Option 2) -------------------------------------------- */

/* Split "scheme://host[:port]/path" into parts.  Returns 0 on success. */
static int split_url(const char *url, int *is_https, char *host, size_t host_cap,
                     char *port, size_t port_cap, char *path, size_t path_cap) {
    const char *p = url, *hp, *slash, *colon;
    size_t hlen;
    if (!url) return -1;

    if (strncmp(p, "https://", 8) == 0) { *is_https = 1; p += 8; strncpy(port, "443", port_cap); }
    else if (strncmp(p, "http://", 7) == 0) { *is_https = 0; p += 7; strncpy(port, "80", port_cap); }
    else return -1;
    port[port_cap - 1] = '\0';

    hp = p;
    slash = strchr(p, '/');
    hlen = slash ? (size_t)(slash - hp) : strlen(hp);
    if (hlen >= host_cap) return -1;
    memcpy(host, hp, hlen);
    host[hlen] = '\0';

    /* explicit :port inside the host part */
    colon = strchr(host, ':');
    if (colon) {
        size_t pl = strlen(colon + 1);
        if (pl == 0 || pl >= port_cap) return -1;
        strncpy(port, colon + 1, port_cap - 1);
        port[port_cap - 1] = '\0';
        *(char *)colon = '\0';
    }

    if (slash) { strncpy(path, slash, path_cap - 1); path[path_cap - 1] = '\0'; }
    else       { strncpy(path, "/", path_cap - 1);   path[path_cap - 1] = '\0'; }
    return 0;
}

/* Fetch an http:// URL over a raw TCP socket (HTTP/1.1 GET), returning only
 * the response body.  Returns bytes written to buf, or negative idea_status. */
#if IDEA_HAVE_POSIX_NET
static long http_get_plain(const char *host, const char *port, const char *path,
                           char *buf, size_t cap) {
    struct addrinfo hints, *res = NULL, *ai;
    int fd = -1;
    char req[2048];
    int reqn;
    long total = 0;
    char *rbuf;
    size_t rcap = 1 << 16, rlen = 0;
    char *body;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, port, &hints, &res) != 0) return IDEA_INPUT_UNAVAILABLE;
    for (ai = res; ai; ai = ai->ai_next) {
        fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (fd < 0) continue;
        if (connect(fd, ai->ai_addr, ai->ai_addrlen) == 0) break;
        close(fd); fd = -1;
    }
    freeaddrinfo(res);
    if (fd < 0) return IDEA_INPUT_UNAVAILABLE;

    reqn = snprintf(req, sizeof req,
                    "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: IdeaSystem/1.0\r\n"
                    "Accept: */*\r\nConnection: close\r\n\r\n", path, host);
    if (reqn <= 0 || (size_t)reqn >= sizeof req) { close(fd); return IDEA_INVALID; }
    if (send(fd, req, (size_t)reqn, 0) != reqn) { close(fd); return IDEA_INPUT_UNAVAILABLE; }

    /* Read the whole response into a growable buffer. */
    rbuf = (char *)malloc(rcap);
    if (!rbuf) { close(fd); return IDEA_INPUT_UNAVAILABLE; }
    for (;;) {
        ssize_t got;
        if (rlen + 4096 > rcap) {
            char *nb = (char *)realloc(rbuf, rcap * 2);
            if (!nb) { free(rbuf); close(fd); return IDEA_INPUT_UNAVAILABLE; }
            rbuf = nb; rcap *= 2;
        }
        got = recv(fd, rbuf + rlen, 4096, 0);
        if (got < 0) { free(rbuf); close(fd); return IDEA_INPUT_UNAVAILABLE; }
        if (got == 0) break;
        rlen += (size_t)got;
        if (rlen > (1u << 22)) break; /* 4 MiB cap */
    }
    close(fd);

    /* Skip HTTP headers; body follows the first blank line. */
    rbuf[rlen < rcap ? rlen : rcap - 1] = '\0';
    body = strstr(rbuf, "\r\n\r\n");
    body = body ? body + 4 : rbuf;
    total = (long)strlen(body);
    if ((size_t)total > cap) total = (long)cap;
    memcpy(buf, body, (size_t)total);
    free(rbuf);
    return total;
}
#endif

/* Fetch an https:// URL by shelling out to the system curl (the core has no
 * TLS stack).  Returns bytes written, or negative idea_status. */
static long http_get_curl(const char *url, char *buf, size_t cap) {
    char cmd[2048];
    FILE *fp;
    size_t n;
    int c;
    /* Validate the URL characters to keep the command injection-safe: only a
     * conservative URL charset is allowed. */
    for (const char *q = url; *q; ++q) {
        if (!((*q >= 'A' && *q <= 'Z') || (*q >= 'a' && *q <= 'z') ||
              (*q >= '0' && *q <= '9') || strchr(":/._~%?#=&+-", *q))) {
            return IDEA_INVALID;
        }
    }
    c = snprintf(cmd, sizeof cmd,
                 "curl -fsSL --max-time 20 --max-filesize 4194304 \"%s\" 2>/dev/null", url);
    if (c <= 0 || (size_t)c >= sizeof cmd) return IDEA_INVALID;
    fp = popen(cmd, "r");
    if (!fp) return IDEA_INPUT_UNAVAILABLE;
    n = fread(buf, 1, cap, fp);
    if (pclose(fp) != 0 && n == 0) return IDEA_INPUT_UNAVAILABLE;
    return (long)n;
}

long idea_http_fetch(const char *url, char *buf, size_t cap, void *user) {
    int is_https = 0;
    char host[256], port[16], path[1024];
    (void)user;
    if (!url || !buf || cap == 0) return IDEA_INVALID;
    if (split_url(url, &is_https, host, sizeof host, port, sizeof port,
                  path, sizeof path) != 0)
        return IDEA_INVALID;
    if (is_https) return http_get_curl(url, buf, cap);
#if IDEA_HAVE_POSIX_NET
    return http_get_plain(host, port, path, buf, cap);
#else
    return IDEA_INPUT_UNAVAILABLE;
#endif
}
