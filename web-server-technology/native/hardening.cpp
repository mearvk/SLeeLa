/*
 * web-server-technology/native/hardening.cpp
 *
 * Implementation of the Section-3 hardening primitives. Transport-agnostic and
 * independently testable (see hardening-test.cpp).
 */
#include "hardening.h"

#include <algorithm>
#include <cctype>
#include <fstream>

namespace sleela {
namespace wst {

// ---- primitives: delegate to the shared C core (slweb) ----------------------
// RateLimiter is implemented inline in the header as a thread-safe wrapper over
// slweb_rate; the three functions below forward to the same C core so the
// server and lib/web-server-technology share ONE implementation.

bool constant_time_equals(const std::string &a, const std::string &b) {
  return slweb_constant_time_equals(a.c_str(), b.c_str()) != 0;
}

bool WriteAuth::authorize(const std::string &authorization_header) const {
  return slweb_write_authorize(token_.empty() ? nullptr : token_.c_str(),
                               authorization_header.c_str()) != 0;
}

std::string client_ip_from(const std::string &peer_ip,
                           const std::string &forwarded_for,
                           const std::vector<std::string> &trusted_proxies) {
  std::vector<const char *> ptrs;
  ptrs.reserve(trusted_proxies.size());
  for (const auto &p : trusted_proxies) ptrs.push_back(p.c_str());
  char out[64];
  if (slweb_client_ip(peer_ip.c_str(), forwarded_for.c_str(),
                      ptrs.empty() ? nullptr : ptrs.data(), ptrs.size(), out,
                      sizeof out) != SLWEB_OK)
    return peer_ip;
  return std::string(out);
}

// ---- hardening config file --------------------------------------------------
namespace {
std::string h_trim(const std::string &s) {
  size_t a = 0, b = s.size();
  while (a < b && std::isspace((unsigned char)s[a])) ++a;
  while (b > a && std::isspace((unsigned char)s[b - 1])) --b;
  return s.substr(a, b - a);
}
std::string h_lower(std::string s) {
  for (char &c : s) c = (char)std::tolower((unsigned char)c);
  return s;
}
bool parse_uint(const std::string &v, unsigned &out) {
  if (v.empty()) return false;
  unsigned long n = 0;
  for (char c : v) {
    if (c < '0' || c > '9') return false;
    n = n * 10 + (unsigned)(c - '0');
    if (n > 4000000000UL) return false;
  }
  out = (unsigned)n;
  return true;
}
}  // namespace

bool load_hardening_config(const std::string &path, HardeningSettings &out,
                           std::string &err) {
  std::ifstream f(path);
  if (!f) { err = "cannot open config file: " + path; return false; }
  std::string line;
  int lineno = 0;
  while (std::getline(f, line)) {
    ++lineno;
    size_t hash = line.find('#');
    if (hash != std::string::npos) line = line.substr(0, hash);
    std::string t = h_trim(line);
    if (t.empty()) continue;
    size_t eq = t.find('=');
    if (eq == std::string::npos) continue;  // not key=value; leave to others
    std::string k = h_lower(h_trim(t.substr(0, eq)));
    std::string v = h_trim(t.substr(eq + 1));
    if (k == "rate-per-peer") {
      if (!parse_uint(v, out.rate_per_peer)) { err = "line " + std::to_string(lineno) + ": rate-per-peer must be a number"; return false; }
      out.has_rate_per_peer = true;
    } else if (k == "rate-global") {
      if (!parse_uint(v, out.rate_global)) { err = "line " + std::to_string(lineno) + ": rate-global must be a number"; return false; }
      out.has_rate_global = true;
    } else if (k == "rate-window-ms") {
      if (!parse_uint(v, out.rate_window_ms)) { err = "line " + std::to_string(lineno) + ": rate-window-ms must be a number"; return false; }
      out.has_rate_window = true;
    } else if (k == "write-token") {
      out.write_token = v;
      out.has_write_token = true;
    } else if (k == "trusted-proxy") {
      if (!v.empty()) out.trusted_proxies.push_back(v);
    } else if (k == "cpu-warm") {
      std::string lv = h_lower(v);
      if (lv == "on" || lv == "true" || lv == "yes" || lv == "1") { out.warm_enabled = true; out.has_warm = true; }
      else if (lv == "off" || lv == "false" || lv == "no" || lv == "0") { out.warm_enabled = false; out.has_warm = true; }
      else { err = "line " + std::to_string(lineno) + ": cpu-warm must be on/off"; return false; }
    } else if (k == "cpu-warm-floor") {
      if (!parse_uint(v, out.warm_floor)) { err = "line " + std::to_string(lineno) + ": cpu-warm-floor must be a number"; return false; }
      out.has_warm_floor = true;
    } else if (k == "cpu-warm-ceiling") {
      if (!parse_uint(v, out.warm_ceiling)) { err = "line " + std::to_string(lineno) + ": cpu-warm-ceiling must be a number"; return false; }
      out.has_warm_ceiling = true;
    } else if (k == "cpu-warm-cores") {
      if (!parse_uint(v, out.warm_cores)) { err = "line " + std::to_string(lineno) + ": cpu-warm-cores must be a number"; return false; }
      out.has_warm_cores = true;
    }
    // all other keys ignored
  }
  return true;
}

}  // namespace wst
}  // namespace sleela
