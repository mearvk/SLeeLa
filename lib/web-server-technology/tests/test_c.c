/*
 * lib/web-server-technology/tests/test_c.c
 * Smoke tests for the native C core (slweb.h). Exit 0 = all pass.
 */
#include "slweb.h"

#include <stdio.h>
#include <string.h>

static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { printf("FAIL: %s\n", m); ++fails; } \
                         else printf("ok:   %s\n", m); } while (0)

static void test_envelopment(void) {
  slweb_envelopment *e = slweb_env_new("science.example");
  CHECK(e != NULL, "env created");
  CHECK(slweb_env_add_program(e, "landing", SLWEB_PROGRAM_MEME, "/", "<h1>hi</h1>", 0) == SLWEB_OK, "add meme program");
  CHECK(slweb_env_add_program(e, "lab", SLWEB_PROGRAM_SQL, "/lab", "/srv/db", 0) == SLWEB_OK, "add sql program");
  CHECK(slweb_env_add_program(e, "lab", SLWEB_PROGRAM_SQL, "/lab", "/srv/db", 0) == SLWEB_ERR_ARG, "duplicate program rejected");
  CHECK(slweb_env_add_warrant(e, "publish", "science.example", "/", "landing", 1) == SLWEB_OK, "add landing warrant");
  CHECK(slweb_env_add_warrant(e, "expose-lab", "science.example", "/lab", "lab", 1) == SLWEB_OK, "add lab warrant");

  CHECK(slweb_env_active(e) == 0, "inactive: enabled+scientist not set");
  slweb_env_set_enabled(e, 1);
  CHECK(slweb_env_active(e) == 0, "inactive: scientist gate still required");
  slweb_env_set_scientist(e, 1);
  CHECK(slweb_env_active(e) == 1, "active: enabled+scientist+warrants");

  char prog[SLWEB_MAX_NAME], cause[SLWEB_MAX_PATH];
  slweb_program_kind kind;
  /* longest-prefix: /lab/x should pick lab, not landing (/). */
  CHECK(slweb_env_resolve(e, "science.example", "/lab/db/query", prog, cause, &kind) == SLWEB_OK, "resolve /lab");
  CHECK(strcmp(prog, "lab") == 0, "longest-prefix picked lab");
  CHECK(kind == SLWEB_PROGRAM_SQL, "lab is sql kind");
  /* root picks landing. */
  CHECK(slweb_env_resolve(e, "science.example", "/", prog, cause, &kind) == SLWEB_OK, "resolve /");
  CHECK(strcmp(prog, "landing") == 0, "root picked landing");
  /* wrong host: no match. */
  CHECK(slweb_env_resolve(e, "other.example", "/", prog, cause, &kind) == SLWEB_ERR_NOTFOUND, "wrong host no match");

  slweb_env_free(e);
}

static void test_rate(void) {
  slweb_rate *r = slweb_rate_new(3, 0, 1000); /* 3/sec per peer */
  CHECK(slweb_rate_enabled(r), "rate enabled");
  CHECK(slweb_rate_allow(r, "1.1.1.1", 0), "req1 allowed");
  CHECK(slweb_rate_allow(r, "1.1.1.1", 10), "req2 allowed");
  CHECK(slweb_rate_allow(r, "1.1.1.1", 20), "req3 allowed");
  CHECK(!slweb_rate_allow(r, "1.1.1.1", 30), "req4 rejected");
  CHECK(slweb_rate_allow(r, "2.2.2.2", 30), "other peer ok");
  CHECK(slweb_rate_allow(r, "1.1.1.1", 1100), "allowed after window");
  slweb_rate_free(r);
}

static void test_auth(void) {
  CHECK(slweb_constant_time_equals("abc", "abc"), "ct equal");
  CHECK(!slweb_constant_time_equals("abc", "abd"), "ct mismatch");
  CHECK(!slweb_constant_time_equals("abc", "abcd"), "ct length mismatch");
  CHECK(slweb_write_authorize(NULL, ""), "no token => authorized");
  CHECK(slweb_write_authorize("tok", "Bearer tok"), "correct token");
  CHECK(slweb_write_authorize("tok", "bearer tok"), "scheme case-insensitive");
  CHECK(!slweb_write_authorize("tok", "Bearer no"), "wrong token");
  CHECK(!slweb_write_authorize("tok", "tok"), "missing scheme");
}

static void test_client_ip(void) {
  const char *proxies[] = {"10.0.0.1"};
  char out[64];
  slweb_client_ip("203.0.113.9", "", proxies, 1, out, sizeof out);
  CHECK(strcmp(out, "203.0.113.9") == 0, "no xff => peer");
  slweb_client_ip("10.0.0.1", "198.51.100.7", proxies, 1, out, sizeof out);
  CHECK(strcmp(out, "198.51.100.7") == 0, "trusted proxy => forwarded");
  slweb_client_ip("203.0.113.9", "198.51.100.7", proxies, 1, out, sizeof out);
  CHECK(strcmp(out, "203.0.113.9") == 0, "untrusted peer cannot spoof");
  slweb_client_ip("10.0.0.1", "198.51.100.7, 10.0.0.2", proxies, 1, out, sizeof out);
  CHECK(strcmp(out, "198.51.100.7") == 0, "left-most of list");
}

static void test_warm(void) {
  slweb_warm w = slweb_warm_default();
  CHECK(w.enabled && w.floor_pct == 11 && w.ceiling_pct == 14, "default 11-14 on");
  slweb_warm bad = {1, 200, 1, 99, 5}; /* out of range */
  slweb_warm s = slweb_warm_sanitize(bad);
  CHECK(s.floor_pct <= 90 && s.ceiling_pct >= s.floor_pct && s.ceiling_pct <= 95, "floor/ceiling clamped");
  CHECK(s.max_cores >= 1 && s.max_cores <= 16, "cores clamped 1..16");
  CHECK(s.window_ms >= 50 && s.window_ms <= 2000, "window clamped");
  CHECK(slweb_warm_target_pct(slweb_warm_default()) == 12.5, "target is band midpoint");
  CHECK(slweb_warm_core_count(16) >= 1, "core count >= 1");
  CHECK(slweb_warm_core_count(2) <= 2, "core count respects cap");
}

int main(void) {
  printf("slweb C core version %s\n", slweb_version());
  test_envelopment();
  test_rate();
  test_auth();
  test_client_ip();
  test_warm();
  if (fails) { printf("\n%d test(s) FAILED\n", fails); return 1; }
  printf("\nall slweb C tests passed\n");
  return 0;
}
