/* ==========================================================================
 * sleela_synchro.c -- Synchro implementation (honest measured latency).
 *
 * Portable UDP probe/measurement. A probe binds a connected UDP socket to the
 * destination, sends timestamped datagrams, and matches echoed replies. All
 * timing uses a monotonic clock; the distribution is measured, never promised.
 *
 * The socket layer is deliberately self-contained (BSD/Winsock directly) so the
 * module has no dependency beyond the platform sockets already required by the
 * engine. It follows the same OS split the rest of the core uses.
 * ========================================================================== */
#if !defined(_WIN32) && !defined(_WIN64) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include "sleela_synchro.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <poll.h>
#endif

struct SLSynchro {
#ifdef _WIN32
    SOCKET fd;
#else
    int fd;
#endif
    char host[256];
    uint16_t port;
    int64_t sent;
    int64_t received;
    uint32_t seq;                    /* monotonically increasing probe id     */
    int nsamples;                    /* retained matched RTT samples (us)      */
    int64_t samples[SL_SYNCHRO_MAX_SAMPLES];
};

/* Monotonic nanoseconds (portable). */
static int64_t mono_ns(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq;
    static int have = 0;
    LARGE_INTEGER c;
    if (!have) { QueryPerformanceFrequency(&freq); have = 1; }
    QueryPerformanceCounter(&c);
    return (int64_t)((c.QuadPart * 1000000000LL) / freq.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000000LL + (int64_t)ts.tv_nsec;
#endif
}

SLSynchro* slsynchro_open(const char* host, uint16_t port) {
    if (!host || !*host) return NULL;
    char service[16];
    snprintf(service, sizeof(service), "%u", (unsigned)port);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    struct addrinfo* list = NULL;
    if (getaddrinfo(host, service, &hints, &list) != 0) return NULL;

    SLSynchro* s = (SLSynchro*)calloc(1, sizeof(SLSynchro));
    if (!s) { freeaddrinfo(list); return NULL; }
#ifdef _WIN32
    s->fd = INVALID_SOCKET;
#else
    s->fd = -1;
#endif
    for (struct addrinfo* p = list; p; p = p->ai_next) {
#ifdef _WIN32
        SOCKET fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd == INVALID_SOCKET) continue;
        if (connect(fd, p->ai_addr, (int)p->ai_addrlen) == 0) { s->fd = fd; break; }
        closesocket(fd);
#else
        int fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd < 0) continue;
        /* connect() on a UDP socket fixes the peer so send/recv work and
         * replies from other sources are filtered by the kernel. */
        if (connect(fd, p->ai_addr, p->ai_addrlen) == 0) { s->fd = fd; break; }
        close(fd);
#endif
    }
    freeaddrinfo(list);
#ifdef _WIN32
    if (s->fd == INVALID_SOCKET) { free(s); return NULL; }
#else
    if (s->fd < 0) { free(s); return NULL; }
#endif
    snprintf(s->host, sizeof(s->host), "%s", host);
    s->port = port;
    return s;
}

/* Wait up to timeout_ms for the socket to be readable. Returns 1 readable,
 * 0 timeout, -1 error. */
static int wait_readable(SLSynchro* s, int timeout_ms) {
#ifdef _WIN32
    fd_set rf;
    FD_ZERO(&rf);
    FD_SET(s->fd, &rf);
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    int r = select(0, &rf, NULL, NULL, &tv);
    return r > 0 ? 1 : (r == 0 ? 0 : -1);
#else
    struct pollfd pfd;
    pfd.fd = s->fd;
    pfd.events = POLLIN;
    int r = poll(&pfd, 1, timeout_ms);
    return r > 0 ? 1 : (r == 0 ? 0 : -1);
#endif
}

int64_t slsynchro_dispatch(SLSynchro* s, size_t payload_len, int timeout_ms) {
    if (!s || timeout_ms < 0) return -1;
    if (payload_len < 8) payload_len = 8;          /* room for the 8-byte tag  */
    if (payload_len > 1472) payload_len = 1472;    /* keep within a typical MTU */

    /* Build a probe: 4-byte magic-seq tag then filler. The reply must echo the
     * tag for the sample to be counted (honest matching, never fabricated). */
    unsigned char buf[1472];
    uint32_t seq = ++s->seq;
    memset(buf, 0, payload_len);
    buf[0] = (unsigned char)(seq >> 24);
    buf[1] = (unsigned char)(seq >> 16);
    buf[2] = (unsigned char)(seq >> 8);
    buf[3] = (unsigned char)(seq);
    buf[4] = 'S'; buf[5] = 'Y'; buf[6] = 'N'; buf[7] = 'C';

    s->sent++;
    int64_t t0 = mono_ns();
#ifdef _WIN32
    int w = send(s->fd, (const char*)buf, (int)payload_len, 0);
#else
    ssize_t w = send(s->fd, buf, payload_len, 0);
#endif
    if (w < 0) return -1;                          /* send failure counts as loss */

    for (;;) {
        int64_t elapsed_ms = (mono_ns() - t0) / 1000000LL;
        int remaining = timeout_ms - (int)elapsed_ms;
        if (remaining < 0) remaining = 0;
        int ready = wait_readable(s, remaining);
        if (ready <= 0) return -1;                 /* timeout or error = loss   */

        unsigned char rbuf[1472];
#ifdef _WIN32
        int n = recv(s->fd, (char*)rbuf, (int)sizeof(rbuf), 0);
#else
        ssize_t n = recv(s->fd, rbuf, sizeof(rbuf), 0);
#endif
        if (n < 4) {
            if ((int)((mono_ns() - t0) / 1000000LL) >= timeout_ms) return -1;
            continue;
        }
        uint32_t rseq = ((uint32_t)rbuf[0] << 24) | ((uint32_t)rbuf[1] << 16) |
                        ((uint32_t)rbuf[2] << 8) | (uint32_t)rbuf[3];
        if (rseq != seq) {
            /* A stale/mismatched reply: keep waiting within the budget. */
            if ((int)((mono_ns() - t0) / 1000000LL) >= timeout_ms) return -1;
            continue;
        }
        int64_t rtt_us = (mono_ns() - t0) / 1000LL;
        s->received++;
        if (s->nsamples < SL_SYNCHRO_MAX_SAMPLES) s->samples[s->nsamples++] = rtt_us;
        else s->samples[s->seq % SL_SYNCHRO_MAX_SAMPLES] = rtt_us;
        return rtt_us;
    }
}

int64_t slsynchro_sent(const SLSynchro* s) { return s ? s->sent : 0; }
int64_t slsynchro_received(const SLSynchro* s) { return s ? s->received : 0; }

int64_t slsynchro_mean_us(const SLSynchro* s) {
    if (!s || s->nsamples == 0) return -1;
    int64_t sum = 0;
    for (int i = 0; i < s->nsamples; i++) sum += s->samples[i];
    return sum / s->nsamples;
}

int64_t slsynchro_min_us(const SLSynchro* s) {
    if (!s || s->nsamples == 0) return -1;
    int64_t m = s->samples[0];
    for (int i = 1; i < s->nsamples; i++) if (s->samples[i] < m) m = s->samples[i];
    return m;
}

int64_t slsynchro_max_us(const SLSynchro* s) {
    if (!s || s->nsamples == 0) return -1;
    int64_t m = s->samples[0];
    for (int i = 1; i < s->nsamples; i++) if (s->samples[i] > m) m = s->samples[i];
    return m;
}

static int cmp_i64(const void* a, const void* b) {
    int64_t x = *(const int64_t*)a, y = *(const int64_t*)b;
    return (x > y) - (x < y);
}

int64_t slsynchro_percentile_us(const SLSynchro* s, int p) {
    if (!s || s->nsamples == 0) return -1;
    if (p < 0) p = 0;
    if (p > 100) p = 100;
    int64_t* tmp = (int64_t*)malloc((size_t)s->nsamples * sizeof(int64_t));
    if (!tmp) return -1;
    memcpy(tmp, s->samples, (size_t)s->nsamples * sizeof(int64_t));
    qsort(tmp, (size_t)s->nsamples, sizeof(int64_t), cmp_i64);
    /* nearest-rank percentile */
    int rank = (int)(((int64_t)p * (s->nsamples - 1) + 50) / 100);
    if (rank < 0) rank = 0;
    if (rank >= s->nsamples) rank = s->nsamples - 1;
    int64_t v = tmp[rank];
    free(tmp);
    return v;
}

int64_t slsynchro_loss_permille(const SLSynchro* s) {
    if (!s || s->sent == 0) return 0;
    int64_t lost = s->sent - s->received;
    if (lost < 0) lost = 0;
    return (lost * 1000) / s->sent;
}

int slsynchro_report(const SLSynchro* s, char* out, size_t cap) {
    if (!out || cap == 0) return 0;
    if (!s) { out[0] = 0; return 0; }
    int n = snprintf(out, cap,
        "synchro %s:%u sent=%lld recv=%lld loss=%lldpermille mean=%lldus p95=%lldus min=%lldus max=%lldus",
        s->host, (unsigned)s->port,
        (long long)s->sent, (long long)s->received,
        (long long)slsynchro_loss_permille(s),
        (long long)slsynchro_mean_us(s),
        (long long)slsynchro_percentile_us(s, 95),
        (long long)slsynchro_min_us(s),
        (long long)slsynchro_max_us(s));
    if (n < 0) { out[0] = 0; return 0; }
    return ((size_t)n < cap) ? n : (int)(cap - 1);
}

void slsynchro_close(SLSynchro* s) {
    if (!s) return;
#ifdef _WIN32
    if (s->fd != INVALID_SOCKET) closesocket(s->fd);
#else
    if (s->fd >= 0) close(s->fd);
#endif
    free(s);
}
