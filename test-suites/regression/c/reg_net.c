/*
 * reg_net.c -- regression suite for impl/core/sleela_net.c  (POSIX build,
 * loopback only: no traffic leaves the machine)
 *
 * Contract (impl/core/sleela_net.h):
 *   - SL_NET_INVALID (-1) is never a usable handle: read, write, accept, close
 *     and set_timeouts on it fail, and fail the same way for zero-length calls;
 *   - bad arguments (NULL/empty host, port 0 for listen_on, backlog < 1,
 *     NULL buffer, zero capacity, zero timeouts) are refused, not forwarded;
 *   - read returns the byte count, 0 at an orderly shutdown by the peer, -1 on
 *     error or when a configured receive timeout expires;
 *   - SLEELA_NET_PLATFORM names the backend; a different OS's name is refused.
 *
 * Ports are scanned from a high range, so a busy port skips a case's port
 * instead of failing it.  A watchdog (alarm) turns any hang into a failure.
 */
#define _POSIX_C_SOURCE 200809L
#include "../../../impl/core/sleela_net.h"
#include "regression.h"

#ifdef _WIN32
#include <stdio.h>
int main(void) { puts("SUITE net: skipped (POSIX-only suite)"); return 0; }
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PORT_FIRST 47100u
#define PORT_LAST  47399u

static uint64_t now_ms(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
}

/* Listen on loopback at the first free port in range.  The scan starts at an
 * offset taken from the process id, so suites running side by side (the mutation
 * test runs several) rarely look at the same port first. */
static SLNetHandle listen_loopback(uint16_t *port_out) {
    const unsigned span = PORT_LAST - PORT_FIRST + 1u;
    const unsigned start = ((unsigned)getpid() * 7u) % span;
    unsigned i;
    for (i = 0; i < span; ++i) {
        uint16_t p = (uint16_t)(PORT_FIRST + (start + i) % span);
        SLNetHandle l = slnet_listen_on("127.0.0.1", p, 4);
        if (slnet_valid(l)) { *port_out = p; return l; }
    }
    return SL_NET_INVALID;
}

/* Open a connected pair: client (connect) and server side (accept). */
static int pair(SLNetHandle *listener, SLNetHandle *client, SLNetHandle *server, uint16_t *port) {
    *listener = listen_loopback(port);
    if (!slnet_valid(*listener)) return 0;
    *client = slnet_connect("127.0.0.1", *port);
    if (!slnet_valid(*client)) { slnet_close(*listener); return 0; }
    *server = slnet_accept(*listener);
    if (!slnet_valid(*server)) { slnet_close(*client); slnet_close(*listener); return 0; }
    return 1;
}

/* ---- platform and startup -------------------------------------------------- */
static void t_platform(void) {
#if defined(__APPLE__)
    REG_EQ_I(SL_NET_MACOS, slnet_platform());
    REG_EQ_S("macos", slnet_platform_name());
#else
    REG_EQ_I(SL_NET_LINUX, slnet_platform());
    REG_EQ_S("linux", slnet_platform_name());
#endif
    REG_EQ_I(1, slnet_platform_is_available(SL_NET_AUTO));
    REG_EQ_I(1, slnet_platform_is_available(slnet_platform()));
    REG_EQ_I(0, slnet_platform_is_available(SL_NET_WINDOWS));
}

static void t_startup_platform_selection(void) {
    unsetenv("SLEELA_NET_PLATFORM");
    REG_EQ_I(0, slnet_startup());
    setenv("SLEELA_NET_PLATFORM", "auto", 1);   REG_EQ_I(0, slnet_startup());
    setenv("SLEELA_NET_PLATFORM", "posix", 1);  REG_EQ_I(0, slnet_startup());
    setenv("SLEELA_NET_PLATFORM", "", 1);       REG_EQ_I(0, slnet_startup());
    setenv("SLEELA_NET_PLATFORM", "no-such-backend", 1); REG_EQ_I(0, slnet_startup()); /* unknown: auto */
    setenv("SLEELA_NET_PLATFORM", "windows", 1); REG_EQ_I(-1, slnet_startup());        /* another OS's backend */
    setenv("SLEELA_NET_PLATFORM", "win32", 1);   REG_EQ_I(-1, slnet_startup());
#if defined(__APPLE__)
    setenv("SLEELA_NET_PLATFORM", "darwin", 1);  REG_EQ_I(0, slnet_startup());
    setenv("SLEELA_NET_PLATFORM", "linux", 1);   REG_EQ_I(-1, slnet_startup());
#else
    setenv("SLEELA_NET_PLATFORM", "linux", 1);   REG_EQ_I(0, slnet_startup());
    setenv("SLEELA_NET_PLATFORM", "macos", 1);   REG_EQ_I(-1, slnet_startup());
    setenv("SLEELA_NET_PLATFORM", "darwin", 1);  REG_EQ_I(-1, slnet_startup());
#endif
    unsetenv("SLEELA_NET_PLATFORM");
    slnet_shutdown();
}

/* ---- the invalid handle ---------------------------------------------------- */
static void t_invalid_handle(void) {
    char buf[8];
    REG_EQ_I(0, slnet_valid(SL_NET_INVALID));
    REG_EQ_I(0, slnet_valid((SLNetHandle)-5));
    REG_EQ_I(1, slnet_valid((SLNetHandle)0));         /* fd 0 is a legal descriptor */
    REG_EQ_I(-1, slnet_close(SL_NET_INVALID));
    REG_EQ_I(-1, slnet_read(SL_NET_INVALID, buf, sizeof buf));
    REG_EQ_I(-1, slnet_write(SL_NET_INVALID, "abc", 3));
    REG_EQ_I(-1, slnet_set_timeouts(SL_NET_INVALID, 100, 100));
    REG_CHECK(!slnet_valid(slnet_accept(SL_NET_INVALID)));
}

/* Regression (was NET-WRITE-INVALID-ZERO): an invalid handle fails a
 * zero-length write too; it is not reported as success. */
static void t_write_zero_length_on_invalid_handle(void) {
    REG_EQ_I(-1, slnet_write(SL_NET_INVALID, "abc", 0));
    REG_EQ_I(-1, slnet_write(SL_NET_INVALID, NULL, 0));
}

/* ---- argument validation ---------------------------------------------------- */
static void t_listen_arguments(void) {
    REG_CHECK(!slnet_valid(slnet_listen(PORT_FIRST, 0)));            /* backlog < 1 */
    REG_CHECK(!slnet_valid(slnet_listen(PORT_FIRST, -3)));
    REG_CHECK(!slnet_valid(slnet_listen_on("127.0.0.1", 0, 4)));      /* port 0 */
    REG_CHECK(!slnet_valid(slnet_listen_on("127.0.0.1", PORT_FIRST, 0)));
    REG_CHECK(!slnet_valid(slnet_listen_on("999.999.999.999", PORT_FIRST, 4)));
    REG_CHECK(!slnet_valid(slnet_listen_on("host.invalid", PORT_FIRST, 4)));
}

static void t_connect_arguments(void) {
    REG_CHECK(!slnet_valid(slnet_connect(NULL, PORT_FIRST)));
    REG_CHECK(!slnet_valid(slnet_connect("", PORT_FIRST)));
    REG_CHECK(!slnet_valid(slnet_connect("999.999.999.999", PORT_FIRST)));
    REG_CHECK(!slnet_valid(slnet_connect("host.invalid", PORT_FIRST)));
}

static void t_connect_to_closed_port_is_refused(void) {
    uint16_t port = 0;
    SLNetHandle l = listen_loopback(&port);
    REG_REQUIRE(slnet_valid(l));
    REG_EQ_I(0, slnet_close(l));                       /* nothing listens there now */
    REG_CHECK(!slnet_valid(slnet_connect("127.0.0.1", port)));
    REG_CHECK(slnet_last_error() != 0);
}

static void t_buffer_arguments(void) {
    SLNetHandle l, c, s;
    uint16_t port;
    char buf[8];
    REG_REQUIRE(pair(&l, &c, &s, &port));
    REG_EQ_I(-1, slnet_read(c, NULL, sizeof buf));
    REG_EQ_I(-1, slnet_read(c, buf, 0));
    REG_EQ_I(-1, slnet_write(c, NULL, 4));
    REG_EQ_I(0, slnet_write(c, "x", 0));              /* a valid handle: zero bytes, zero result */
    REG_EQ_I(0, slnet_set_timeouts(c, 50, 50));
    REG_EQ_I(-1, slnet_set_timeouts(c, 0, 50));       /* zero would mean "forever": refused */
    REG_EQ_I(-1, slnet_set_timeouts(c, 50, 0));
    REG_EQ_I(0, slnet_close(c)); REG_EQ_I(0, slnet_close(s)); REG_EQ_I(0, slnet_close(l));
}

/* ---- data transfer ---------------------------------------------------------- */
static void t_round_trip(void) {
    SLNetHandle l, c, s;
    uint16_t port;
    char buf[64];
    SLNetCount n;
    REG_REQUIRE(pair(&l, &c, &s, &port));
    REG_EQ_I(4, slnet_write(c, "ping", 4));
    n = slnet_read(s, buf, sizeof buf);
    REG_EQ_I(4, n);
    REG_CHECK(n == 4 && memcmp(buf, "ping", 4) == 0);
    REG_EQ_I(4, slnet_write(s, "pong", 4));
    n = slnet_read(c, buf, sizeof buf);
    REG_EQ_I(4, n);
    REG_CHECK(n == 4 && memcmp(buf, "pong", 4) == 0);
    slnet_close(c); slnet_close(s); slnet_close(l);
}

static void t_large_payload_survives_in_order(void) {
    enum { TOTAL = 48000 };
    SLNetHandle l, c, s;
    uint16_t port;
    static unsigned char out[TOTAL], in[TOTAL];
    size_t sent = 0, got = 0, i;
    REG_REQUIRE(pair(&l, &c, &s, &port));
    for (i = 0; i < TOTAL; ++i) out[i] = (unsigned char)(i * 31u + (i >> 8));
    slnet_set_timeouts(s, 3000, 3000);
    while (sent < TOTAL) {                             /* fits in loopback buffers; no thread needed */
        SLNetCount w = slnet_write(c, (const char *)out + sent, TOTAL - sent);
        if (w <= 0) break;
        sent += (size_t)w;
    }
    REG_EQ_U(TOTAL, sent);
    while (got < TOTAL) {
        SLNetCount r = slnet_read(s, (char *)in + got, TOTAL - got);
        if (r <= 0) break;
        got += (size_t)r;
    }
    REG_EQ_U(TOTAL, got);
    REG_EQ_MEM(out, in, TOTAL);
    slnet_close(c); slnet_close(s); slnet_close(l);
}

static void t_orderly_shutdown_reads_zero(void) {
    SLNetHandle l, c, s;
    uint16_t port;
    char buf[8];
    REG_REQUIRE(pair(&l, &c, &s, &port));
    REG_EQ_I(2, slnet_write(c, "hi", 2));
    REG_EQ_I(0, slnet_close(c));
    REG_EQ_I(2, slnet_read(s, buf, sizeof buf));       /* data sent before the close still arrives */
    REG_EQ_I(0, slnet_read(s, buf, sizeof buf));       /* then end of stream: 0, not an error */
    REG_EQ_I(0, slnet_read(s, buf, sizeof buf));       /* and it stays 0 */
    slnet_close(s); slnet_close(l);
}

static void t_receive_timeout(void) {
    SLNetHandle l, c, s;
    uint16_t port;
    char buf[8];
    uint64_t t0, dt;
    REG_REQUIRE(pair(&l, &c, &s, &port));
    REG_EQ_I(0, slnet_set_timeouts(s, 150, 150));
    t0 = now_ms();
    REG_EQ_I(-1, slnet_read(s, buf, sizeof buf));      /* silent peer: the timeout fires */
    dt = now_ms() - t0;
    REG_CHECK(dt >= 100 && dt < 3000);                 /* about 150 ms, not forever, not at once */
    slnet_close(c); slnet_close(s); slnet_close(l);
}

/* ---- listeners -------------------------------------------------------------- */
static void t_second_listener_on_same_port_fails(void) {
    uint16_t port;
    SLNetHandle a = listen_loopback(&port), b;
    REG_REQUIRE(slnet_valid(a));
    b = slnet_listen_on("127.0.0.1", port, 4);
    REG_CHECK(!slnet_valid(b));                        /* the port is in use */
    if (slnet_valid(b)) slnet_close(b);
    slnet_close(a);
}

static void t_any_address_listener(void) {
    uint16_t p = PORT_FIRST;
    SLNetHandle l = SL_NET_INVALID, c, s;
    {
        const unsigned span = PORT_LAST - PORT_FIRST + 1u;
        const unsigned start = ((unsigned)getpid() * 11u) % span;
        unsigned i;
        for (i = 0; i < span; ++i) {
            p = (uint16_t)(PORT_FIRST + (start + i) % span);
            l = slnet_listen(p, 4);
            if (slnet_valid(l)) break;
        }
    }
    REG_REQUIRE(slnet_valid(l));
    c = slnet_connect("127.0.0.1", p);
    REG_REQUIRE(slnet_valid(c));
    s = slnet_accept(l);
    REG_CHECK(slnet_valid(s));
    if (slnet_valid(s)) {
        char b[4];
        REG_EQ_I(1, slnet_write(c, "z", 1));
        REG_EQ_I(1, slnet_read(s, b, sizeof b));
        slnet_close(s);
    }
    slnet_close(c); slnet_close(l);
}

static void t_double_close(void) {
    uint16_t port;
    SLNetHandle l = listen_loopback(&port);
    REG_REQUIRE(slnet_valid(l));
    REG_EQ_I(0, slnet_close(l));
    REG_EQ_I(-1, slnet_close(l));                      /* the second close is an error */
}

int main(void) {
    alarm(120);                                        /* a hang is a failure, not a stuck build */
    printf("net regression suite (loopback)\n");
    REG_RUN(t_platform);
    REG_RUN(t_startup_platform_selection);
    REG_RUN(t_invalid_handle);
    REG_RUN(t_write_zero_length_on_invalid_handle);
    REG_RUN(t_listen_arguments);
    REG_RUN(t_connect_arguments);
    REG_RUN(t_connect_to_closed_port_is_refused);
    REG_RUN(t_buffer_arguments);
    REG_RUN(t_round_trip);
    REG_RUN(t_large_payload_survives_in_order);
    REG_RUN(t_orderly_shutdown_reads_zero);
    REG_RUN(t_receive_timeout);
    REG_RUN(t_second_listener_on_same_port_fails);
    REG_RUN(t_any_address_listener);
    REG_RUN(t_double_close);
    return reg_finish("net");
}
#endif
