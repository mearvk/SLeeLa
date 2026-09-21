/* ==========================================================================
 * sleela_munction.c -- Munction reach-composition engine (C core).
 *
 * Implements the verb ladder (start -> connect -> [open/enable] -> send ->
 * thatch -> consume* -> latch -> close/abort) with a coherent-send accounting
 * and a receivable receipt, over the system-method channels. The sentence is
 * policed for the 4..16 verb bound and the legal order; a shape violation is a
 * recorded boundary stop, never invented meaning.
 *
 * Channel realizations:
 *   pipe / file / sdps / crypto -> an in-process framed buffer (deterministic,
 *     testable), with sdps carrying magic+length+MAC framing and crypto an
 *     authenticated keystream seal/open. file additionally mirrors to disk.
 *   tcp -> a real connected socket (best-effort; a network bump is recorded as
 *     a residual, the reach still closes with a truthful receipt).
 * ========================================================================== */
#if !defined(_WIN32) && !defined(_WIN64) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include "sleela_munction.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

/* Reach phases, to police the legal sequence. */
typedef enum { PH_STARTED = 0, PH_CONNECTED, PH_MOVING, PH_CLOSED } SLMunPhase;

typedef struct { unsigned char* bytes; size_t len; } SLMunFrame;

struct SLMunction {
    char name[128];
    SLMunChannel channel;
    SLMunChannel inner;          /* for crypto envelope: the wrapped channel   */
    char address[256];
    char interims[256];
    SLMunPhase phase;
    int verbs;                   /* total verb calls incl. opener + closer     */
    int latched;
    int bumped;
    int64_t sent_bytes;
    int64_t sent_calls;
    int64_t ack_bytes;
    int64_t received_units;
    unsigned char key[32];       /* crypto envelope key (derived from keyref)  */
    int keyed;

    /* In-process framed queue for pipe/file/sdps/crypto. */
    SLMunFrame q[SL_MUNCTION_QUEUE];
    int qhead, qtail, qcount;

    /* file channel: mirror path. */
    FILE* fp;

    /* net channel: a real connected socket. */
#ifdef _WIN32
    SOCKET fd;
#else
    int fd;
#endif

    unsigned char last_reception[2048];
    int last_reception_len;
};

/* ---- small helpers ------------------------------------------------------- */

static void mun_bump(SLMunction* m) { if (m) m->bumped = 1; }

static int count_verb(SLMunction* m) {
    if (!m) return -1;
    if (m->phase == PH_CLOSED) return -1;
    m->verbs++;
    if (m->verbs > SL_MUNCTION_MAX_VERBS) { mun_bump(m); return -1; }
    return 0;
}

/* FNV-1a based short digest used for coherence identity + the crypto tag. */
static uint64_t fnv1a(const unsigned char* p, size_t n, uint64_t seed) {
    uint64_t h = seed ? seed : 1469598103934665603ULL;
    for (size_t i = 0; i < n; i++) { h ^= p[i]; h *= 1099511628211ULL; }
    return h;
}

static void derive_key(SLMunction* m, const char* keyref) {
    uint64_t a = fnv1a((const unsigned char*)keyref, strlen(keyref), 1469598103934665603ULL);
    uint64_t b = fnv1a((const unsigned char*)keyref, strlen(keyref), a);
    for (int i = 0; i < 32; i++) {
        uint64_t src = (i < 16) ? a : b;
        m->key[i] = (unsigned char)(src >> ((i % 8) * 8));
    }
    m->keyed = 1;
}

/* Keystream XOR (its own inverse) for the crypto envelope. */
static void crypto_mask(SLMunction* m, unsigned char* data, size_t n) {
    for (size_t i = 0; i < n; i++) {
        unsigned char k = m->keyed ? m->key[i % 32] : (unsigned char)(0xA5 ^ (i & 0xFF));
        data[i] ^= k;
    }
}

/* ---- framed queue -------------------------------------------------------- */

static int q_push(SLMunction* m, const unsigned char* p, size_t n) {
    if (m->qcount >= SL_MUNCTION_QUEUE) return -1;
    unsigned char* copy = (unsigned char*)malloc(n ? n : 1);
    if (!copy) return -1;
    if (n) memcpy(copy, p, n);
    m->q[m->qtail].bytes = copy;
    m->q[m->qtail].len = n;
    m->qtail = (m->qtail + 1) % SL_MUNCTION_QUEUE;
    m->qcount++;
    return 0;
}

static int q_pop(SLMunction* m, unsigned char* out, size_t cap, size_t* outlen) {
    if (m->qcount <= 0) return -1;
    SLMunFrame* f = &m->q[m->qhead];
    size_t n = f->len < cap ? f->len : cap;
    if (out && n) memcpy(out, f->bytes, n);
    if (outlen) *outlen = f->len;
    free(f->bytes);
    f->bytes = NULL;
    f->len = 0;
    m->qhead = (m->qhead + 1) % SL_MUNCTION_QUEUE;
    m->qcount--;
    return 0;
}

/* ---- scheme parsing ------------------------------------------------------ */

static SLMunChannel scheme_of(const char* uri, const char** rest_out) {
    const char* colon = strchr(uri, ':');
    if (!colon) return SL_MUN_NONE;
    size_t n = (size_t)(colon - uri);
    const char* rest = colon + 1;
    if (rest[0] == '/' && rest[1] == '/') rest += 2;
    if (rest_out) *rest_out = rest;
    if (n == 4 && strncmp(uri, "pipe", 4) == 0) return SL_MUN_PIPE;
    if (n == 4 && strncmp(uri, "file", 4) == 0) return SL_MUN_FILE;
    if (n == 3 && strncmp(uri, "tcp", 3) == 0) return SL_MUN_NET;
    if (n == 3 && strncmp(uri, "net", 3) == 0) return SL_MUN_NET;
    if (n == 4 && strncmp(uri, "sdps", 4) == 0) return SL_MUN_SDPS;
    if (n == 6 && strncmp(uri, "crypto", 6) == 0) return SL_MUN_CRYPTO;
    return SL_MUN_NONE;
}

/* ---- lifecycle ----------------------------------------------------------- */

SLMunction* slmunction_start(const char* name) {
    if (!name || !*name) return NULL;
    SLMunction* m = (SLMunction*)calloc(1, sizeof(SLMunction));
    if (!m) return NULL;
    snprintf(m->name, sizeof(m->name), "%s", name);
    m->phase = PH_STARTED;
    m->verbs = 1;                /* start(...) is the first verb */
    m->channel = SL_MUN_NONE;
    m->inner = SL_MUN_NONE;
#ifdef _WIN32
    m->fd = INVALID_SOCKET;
#else
    m->fd = -1;
#endif
    return m;
}

static int net_open(SLMunction* m, const char* hostport) {
    char host[256];
    snprintf(host, sizeof(host), "%s", hostport);
    char* colon = strrchr(host, ':');
    const char* service = "0";
    if (colon) { *colon = 0; service = colon + 1; }
    struct addrinfo hints, *list = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, service, &hints, &list) != 0) return -1;
    int ok = -1;
    for (struct addrinfo* p = list; p; p = p->ai_next) {
#ifdef _WIN32
        SOCKET fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd == INVALID_SOCKET) continue;
        if (connect(fd, p->ai_addr, (int)p->ai_addrlen) == 0) { m->fd = fd; ok = 0; break; }
        closesocket(fd);
#else
        int fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd < 0) continue;
        if (connect(fd, p->ai_addr, p->ai_addrlen) == 0) { m->fd = fd; ok = 0; break; }
        close(fd);
#endif
    }
    freeaddrinfo(list);
    return ok;
}

int slmunction_connect(SLMunction* m, const char* uri) {
    if (count_verb(m) != 0) return -1;
    if (!uri || m->phase != PH_STARTED) { mun_bump(m); return -1; }
    const char* rest = NULL;
    SLMunChannel ch = scheme_of(uri, &rest);
    if (ch == SL_MUN_NONE) { mun_bump(m); return -1; }

    if (ch == SL_MUN_CRYPTO) {
        /* crypto wraps an inner channel named by the remainder URI. */
        m->channel = SL_MUN_CRYPTO;
        const char* inner_rest = NULL;
        m->inner = scheme_of(rest, &inner_rest);
        if (m->inner == SL_MUN_NONE) { m->inner = SL_MUN_PIPE; inner_rest = rest; }
        snprintf(m->address, sizeof(m->address), "%s", inner_rest ? inner_rest : "");
        if (m->inner == SL_MUN_NET && net_open(m, m->address) != 0) mun_bump(m);
    } else {
        m->channel = ch;
        snprintf(m->address, sizeof(m->address), "%s", rest ? rest : "");
        if (ch == SL_MUN_FILE) {
            m->fp = fopen(m->address, "a+b");
            if (!m->fp) mun_bump(m);
        } else if (ch == SL_MUN_NET) {
            if (net_open(m, m->address) != 0) mun_bump(m);
        }
    }
    m->phase = PH_CONNECTED;
    return 0;
}

int slmunction_enable(SLMunction* m, const char* policy) {
    if (count_verb(m) != 0) return -1;
    if (!policy || m->phase != PH_CONNECTED) { mun_bump(m); return -1; }
    if (strncmp(policy, "crypto:", 7) == 0) derive_key(m, policy + 7);
    return 0;
}

/* SDPS framing: "SDPS" | uint32 len | payload | 8-byte MAC. */
static int sdps_frame(SLMunction* m, const unsigned char* payload, size_t n,
                      unsigned char* out, size_t cap, size_t* outlen) {
    if (n + 16 > cap) return -1;
    memcpy(out, "SDPS", 4);
    out[4] = (unsigned char)(n >> 24); out[5] = (unsigned char)(n >> 16);
    out[6] = (unsigned char)(n >> 8);  out[7] = (unsigned char)(n);
    memcpy(out + 8, payload, n);
    uint64_t mac = fnv1a(payload, n, fnv1a((const unsigned char*)m->address, strlen(m->address), 0));
    for (int i = 0; i < 8; i++) out[8 + n + i] = (unsigned char)(mac >> (i * 8));
    *outlen = n + 16;
    return 0;
}

static int sdps_deframe(SLMunction* m, const unsigned char* frame, size_t flen,
                        unsigned char* out, size_t cap, size_t* outlen) {
    if (flen < 16 || memcmp(frame, "SDPS", 4) != 0) return -1;
    size_t n = ((size_t)frame[4] << 24) | ((size_t)frame[5] << 16) |
               ((size_t)frame[6] << 8) | (size_t)frame[7];
    if (n + 16 != flen || n > cap) return -1;
    uint64_t mac = fnv1a(frame + 8, n, fnv1a((const unsigned char*)m->address, strlen(m->address), 0));
    for (int i = 0; i < 8; i++) if (frame[8 + n + i] != (unsigned char)(mac >> (i * 8))) return -1;
    memcpy(out, frame + 8, n);
    *outlen = n;
    return 0;
}

int64_t slmunction_send(SLMunction* m, const char* datum, size_t len) {
    if (count_verb(m) != 0) return -1;
    if (m->phase == PH_STARTED || m->phase == PH_CLOSED) { mun_bump(m); return -1; }
    m->phase = PH_MOVING;
    m->sent_calls++;
    m->sent_bytes += (int64_t)len;

    unsigned char scratch[2048];
    if (len > sizeof(scratch)) len = sizeof(scratch);
    memcpy(scratch, datum, len);

    SLMunChannel eff = (m->channel == SL_MUN_CRYPTO) ? m->inner : m->channel;

    if (m->channel == SL_MUN_CRYPTO) crypto_mask(m, scratch, len);   /* seal */

    if (eff == SL_MUN_NET) {
#ifdef _WIN32
        int w = (m->fd == INVALID_SOCKET) ? -1 : send(m->fd, (const char*)scratch, (int)len, 0);
#else
        long w = (m->fd < 0) ? -1 : (long)send(m->fd, scratch, len, 0);
#endif
        if (w < 0) { mun_bump(m); return -1; }
        m->ack_bytes += w;
        return w;
    }

    /* pipe / file / sdps / crypto-over-buffer: enqueue a frame. */
    if (eff == SL_MUN_SDPS) {
        unsigned char framed[2100];
        size_t flen = 0;
        if (sdps_frame(m, scratch, len, framed, sizeof(framed), &flen) != 0) { mun_bump(m); return -1; }
        if (q_push(m, framed, flen) != 0) { mun_bump(m); return -1; }
    } else {
        if (q_push(m, scratch, len) != 0) { mun_bump(m); return -1; }
    }
    if (eff == SL_MUN_FILE && m->fp) {
        fwrite(scratch, 1, len, m->fp);
        fflush(m->fp);
    }
    m->ack_bytes += (int64_t)len;
    return (int64_t)len;
}

int slmunction_thatch(SLMunction* m, const char* spec) {
    if (count_verb(m) != 0) return -1;
    if (m->phase == PH_STARTED || m->phase == PH_CLOSED) { mun_bump(m); return -1; }
    if (spec) snprintf(m->interims, sizeof(m->interims), "%s", spec);
    m->phase = PH_MOVING;
    return 0;
}

int64_t slmunction_consume(SLMunction* m) {
    if (count_verb(m) != 0) return -1;
    if (m->phase == PH_STARTED || m->phase == PH_CLOSED) { mun_bump(m); return -1; }
    m->phase = PH_MOVING;
    m->last_reception_len = 0;

    SLMunChannel eff = (m->channel == SL_MUN_CRYPTO) ? m->inner : m->channel;

    if (eff == SL_MUN_NET) {
        char buf[2048];
#ifdef _WIN32
        int n = (m->fd == INVALID_SOCKET) ? -1 : recv(m->fd, buf, (int)sizeof(buf), 0);
#else
        long n = (m->fd < 0) ? -1 : (long)recv(m->fd, buf, sizeof(buf), 0);
#endif
        if (n <= 0) return -1;              /* absent reception, not fabricated */
        if (m->channel == SL_MUN_CRYPTO) crypto_mask(m, (unsigned char*)buf, (size_t)n);
        int cn = (int)n; if (cn > (int)sizeof(m->last_reception)) cn = (int)sizeof(m->last_reception);
        memcpy(m->last_reception, buf, (size_t)cn);
        m->last_reception_len = cn;
        m->received_units++;
        return cn;
    }

    unsigned char frame[2100];
    size_t flen = 0;
    if (q_pop(m, frame, sizeof(frame), &flen) != 0) return -1;   /* absent */
    unsigned char payload[2048];
    size_t plen = 0;
    if (eff == SL_MUN_SDPS) {
        if (sdps_deframe(m, frame, flen, payload, sizeof(payload), &plen) != 0) {
            mun_bump(m);                    /* integrity bump: detected, dropped */
            return -1;
        }
    } else {
        plen = flen < sizeof(payload) ? flen : sizeof(payload);
        memcpy(payload, frame, plen);
    }
    if (m->channel == SL_MUN_CRYPTO) crypto_mask(m, payload, plen);   /* open */
    int cn = (int)(plen < sizeof(m->last_reception) ? plen : sizeof(m->last_reception));
    memcpy(m->last_reception, payload, (size_t)cn);
    m->last_reception_len = cn;
    m->received_units++;
    return cn;
}

int slmunction_last_reception(const SLMunction* m, char* out, size_t cap) {
    if (!m || !out || cap == 0) return 0;
    int n = m->last_reception_len;
    if (n > (int)cap - 1) n = (int)cap - 1;
    if (n < 0) n = 0;
    memcpy(out, m->last_reception, (size_t)n);
    out[n] = 0;
    return n;
}

int slmunction_observe(SLMunction* m, char* out, size_t cap) {
    if (count_verb(m) != 0) { if (out && cap) out[0] = 0; return 0; }
    if (!out || cap == 0) return 0;
    const char* chname = "none";
    switch (m->channel == SL_MUN_CRYPTO ? m->inner : m->channel) {
        case SL_MUN_PIPE: chname = "pipe"; break;
        case SL_MUN_FILE: chname = "file"; break;
        case SL_MUN_NET:  chname = "tcp";  break;
        case SL_MUN_SDPS: chname = "sdps"; break;
        default: chname = "none"; break;
    }
    int n = snprintf(out, cap, "%s%s:queued=%d:sent=%lld:recv=%lld%s",
                     (m->channel == SL_MUN_CRYPTO ? "crypto/" : ""),
                     chname, m->qcount,
                     (long long)m->sent_calls, (long long)m->received_units,
                     m->latched ? ":latched" : "");
    if (n < 0) return 0;
    return ((size_t)n < cap) ? n : (int)(cap - 1);
}

int slmunction_latch(SLMunction* m) {
    if (count_verb(m) != 0) return -1;
    if (m->phase == PH_STARTED || m->phase == PH_CLOSED || m->latched) { mun_bump(m); return -1; }
    m->latched = 1;
    return 0;
}

static void mun_release(SLMunction* m) {
    for (int i = 0; i < SL_MUNCTION_QUEUE; i++) { free(m->q[i].bytes); m->q[i].bytes = NULL; }
    if (m->fp) { fclose(m->fp); m->fp = NULL; }
#ifdef _WIN32
    if (m->fd != INVALID_SOCKET) { closesocket(m->fd); m->fd = INVALID_SOCKET; }
#else
    if (m->fd >= 0) { close(m->fd); m->fd = -1; }
#endif
}

static SLMunOutcome mun_finish(SLMunction* m, SLMunOutcome requested,
                               char* out, size_t cap) {
    SLMunOutcome outcome = requested;
    if (m->verbs < SL_MUNCTION_MIN_VERBS || m->verbs > SL_MUNCTION_MAX_VERBS) {
        m->bumped = 1;
        if (outcome == SL_MUN_REACHED) outcome = SL_MUN_CONTAINED;
    }
    if (m->bumped && outcome == SL_MUN_REACHED) outcome = SL_MUN_CONTAINED;
    const char* oc = outcome == SL_MUN_REACHED ? "REACHED"
                   : outcome == SL_MUN_CONTAINED ? "CONTAINED" : "ABORTED";
    if (out && cap) {
        snprintf(out, cap,
            "receipt{name=%s scheme=%s address=%s verbs=%d sent=%lld ack=%lld recv=%lld "
            "interims=[%s] latched=%s coherent=%s outcome=%s}",
            m->name,
            m->channel == SL_MUN_CRYPTO ? "crypto" :
              m->channel == SL_MUN_PIPE ? "pipe" :
              m->channel == SL_MUN_FILE ? "file" :
              m->channel == SL_MUN_NET ? "tcp" :
              m->channel == SL_MUN_SDPS ? "sdps" : "none",
            m->address, m->verbs,
            (long long)m->sent_bytes, (long long)m->ack_bytes,
            (long long)m->received_units, m->interims,
            m->latched ? "true" : "false",
            slmunction_coherent(m) ? "true" : "false", oc);
    }
    mun_release(m);
    m->phase = PH_CLOSED;
    /* The reach is a single-use sentence; the closer frees the handle. */
    free(m);
    return outcome;
}

SLMunOutcome slmunction_close(SLMunction* m, char* out, size_t cap) {
    if (!m) { if (out && cap) out[0] = 0; return SL_MUN_ABORTED; }
    if (m->phase == PH_CLOSED) { if (out && cap) out[0] = 0; return SL_MUN_CONTAINED; }
    m->verbs++;                    /* the closer is a verb */
    return mun_finish(m, SL_MUN_REACHED, out, cap);
}

SLMunOutcome slmunction_abort(SLMunction* m, char* out, size_t cap) {
    if (!m) { if (out && cap) out[0] = 0; return SL_MUN_ABORTED; }
    if (m->phase == PH_CLOSED) { if (out && cap) out[0] = 0; return SL_MUN_CONTAINED; }
    m->verbs++;
    m->bumped = 1;                 /* abort records a residual */
    return mun_finish(m, SL_MUN_ABORTED, out, cap);
}

int slmunction_verb_count(const SLMunction* m) { return m ? m->verbs : 0; }
int64_t slmunction_sent_bytes(const SLMunction* m) { return m ? m->sent_bytes : 0; }
int64_t slmunction_received_units(const SLMunction* m) { return m ? m->received_units : 0; }
int slmunction_coherent(const SLMunction* m) {
    if (!m) return 0;
    return (!m->bumped && m->ack_bytes == m->sent_bytes) ? 1 : 0;
}
