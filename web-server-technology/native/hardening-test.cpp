/*
 * web-server-technology/native/hardening-test.cpp
 *
 * Unit tests for the Section-3 hardening primitives. No sockets; pure logic.
 * Build: see the `check` target in native/Makefile. Exit 0 = all pass.
 */
#include "hardening.h"

#include <cassert>
#include <cstdio>

using namespace sleela::wst;
using clock_t_ = RateLimiter::clock;

static int failures = 0;
#define CHECK(cond, msg)                                                   \
  do {                                                                     \
    if (!(cond)) { std::printf("FAIL: %s\n", msg); ++failures; }           \
    else { std::printf("ok:   %s\n", msg); }                               \
  } while (0)

static void test_rate_limiter_per_peer() {
  RateLimiter rl(3, 0, std::chrono::milliseconds(1000));  // 3/sec per peer
  auto t0 = clock_t_::now();
  CHECK(rl.allow("1.1.1.1", t0), "per-peer req 1 allowed");
  CHECK(rl.allow("1.1.1.1", t0), "per-peer req 2 allowed");
  CHECK(rl.allow("1.1.1.1", t0), "per-peer req 3 allowed");
  CHECK(!rl.allow("1.1.1.1", t0), "per-peer req 4 rejected (over cap)");
  // A different peer is independent.
  CHECK(rl.allow("2.2.2.2", t0), "other peer unaffected");
  // After the window passes, the first peer is allowed again.
  auto t1 = t0 + std::chrono::milliseconds(1001);
  CHECK(rl.allow("1.1.1.1", t1), "per-peer allowed after window elapses");
}

static void test_rate_limiter_global() {
  RateLimiter rl(0, 2, std::chrono::milliseconds(1000));  // 2/sec global
  auto t0 = clock_t_::now();
  CHECK(rl.allow("a", t0), "global req 1 allowed");
  CHECK(rl.allow("b", t0), "global req 2 allowed (diff peer)");
  CHECK(!rl.allow("c", t0), "global req 3 rejected (global cap)");
}

static void test_rate_limiter_disabled() {
  RateLimiter rl(0, 0, std::chrono::milliseconds(1000));  // disabled
  auto t0 = clock_t_::now();
  bool all = true;
  for (int i = 0; i < 1000; ++i) all = all && rl.allow("x", t0);
  CHECK(all, "disabled limiter allows 1000 straight requests");
}

static void test_constant_time_equals() {
  CHECK(constant_time_equals("secret", "secret"), "equal strings match");
  CHECK(!constant_time_equals("secret", "secrff"), "same-length mismatch");
  CHECK(!constant_time_equals("secret", "secretx"), "length mismatch rejected");
  CHECK(!constant_time_equals("", "x"), "empty vs nonempty");
  CHECK(constant_time_equals("", ""), "empty equals empty");
}

static void test_write_auth() {
  WriteAuth noauth;
  CHECK(noauth.authorize(""), "no token => always authorized");
  CHECK(!noauth.required(), "no token => not required");

  WriteAuth a;
  a.set_token("s3cr3t");
  CHECK(a.required(), "token set => required");
  CHECK(a.authorize("Bearer s3cr3t"), "correct bearer token authorized");
  CHECK(a.authorize("bearer s3cr3t"), "scheme is case-insensitive");
  CHECK(a.authorize("Bearer   s3cr3t  "), "extra whitespace tolerated");
  CHECK(!a.authorize("Bearer wrong"), "wrong token rejected");
  CHECK(!a.authorize("s3cr3t"), "missing Bearer scheme rejected");
  CHECK(!a.authorize(""), "absent header rejected when required");
  CHECK(!a.authorize("Basic s3cr3t"), "wrong scheme rejected");
}

static void test_client_ip() {
  std::vector<std::string> proxies = {"10.0.0.1"};
  // No forwarded header: peer is authoritative.
  CHECK(client_ip_from("203.0.113.9", "", proxies) == "203.0.113.9",
        "no XFF => peer ip");
  // Peer is a trusted proxy: forwarded client is used.
  CHECK(client_ip_from("10.0.0.1", "198.51.100.7", proxies) == "198.51.100.7",
        "trusted proxy => forwarded client ip");
  // Peer NOT trusted: forwarded header is ignored (anti-spoofing).
  CHECK(client_ip_from("203.0.113.9", "198.51.100.7", proxies) == "203.0.113.9",
        "untrusted peer cannot spoof via XFF");
  // Comma list from trusted proxy: left-most (origin) wins.
  CHECK(client_ip_from("10.0.0.1", "198.51.100.7, 10.0.0.2", proxies) ==
            "198.51.100.7",
        "left-most of XFF list is the origin client");
}

int main() {
  test_rate_limiter_per_peer();
  test_rate_limiter_global();
  test_rate_limiter_disabled();
  test_constant_time_equals();
  test_write_auth();
  test_client_ip();
  if (failures) {
    std::printf("\n%d test(s) FAILED\n", failures);
    return 1;
  }
  std::printf("\nall hardening unit tests passed\n");
  return 0;
}
