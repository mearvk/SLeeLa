/*
 * reg_time_memory.c -- regression suite for impl/core/sleela_time.c (local,
 * network-free surface) and impl/core/sleela_memory.c
 *
 * Time contract (sleela_time.h): clocks are monotonic/realtime, formatting is
 * UTC, ISO-8601 is "YYYY-MM-DDTHH:MM:SS.mmmZ" (needs a >=32 byte buffer), the
 * HTTP date is RFC 9110 IMF-fixdate (>=30 bytes), JSON/BODI records have a
 * fixed layout.  Expected strings were cross-computed with Python's datetime;
 * the calendar sweep uses an independent days-to-civil algorithm.
 *
 * NTP/network entry points are deliberately not exercised (no network in a
 * regression run).
 */
#define _POSIX_C_SOURCE 200809L
#include "../../../impl/core/sleela_memory.h"
#include "../../../impl/core/sleela_time.h"
#include "regression.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---- independent calendar oracle (Hinnant civil_from_days) -------------- */
static void civil_from_days(long long z, long long *y, unsigned *m, unsigned *d) {
    long long era, yoe, doy, doe, mp;
    z += 719468;
    era = (z >= 0 ? z : z - 146096) / 146097;
    doe = z - era * 146097;
    yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    *y = yoe + era * 400;
    doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    mp = (5 * doy + 2) / 153;
    *d = (unsigned)(doy - (153 * mp + 2) / 5 + 1);
    *m = (unsigned)(mp < 10 ? mp + 3 : mp - 9);
    if (*m <= 2) ++*y;
}

static long long floor_div(long long a, long long b) {
    long long q = a / b; if ((a % b != 0) && ((a < 0) != (b < 0))) --q; return q;
}

static void oracle_iso(long long ms, char *out, char *http) {
    static const char *wd[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    static const char *mo[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    long long secs = floor_div(ms, 1000), days = floor_div(secs, 86400), sod = secs - days * 86400, y;
    unsigned m, d; int wday = (int)(((days % 7) + 11) % 7); /* 1970-01-01 is Thursday (4) */
    civil_from_days(days, &y, &m, &d);
    snprintf(out, 128, "%04lld-%02u-%02uT%02lld:%02lld:%02lld.%03lldZ", y, m, d, sod / 3600, (sod / 60) % 60, sod % 60, ms - secs * 1000);
    snprintf(http, 128, "%s, %02u %s %04lld %02lld:%02lld:%02lld GMT", wd[wday], d, mo[m - 1], y, sod / 3600, (sod / 60) % 60, sod % 60);
}

/* ---- time --------------------------------------------------------------- */

static void t_location_defaults_then_set(void) {
    /* must run first: process-global state */
    char *saved = getenv("TZ");
    REG_EQ_S("??", sltime_location_country());
    unsetenv("TZ");
    REG_EQ_S("LOCAL", sltime_location_timezone());
    setenv("TZ", "Europe/Paris", 1);
    REG_EQ_S("Europe/Paris", sltime_location_timezone());
    if (saved) setenv("TZ", saved, 1); else unsetenv("TZ");

    REG_EQ_I(0, sltime_set_location(SL_TIME_LOCATION_COUNTRY, "US", "America/New_York"));
    REG_EQ_S("US", sltime_location_country()); REG_EQ_S("America/New_York", sltime_location_timezone());
    REG_EQ_I(0, sltime_set_location(SL_TIME_LOCATION_HOST, "USA", "Zone"));       /* only 2 letters kept */
    REG_EQ_S("US", sltime_location_country());
    REG_EQ_I(0, sltime_set_location(SL_TIME_LOCATION_UTC, "U", NULL));            /* too short: cleared */
    REG_EQ_S("??", sltime_location_country());
    {   /* an over-long zone is truncated and always NUL-terminated */
        char zone[200]; memset(zone, 'z', sizeof zone - 1); zone[sizeof zone - 1] = 0;
        REG_EQ_I(0, sltime_set_location(SL_TIME_LOCATION_COUNTRY, "DE", zone));
        REG_EQ_U(63, strlen(sltime_location_timezone()));
    }
    REG_EQ_I(0, sltime_set_location(SL_TIME_LOCATION_UTC, NULL, NULL));           /* reset */
    REG_EQ_S("??", sltime_location_country());
}

static void t_sample(void) {
    SLTimeSample s; time_t now = time(NULL);
    REG_EQ_I(EINVAL, sltime_sample(NULL));
    REG_EQ_I(0, sltime_sample(&s));
    REG_CHECK(s.utc_ms > 0);
    REG_CHECK(llabs((long long)(s.utc_ms / 1000 - (long long)now)) <= 2);
    REG_EQ_U(SL_TIME_SOURCE_SYSTEM, s.source);
    REG_EQ_U(1000, s.uncertainty_us);
    REG_EQ_U(0, s.stratum);
    REG_EQ_S("??", s.country);
    REG_CHECK(s.timezone[0] != 0);
    REG_EQ_I(0, sltime_init());
}

static void t_clocks(void) {
    uint64_t prev = sltime_monotonic_nanos(); int64_t ms, ns;
    for (int i = 0; i < 20000; ++i) {
        uint64_t cur = sltime_monotonic_nanos();
        if (cur < prev) { REG_CHECK(!"monotonic clock went backwards"); break; }
        prev = cur;
    }
    ms = sltime_utc_millis(); ns = sltime_utc_nanos();
    REG_CHECK(ms > 1600000000000LL);                       /* after Sep 2020 */
    REG_CHECK(llabs(ns / 1000000 - ms) <= 200);
    REG_CHECK(llabs(ms / 1000 - (long long)time(NULL)) <= 2);
    {   uint64_t a = slmemory_monotonic_millis(), b;
        sltime_sleep_millis(5); b = slmemory_monotonic_millis();
        REG_CHECK(b >= a);
    }
}

static void t_sleep(void) {
    uint64_t t0, t1;
    t0 = sltime_monotonic_nanos();
    REG_EQ_I(0, sltime_sleep_millis(0)); REG_EQ_I(0, sltime_sleep_millis(-5));
    REG_EQ_I(0, sltime_sleep_millis(INT64_MIN));
    t1 = sltime_monotonic_nanos();
    REG_CHECK(t1 - t0 < 100u * 1000000u);                  /* non-positive returns immediately */
    t0 = sltime_monotonic_nanos();
    REG_EQ_I(0, sltime_sleep_millis(40));
    t1 = sltime_monotonic_nanos();
    REG_CHECK(t1 - t0 >= 40u * 1000000u);                  /* never returns early */
    REG_CHECK(t1 - t0 < 3000u * 1000000u);                 /* and does not hang */
}

static void t_format_golden(void) {
    static const struct { long long ms; const char *iso, *http; } g[] = {
        {0LL, "1970-01-01T00:00:00.000Z", "Thu, 01 Jan 1970 00:00:00 GMT"},
        {1LL, "1970-01-01T00:00:00.001Z", "Thu, 01 Jan 1970 00:00:00 GMT"},
        {999LL, "1970-01-01T00:00:00.999Z", "Thu, 01 Jan 1970 00:00:00 GMT"},
        {1000LL, "1970-01-01T00:00:01.000Z", "Thu, 01 Jan 1970 00:00:01 GMT"},
        {86399999LL, "1970-01-01T23:59:59.999Z", "Thu, 01 Jan 1970 23:59:59 GMT"},
        {1234567890000LL, "2009-02-13T23:31:30.000Z", "Fri, 13 Feb 2009 23:31:30 GMT"},
        {1700000000123LL, "2023-11-14T22:13:20.123Z", "Tue, 14 Nov 2023 22:13:20 GMT"},
        {951782400000LL, "2000-02-29T00:00:00.000Z", "Tue, 29 Feb 2000 00:00:00 GMT"},   /* leap, /400 */
        {951868799999LL, "2000-02-29T23:59:59.999Z", "Tue, 29 Feb 2000 23:59:59 GMT"},
        {1709164800000LL, "2024-02-29T00:00:00.000Z", "Thu, 29 Feb 2024 00:00:00 GMT"},
        {4102444800000LL, "2100-01-01T00:00:00.000Z", "Fri, 01 Jan 2100 00:00:00 GMT"},  /* 2100 not leap */
        {253402300799999LL, "9999-12-31T23:59:59.999Z", "Fri, 31 Dec 9999 23:59:59 GMT"},
        {-1000LL, "1969-12-31T23:59:59.000Z", "Wed, 31 Dec 1969 23:59:59 GMT"},
        {-2208988800000LL, "1900-01-01T00:00:00.000Z", "Mon, 01 Jan 1900 00:00:00 GMT"},  /* 1900 not leap */
    };
    char iso[64], http[64];
    for (size_t i = 0; i < sizeof g / sizeof g[0]; ++i) {
        reg_note_check();
        if (sltime_format_iso8601(g[i].ms, iso, sizeof iso) != 0 || strcmp(iso, g[i].iso) != 0)
            reg_fail(__FILE__, __LINE__, "iso8601(%lld) = \"%s\", want \"%s\"", g[i].ms, iso, g[i].iso);
        reg_note_check();
        if (sltime_http_date(g[i].ms, http, sizeof http) != 0 || strcmp(http, g[i].http) != 0)
            reg_fail(__FILE__, __LINE__, "http_date(%lld) = \"%s\", want \"%s\"", g[i].ms, http, g[i].http);
    }
}

static void t_format_buffer_contract(void) {
    char b[64];
    REG_EQ_I(EINVAL, sltime_format_iso8601(0, NULL, 64));
    REG_EQ_I(EINVAL, sltime_format_iso8601(0, b, 31));    /* needs >= 32 */
    REG_EQ_I(0, sltime_format_iso8601(0, b, 32)); REG_EQ_U(24, strlen(b));
    REG_EQ_I(EINVAL, sltime_http_date(0, NULL, 64));
    REG_EQ_I(EINVAL, sltime_http_date(0, b, 29));         /* needs >= 30 */
    REG_EQ_I(0, sltime_http_date(0, b, 30)); REG_EQ_U(29, strlen(b));
    {   /* writes stay inside the stated size */
        struct { char buf[32]; unsigned char guard[8]; } s; unsigned char canary[8];
        memset(&s, 0x5A, sizeof s); memset(canary, 0x5A, sizeof canary);
        REG_EQ_I(0, sltime_format_iso8601(1700000000123LL, s.buf, sizeof s.buf));
        REG_EQ_MEM(canary, s.guard, sizeof canary);
        memset(&s, 0x5A, sizeof s);
        REG_EQ_I(0, sltime_http_date(1700000000123LL, s.buf, 30));
        REG_EQ_MEM(canary, s.guard, sizeof canary);
    }
}

/* Calendar sweep across 1970..2262 against the independent oracle. */
static void t_format_calendar_sweep(void) {
    reg_seed(0x7131E5ull);
    for (int it = 0; it < 60000; ++it) {
        long long ms = (long long)(reg_next() % 9000000000000ull);   /* 1970 .. ~2255 */
        char want_iso[128], want_http[128], iso[48], http[48];
        oracle_iso(ms, want_iso, want_http);
        REG_REQUIRE(sltime_format_iso8601(ms, iso, sizeof iso) == 0);
        REG_REQUIRE(sltime_http_date(ms, http, sizeof http) == 0);
        reg_note_check();
        if (strcmp(iso, want_iso) != 0 || strcmp(http, want_http) != 0) {
            reg_fail(__FILE__, __LINE__, "ms=%lld got %s / %s want %s / %s", ms, iso, http, want_iso, want_http);
            return;
        }
    }
    /* every month boundary in 2023/2024 (leap year) */
    for (int month = 0; month < 24; ++month) {
        char want_iso[128], want_http[128], iso[48];
        long long ms = 1672531200000LL + (long long)month * 31LL * 86400000LL; /* sampled mid-range */
        oracle_iso(ms, want_iso, want_http);
        REG_REQUIRE(sltime_format_iso8601(ms, iso, sizeof iso) == 0);
        REG_EQ_S(want_iso, iso);
    }
}

/* KNOWN ISSUE: seconds are derived with C division (truncates toward zero) but
 * the millisecond field with a floored modulus, so any pre-1970 instant that is
 * not a whole second is formatted a second late (-1 ms -> ".999" of 1970-01-01
 * instead of 1969-12-31T23:59:59.999Z). */
static void t_known_negative_fractional_ms(void) {
    static const long long v[] = {-1, -999, -1001, -86400001, -1700000000123LL};
    int all_ok = 1;
    for (size_t i = 0; i < sizeof v / sizeof v[0]; ++i) {
        char want_iso[128], want_http[128], iso[48], http[48];
        oracle_iso(v[i], want_iso, want_http);
        (void)sltime_format_iso8601(v[i], iso, sizeof iso);
        (void)sltime_http_date(v[i], http, sizeof http);
        if (strcmp(iso, want_iso) != 0 || strcmp(http, want_http) != 0) all_ok = 0;
    }
    REG_KNOWN(all_ok, "TIME-NEG-FLOOR",
              "format_iso8601/http_date truncate instead of floor for negative non-whole-second instants");
}

static void t_json_and_records(void) {
    SLTimeSample s; char j[512]; uint8_t rec[40];
    const char *want = "{\"utc_ms\":1700000000123,\"monotonic_ns\":42,\"offset_ms\":-3600000,"
                       "\"uncertainty_us\":1000,\"source\":1,\"stratum\":2,\"country\":\"US\","
                       "\"timezone\":\"America/New_York\",\"source_host\":\"pool.ntp.org\"}";
    static const uint8_t bodi[32] = {0x00, 0x00, 0x01, 0x8B, 0xCF, 0xE5, 0x68, 0x7B,
                                     0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,
                                     0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02};
    memset(&s, 0, sizeof s);
    s.utc_ms = 1700000000123LL; s.monotonic_ns = 42; s.utc_offset_ms = -3600000; s.uncertainty_us = 1000;
    s.source = SL_TIME_SOURCE_NTP; s.stratum = 2;
    strcpy(s.country, "US"); strcpy(s.timezone, "America/New_York"); strcpy(s.source_host, "pool.ntp.org");
    REG_EQ_I(0, sltime_json(&s, j, sizeof j)); REG_EQ_S(want, j);
    REG_EQ_I(0, sltime_rmi_record(&s, j, sizeof j)); REG_EQ_S(want, j);
    REG_EQ_I(EINVAL, sltime_json(NULL, j, sizeof j));
    REG_EQ_I(EINVAL, sltime_json(&s, NULL, sizeof j));
    REG_EQ_I(EINVAL, sltime_json(&s, j, 63));              /* needs >= 64 */

    s.monotonic_ns = 0x0102030405060708ull;
    memset(rec, 0xEE, sizeof rec);
    REG_EQ_U(32, sltime_bodi_record(&s, rec, sizeof rec));
    REG_EQ_MEM(bodi, rec, 32);                             /* big-endian fixed layout */
    REG_EQ_U(0xEE, rec[32]);                               /* wrote exactly 32 bytes */
    REG_EQ_U(0, sltime_bodi_record(&s, rec, 31));
    REG_EQ_U(0, sltime_bodi_record(NULL, rec, 32));
    REG_EQ_U(0, sltime_bodi_record(&s, NULL, 32));
    {   uint8_t exact[32]; REG_EQ_U(32, sltime_bodi_record(&s, exact, 32)); REG_EQ_MEM(bodi, exact, 32); }
}

/* KNOWN ISSUE: string fields are interpolated into JSON without escaping, so a
 * timezone containing a quote produces a record that is not the same JSON
 * value (structure injection through sltime_set_location). */
static void t_known_json_escaping(void) {
    SLTimeSample s; char j[512];
    memset(&s, 0, sizeof s);
    strcpy(s.timezone, "x\"y");
    REG_EQ_I(0, sltime_json(&s, j, sizeof j));
    REG_KNOWN(strstr(j, "x\"y") == NULL, "TIME-JSON-ESCAPE",
              "sltime_json emits timezone/country/source_host verbatim, unescaped");
}

/* ---- memory ------------------------------------------------------------- */

static void t_memory_platform(void) {
#if defined(__APPLE__)
    REG_EQ_S("macos-posix", slmemory_platform_name()); REG_EQ_I(SL_MEMORY_MACOS, slmemory_platform());
#else
    REG_EQ_S("linux-posix", slmemory_platform_name()); REG_EQ_I(SL_MEMORY_LINUX, slmemory_platform());
#endif
    REG_EQ_I(1, slmemory_platform_is_available(SL_MEMORY_AUTO));
    REG_EQ_I(1, slmemory_platform_is_available(slmemory_platform()));
    REG_EQ_I(0, slmemory_platform_is_available(SL_MEMORY_WINDOWS));
    {   size_t p = slmemory_page_size();
        REG_CHECK(p >= 1024 && (p & (p - 1)) == 0);       /* a power of two */
    }
}

static void t_memory_alloc_semantics(void) {
    unsigned char *p = (unsigned char *)slmemory_alloc(100), *z, *g;
    REG_REQUIRE(p != NULL);
    memset(p, 0x11, 100);                                  /* guard bytes catch overruns on free */
    z = (unsigned char *)slmemory_calloc(64, 8);
    REG_REQUIRE(z != NULL);
    for (size_t i = 0; i < 512; ++i) if (z[i] != 0) { REG_CHECK(!"calloc memory not zeroed"); break; }
    REG_CHECK(slmemory_calloc((size_t)-1, 2) == NULL);     /* n*size overflow is refused */
    g = (unsigned char *)slmemory_realloc(p, 4000);        /* grow: prefix preserved */
    REG_REQUIRE(g != NULL);
    for (size_t i = 0; i < 100; ++i) if (g[i] != 0x11) { REG_CHECK(!"realloc lost data"); break; }
    memset(g, 0x22, 4000);
    p = (unsigned char *)slmemory_realloc(g, 10);          /* shrink: prefix preserved */
    REG_REQUIRE(p != NULL);
    for (size_t i = 0; i < 10; ++i) if (p[i] != 0x22) { REG_CHECK(!"realloc shrink lost data"); break; }
    slmemory_free(p); slmemory_free(z); slmemory_free(NULL);
    p = (unsigned char *)slmemory_realloc(NULL, 8);        /* realloc(NULL) == alloc */
    REG_CHECK(p != NULL); slmemory_free(p);
}

static void t_memory_alloc_failure_paths(void) {
    reg_alloc_fail_nth(1);
    REG_CHECK(slmemory_alloc(16) == NULL); REG_CHECK(reg_alloc_injected());
    reg_alloc_fail_nth(1);
    REG_CHECK(slmemory_calloc(4, 4) == NULL);
    {   void *p = slmemory_alloc(16); REG_REQUIRE(p != NULL);
        reg_alloc_fail_nth(1);
        REG_CHECK(slmemory_realloc(p, 4096) == NULL);      /* original block must stay valid */
        reg_alloc_fail_nth(0);
        memset(p, 1, 16); slmemory_free(p);
    }
    reg_alloc_fail_nth(0);
}

static void t_memory_aligned(void) {
    static const size_t good[] = {8, 16, 32, 64, 128, 4096, 65536};
    for (size_t i = 0; i < sizeof good / sizeof good[0]; ++i) {
        void *p = slmemory_aligned_alloc(good[i], 1000);
        reg_note_check();
        if (!p || ((uintptr_t)p % good[i]) != 0)
            reg_fail(__FILE__, __LINE__, "alignment %zu not honoured (%p)", good[i], p);
        memset(p, 0x7E, 1000);
        slmemory_aligned_free(p);
    }
    REG_CHECK(slmemory_aligned_alloc(0, 64) == NULL);      /* zero alignment */
    REG_CHECK(slmemory_aligned_alloc(24, 64) == NULL);     /* not a power of two */
    REG_CHECK(slmemory_aligned_alloc(3, 64) == NULL);
    slmemory_aligned_free(NULL);
}

static void t_memory_monotonic_millis(void) {
    uint64_t prev = slmemory_monotonic_millis();
    for (int i = 0; i < 5000; ++i) {
        uint64_t c = slmemory_monotonic_millis();
        if (c < prev) { REG_CHECK(!"slmemory_monotonic_millis went backwards"); break; }
        prev = c;
    }
}

int main(void) {
    REG_RUN(t_location_defaults_then_set);   /* first: touches global state */
    REG_RUN(t_sample);
    REG_RUN(t_clocks);
    REG_RUN(t_sleep);
    REG_RUN(t_format_golden);
    REG_RUN(t_format_buffer_contract);
    REG_RUN(t_format_calendar_sweep);
    REG_RUN(t_known_negative_fractional_ms);
    REG_RUN(t_json_and_records);
    REG_RUN(t_known_json_escaping);
    REG_RUN(t_memory_platform);
    REG_RUN(t_memory_alloc_semantics);
    REG_RUN(t_memory_alloc_failure_paths);
    REG_RUN(t_memory_aligned);
    REG_RUN(t_memory_monotonic_millis);
    return reg_finish("time_memory");
}
