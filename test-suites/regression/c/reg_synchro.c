/*
 * reg_synchro.c -- regression suite for impl/core/sleela_synchro.c  (POSIX
 * build; UDP probes against an echo server on the loopback interface)
 *
 * Contract (impl/core/sleela_synchro.h):
 *   - open refuses a NULL/empty host; every accessor tolerates a NULL handle;
 *   - dispatch sends one timestamped probe and returns the round-trip time in
 *     microseconds, or -1 on loss, timeout, a bad timeout argument or a send
 *     failure; a reply counts only when it echoes the probe's sequence tag;
 *   - payloads are clamped to 8..1472 bytes;
 *   - statistics come only from matched replies: min <= mean <= max, the
 *     percentile is clamped to 0..100 and is monotonic, loss is per mille;
 *   - the sample store is bounded (4096) and keeps working past that bound;
 *   - report is always NUL-terminated and never writes past the capacity.
 *
 * The echo server runs in a thread that never uses the REG_ macros.
 */
#define _POSIX_C_SOURCE 200809L
#include "../../../impl/core/sleela_synchro.h"
#include "regression.h"

#ifdef _WIN32
#include <stdio.h>
int main(void) { puts("SUITE synchro: skipped (POSIX-only suite)"); return 0; }
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

static uint64_t now_ms(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
}

typedef enum {
    MODE_ECHO,         /* reply to every probe                                  */
    MODE_DROP_ODD,     /* swallow every second probe                            */
    MODE_STALE_FIRST,  /* send a reply with a wrong tag, then the right one     */
    MODE_SHORT_FIRST,  /* send a 2-byte datagram, then the right reply          */
    MODE_WRONG_ONLY    /* only ever reply with a wrong tag                      */
} Mode;

typedef struct {
    int fd;
    uint16_t port;
    Mode mode;
    volatile int stop;
    volatile int min_len;     /* shortest / longest probe the server saw */
    volatile int max_len;
    volatile int seen;
} Server;

static void *serve(void *arg) {
    Server *sv = (Server *)arg;
    unsigned char buf[2048];
    while (!sv->stop) {
        struct pollfd p = { sv->fd, POLLIN, 0 };
        struct sockaddr_in from;
        socklen_t fl = sizeof from;
        ssize_t n;
        if (poll(&p, 1, 10) <= 0) continue;
        n = recvfrom(sv->fd, buf, sizeof buf, 0, (struct sockaddr *)&from, &fl);
        if (n < 0) continue;
        sv->seen++;
        if (sv->seen == 1 || (int)n < sv->min_len) sv->min_len = (int)n;
        if ((int)n > sv->max_len) sv->max_len = (int)n;
        switch (sv->mode) {
        case MODE_ECHO:
            sendto(sv->fd, buf, (size_t)n, 0, (struct sockaddr *)&from, fl);
            break;
        case MODE_DROP_ODD:
            if (sv->seen % 2 == 1) sendto(sv->fd, buf, (size_t)n, 0, (struct sockaddr *)&from, fl);
            break;
        case MODE_STALE_FIRST: {
            unsigned char wrong[8];
            memcpy(wrong, buf, 8);
            wrong[3] = (unsigned char)(wrong[3] + 77u);      /* a different tag */
            sendto(sv->fd, wrong, 8, 0, (struct sockaddr *)&from, fl);
            sendto(sv->fd, buf, (size_t)n, 0, (struct sockaddr *)&from, fl);
            break;
        }
        case MODE_SHORT_FIRST:
            sendto(sv->fd, buf, 2, 0, (struct sockaddr *)&from, fl);
            sendto(sv->fd, buf, (size_t)n, 0, (struct sockaddr *)&from, fl);
            break;
        case MODE_WRONG_ONLY: {
            unsigned char wrong[8];
            memcpy(wrong, buf, 8);
            wrong[3] = (unsigned char)(wrong[3] + 77u);
            sendto(sv->fd, wrong, 8, 0, (struct sockaddr *)&from, fl);
            break;
        }
        }
    }
    return NULL;
}

static int server_start(Server *sv, pthread_t *th, Mode mode) {
    struct sockaddr_in a;
    socklen_t al = sizeof a;
    memset(sv, 0, sizeof *sv);
    sv->mode = mode;
    sv->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sv->fd < 0) return 0;
    memset(&a, 0, sizeof a);
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    a.sin_port = 0;                                        /* any free port */
    if (bind(sv->fd, (struct sockaddr *)&a, sizeof a) != 0 ||
        getsockname(sv->fd, (struct sockaddr *)&a, &al) != 0) { close(sv->fd); return 0; }
    sv->port = ntohs(a.sin_port);
    if (pthread_create(th, NULL, serve, sv) != 0) { close(sv->fd); return 0; }
    return 1;
}

static void server_stop(Server *sv, pthread_t th) {
    sv->stop = 1;
    pthread_join(th, NULL);
    close(sv->fd);
}

/* ---- arguments and empty state ---------------------------------------------- */
static void t_open_arguments(void) {
    REG_CHECK(slsynchro_open(NULL, 9) == NULL);
    REG_CHECK(slsynchro_open("", 9) == NULL);
    REG_CHECK(slsynchro_open("host.invalid", 9) == NULL);
    slsynchro_close(NULL);                                 /* no-op */
}

static void t_null_handle_accessors(void) {
    char out[32] = "junk";
    REG_EQ_I(0, slsynchro_sent(NULL));
    REG_EQ_I(0, slsynchro_received(NULL));
    REG_EQ_I(-1, slsynchro_mean_us(NULL));
    REG_EQ_I(-1, slsynchro_min_us(NULL));
    REG_EQ_I(-1, slsynchro_max_us(NULL));
    REG_EQ_I(-1, slsynchro_percentile_us(NULL, 50));
    REG_EQ_I(0, slsynchro_loss_permille(NULL));
    REG_EQ_I(-1, slsynchro_dispatch(NULL, 64, 10));
    REG_EQ_I(0, slsynchro_report(NULL, out, sizeof out));
    REG_EQ_I(0, out[0]);                                   /* cleared, not left as junk */
    REG_EQ_I(0, slsynchro_report(NULL, NULL, 0));
}

static void t_fresh_handle_has_no_statistics(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    REG_REQUIRE(server_start(&sv, &th, MODE_ECHO));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    REG_EQ_I(0, slsynchro_sent(s));
    REG_EQ_I(0, slsynchro_received(s));
    REG_EQ_I(-1, slsynchro_mean_us(s));                    /* no samples: -1, not 0 and not a crash */
    REG_EQ_I(-1, slsynchro_min_us(s));
    REG_EQ_I(-1, slsynchro_max_us(s));
    REG_EQ_I(-1, slsynchro_percentile_us(s, 95));
    REG_EQ_I(0, slsynchro_loss_permille(s));
    slsynchro_close(s);
    server_stop(&sv, th);
}

static void t_negative_timeout_is_refused(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    REG_REQUIRE(server_start(&sv, &th, MODE_ECHO));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    REG_EQ_I(-1, slsynchro_dispatch(s, 64, -1));
    REG_EQ_I(0, slsynchro_sent(s));                        /* nothing was sent */
    slsynchro_close(s);
    server_stop(&sv, th);
}

/* ---- measurement ------------------------------------------------------------ */
static void t_round_trips_and_statistics(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    int i;
    int64_t last = 0, p0, p50, p95, p100, mn, mx, mean;
    REG_REQUIRE(server_start(&sv, &th, MODE_ECHO));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    for (i = 0; i < 40; ++i) {
        last = slsynchro_dispatch(s, 64, 500);
        REG_CHECK(last >= 0 && last < 500000);             /* a real reply, in under 500 ms */
    }
    REG_EQ_I(40, slsynchro_sent(s));
    REG_EQ_I(40, slsynchro_received(s));
    REG_EQ_I(0, slsynchro_loss_permille(s));
    mn = slsynchro_min_us(s); mx = slsynchro_max_us(s); mean = slsynchro_mean_us(s);
    REG_CHECK(mn >= 0 && mn <= mean && mean <= mx);
    p0 = slsynchro_percentile_us(s, 0);
    p50 = slsynchro_percentile_us(s, 50);
    p95 = slsynchro_percentile_us(s, 95);
    p100 = slsynchro_percentile_us(s, 100);
    REG_EQ_I(mn, p0);                                      /* p0 is the minimum, p100 the maximum */
    REG_EQ_I(mx, p100);
    REG_CHECK(p0 <= p50 && p50 <= p95 && p95 <= p100);     /* monotonic in p */
    REG_EQ_I(mn, slsynchro_percentile_us(s, -20));         /* out-of-range p is clamped */
    REG_EQ_I(mx, slsynchro_percentile_us(s, 1000));
    (void)last;
    slsynchro_close(s);
    server_stop(&sv, th);
}

static void t_payload_is_clamped(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    REG_REQUIRE(server_start(&sv, &th, MODE_ECHO));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    REG_CHECK(slsynchro_dispatch(s, 0, 500) >= 0);         /* too small: raised to 8 */
    REG_CHECK(slsynchro_dispatch(s, 3, 500) >= 0);
    REG_CHECK(slsynchro_dispatch(s, 100000, 500) >= 0);    /* too large: cut to 1472 */
    slsynchro_close(s);
    server_stop(&sv, th);
    REG_EQ_I(8, sv.min_len);
    REG_EQ_I(1472, sv.max_len);
}

static void t_loss_is_counted(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    int i, ok = 0, lost = 0;
    REG_REQUIRE(server_start(&sv, &th, MODE_DROP_ODD));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    for (i = 0; i < 10; ++i) {
        int64_t r = slsynchro_dispatch(s, 32, 250);
        if (r >= 0) ++ok; else ++lost;
    }
    REG_EQ_I(5, ok);
    REG_EQ_I(5, lost);
    REG_EQ_I(10, slsynchro_sent(s));
    REG_EQ_I(5, slsynchro_received(s));
    REG_EQ_I(500, slsynchro_loss_permille(s));
    REG_CHECK(slsynchro_mean_us(s) >= 0);                  /* only matched replies feed the statistics */
    slsynchro_close(s);
    server_stop(&sv, th);
}

static void t_stale_reply_is_ignored(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    int64_t r;
    REG_REQUIRE(server_start(&sv, &th, MODE_STALE_FIRST));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    r = slsynchro_dispatch(s, 32, 500);
    REG_CHECK(r >= 0);                                     /* the right reply still arrives behind it */
    REG_EQ_I(1, slsynchro_received(s));
    slsynchro_close(s);
    server_stop(&sv, th);
}

static void t_short_reply_is_ignored(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    REG_REQUIRE(server_start(&sv, &th, MODE_SHORT_FIRST));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    REG_CHECK(slsynchro_dispatch(s, 32, 500) >= 0);
    REG_EQ_I(1, slsynchro_received(s));
    slsynchro_close(s);
    server_stop(&sv, th);
}

static void t_wrong_tag_only_times_out_on_budget(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    uint64_t t0, dt;
    REG_REQUIRE(server_start(&sv, &th, MODE_WRONG_ONLY));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    t0 = now_ms();
    REG_EQ_I(-1, slsynchro_dispatch(s, 32, 120));
    dt = now_ms() - t0;
    REG_CHECK(dt >= 100 && dt < 2000);                     /* gave up at the budget, neither at once nor never */
    REG_EQ_I(0, slsynchro_received(s));                    /* a mismatched reply is never a sample */
    REG_EQ_I(-1, slsynchro_mean_us(s));
    REG_EQ_I(1000, slsynchro_loss_permille(s));
    slsynchro_close(s);
    server_stop(&sv, th);
}

static void t_dead_port_is_loss_not_hang(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    uint64_t t0;
    uint16_t port;
    REG_REQUIRE(server_start(&sv, &th, MODE_ECHO));
    port = sv.port;
    server_stop(&sv, th);                                  /* the port is now closed */
    s = slsynchro_open("127.0.0.1", port);
    REG_REQUIRE(s != NULL);
    t0 = now_ms();
    REG_EQ_I(-1, slsynchro_dispatch(s, 32, 300));
    REG_CHECK(now_ms() - t0 < 2000);
    REG_EQ_I(1, slsynchro_sent(s));
    REG_EQ_I(0, slsynchro_received(s));
    slsynchro_close(s);
}

/* The sample store holds 4096 entries; probes beyond that must not overflow it. */
static void t_sample_store_is_bounded(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    int i, got = 0;
    int64_t mn, mx, p50;
    enum { PROBES = SL_SYNCHRO_MAX_SAMPLES + 300 };
    REG_REQUIRE(server_start(&sv, &th, MODE_ECHO));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    for (i = 0; i < PROBES; ++i)
        if (slsynchro_dispatch(s, 8, 1000) >= 0) ++got;
    REG_EQ_I(got, slsynchro_received(s));
    REG_CHECK(got > SL_SYNCHRO_MAX_SAMPLES);               /* we really went past the bound */
    mn = slsynchro_min_us(s); mx = slsynchro_max_us(s); p50 = slsynchro_percentile_us(s, 50);
    REG_CHECK(mn >= 0 && mn <= p50 && p50 <= mx);
    slsynchro_close(s);
    server_stop(&sv, th);
}

/* ---- report ------------------------------------------------------------------ */
static void t_report_format_and_bounds(void) {
    Server sv; pthread_t th;
    SLSynchro *s;
    char out[256], want[64], tiny[12];
    int n, i;
    REG_REQUIRE(server_start(&sv, &th, MODE_ECHO));
    s = slsynchro_open("127.0.0.1", sv.port);
    REG_REQUIRE(s != NULL);
    for (i = 0; i < 3; ++i) REG_CHECK(slsynchro_dispatch(s, 16, 500) >= 0);
    n = slsynchro_report(s, out, sizeof out);
    REG_CHECK(n > 0 && (size_t)n == strlen(out));
    snprintf(want, sizeof want, "synchro 127.0.0.1:%u sent=3 recv=3 loss=0permille", (unsigned)sv.port);
    REG_CHECK(strncmp(out, want, strlen(want)) == 0);
    REG_CHECK(strstr(out, "mean=") && strstr(out, "p95=") && strstr(out, "min=") && strstr(out, "max="));

    memset(tiny, 'x', sizeof tiny);
    n = slsynchro_report(s, tiny, sizeof tiny);            /* truncated: capacity - 1, NUL-terminated */
    REG_EQ_I((int)sizeof tiny - 1, n);
    REG_EQ_I(0, tiny[sizeof tiny - 1]);
    REG_EQ_I(0, slsynchro_report(s, out, 0));
    slsynchro_close(s);
    server_stop(&sv, th);
}

int main(void) {
    alarm(180);
    printf("synchro regression suite (loopback UDP)\n");
    REG_RUN(t_open_arguments);
    REG_RUN(t_null_handle_accessors);
    REG_RUN(t_fresh_handle_has_no_statistics);
    REG_RUN(t_negative_timeout_is_refused);
    REG_RUN(t_round_trips_and_statistics);
    REG_RUN(t_payload_is_clamped);
    REG_RUN(t_loss_is_counted);
    REG_RUN(t_stale_reply_is_ignored);
    REG_RUN(t_short_reply_is_ignored);
    REG_RUN(t_wrong_tag_only_times_out_on_budget);
    REG_RUN(t_dead_port_is_loss_not_hang);
    REG_RUN(t_sample_store_is_bounded);
    REG_RUN(t_report_format_and_bounds);
    return reg_finish("synchro");
}
#endif
