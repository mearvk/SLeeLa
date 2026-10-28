/*
 * lib/web-server-technology/tests/test_cpp.cpp
 * Smoke tests for the C++17 facade (slweb.hpp). Exit 0 = all pass.
 */
#include "slweb.hpp"

#include <cstdio>

using namespace sleela::web;

static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { std::printf("FAIL: %s\n", m); ++fails; } \
                         else std::printf("ok:   %s\n", m); } while (0)

int main() {
  std::printf("slweb C++ facade version %s\n", version().c_str());

  Envelopment env("science.example");
  CHECK(env.add_program("landing", ProgramKind::Meme, "/", "<h1>hi</h1>"), "add meme");
  CHECK(env.add_program("lab", ProgramKind::Sql, "/lab", "/srv/db"), "add sql");
  CHECK(env.add_warrant("publish", "science.example", "/", "landing"), "add landing warrant");
  CHECK(env.add_warrant("expose", "science.example", "/lab", "lab"), "add lab warrant");

  CHECK(!env.active(), "inactive without gates");
  env.set_enabled(true);
  env.set_scientist(true);
  CHECK(env.active(), "active with both gates + warrants");

  auto r = env.resolve("science.example", "/lab/db/query");
  CHECK(r.has_value() && r->program == "lab" && r->kind == ProgramKind::Sql, "longest-prefix -> lab(sql)");
  auto root = env.resolve("science.example", "/");
  CHECK(root.has_value() && root->program == "landing", "root -> landing");
  CHECK(!env.resolve("other.example", "/").has_value(), "wrong host -> none");

  RateLimiter rl(2, 0, std::chrono::milliseconds(1000));
  CHECK(rl.allow("a", 0), "rl req1");
  CHECK(rl.allow("a", 1), "rl req2");
  CHECK(!rl.allow("a", 2), "rl req3 rejected");

  WriteAuth wa;
  CHECK(wa.authorize("anything"), "no token => ok");
  wa.set_token("sekret");
  CHECK(wa.required(), "token required");
  CHECK(wa.authorize("Bearer sekret"), "correct token");
  CHECK(!wa.authorize("Bearer nope"), "wrong token");

  CHECK(client_ip("10.0.0.1", "9.9.9.9", {"10.0.0.1"}) == "9.9.9.9", "trusted proxy client ip");
  CHECK(client_ip("3.3.3.3", "9.9.9.9", {"10.0.0.1"}) == "3.3.3.3", "untrusted peer no spoof");

  Warm w = Warm::make_default();
  CHECK(w.enabled && w.floor_pct == 11 && w.ceiling_pct == 14, "warm default 11-14");
  CHECK(w.target_pct() == 12.5, "warm target midpoint");

  if (fails) { std::printf("\n%d test(s) FAILED\n", fails); return 1; }
  std::printf("\nall slweb C++ tests passed\n");
  return 0;
}
