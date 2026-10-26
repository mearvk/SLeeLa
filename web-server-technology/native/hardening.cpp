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

// ---- RateLimiter ------------------------------------------------------------
void RateLimiter::trim(std::deque<clock::time_point> &dq,
                       clock::time_point cutoff) {
  while (!dq.empty() && dq.front() <= cutoff) dq.pop_front();
}

bool RateLimiter::allow(const std::string &peer, clock::time_point now) {
  std::lock_guard<std::mutex> g(m_);
  if (per_peer_max_ == 0 && global_max_ == 0) return true;  // disabled
  const clock::time_point cutoff = now - window_;

  // Check global window first (without mutating until we know we admit).
  trim(global_, cutoff);
  if (global_max_ != 0 && global_.size() >= global_max_) return false;

  auto &pq = peers_[peer];
  trim(pq, cutoff);
  if (per_peer_max_ != 0 && pq.size() >= per_peer_max_) {
    // Rejected: do not count it, so a sustained flood from one peer does not
    // also consume the global budget or keep its own window pinned full.
    if (pq.empty()) peers_.erase(peer);
    return false;
  }

  // Admit: record in both windows.
  if (global_max_ != 0) global_.push_back(now);
  pq.push_back(now);
  return true;
}

void RateLimiter::sweep(clock::time_point now) {
  std::lock_guard<std::mutex> g(m_);
  const clock::time_point cutoff = now - window_;
  trim(global_, cutoff);
  for (auto it = peers_.begin(); it != peers_.end();) {
    trim(it->second, cutoff);
    if (it->second.empty())
      it = peers_.erase(it);
    else
      ++it;
  }
}

// ---- constant-time compare & WriteAuth --------------------------------------
bool constant_time_equals(const std::string &a, const std::string &b) {
  // Compare over a fixed number of iterations so the running time depends on
  // the max length, not on where the first mismatch is. A length difference is
  // folded into the accumulator rather than short-circuiting.
  const size_t n = std::max(a.size(), b.size());
  unsigned diff = (a.size() == b.size()) ? 0u : 1u;
  for (size_t i = 0; i < n; ++i) {
    unsigned char ca = i < a.size() ? (unsigned char)a[i] : 0;
    unsigned char cb = i < b.size() ? (unsigned char)b[i] : 0;
    diff |= (unsigned)(ca ^ cb);
  }
  return diff == 0;
}

bool WriteAuth::authorize(const std::string &authorization_header) const {
  if (token_.empty()) return true;  // no token configured => no gate
  // Expect "Bearer <token>" (scheme is case-insensitive per RFC 7617/6750).
  const std::string &h = authorization_header;
  // find first non-space
  size_t i = 0;
  while (i < h.size() && std::isspace((unsigned char)h[i])) ++i;
  // scheme
  size_t s = i;
  while (s < h.size() && !std::isspace((unsigned char)h[s])) ++s;
  std::string scheme = h.substr(i, s - i);
  for (char &c : scheme) c = (char)std::tolower((unsigned char)c);
  if (scheme != "bearer") return false;
  // skip spaces to the credential
  while (s < h.size() && std::isspace((unsigned char)h[s])) ++s;
  std::string presented = h.substr(s);
  // trim trailing whitespace from the credential
  while (!presented.empty() && std::isspace((unsigned char)presented.back()))
    presented.pop_back();
  return constant_time_equals(presented, token_);
}

// ---- client IP from a trusted proxy -----------------------------------------
std::string client_ip_from(const std::string &peer_ip,
                           const std::string &forwarded_for,
                           const std::vector<std::string> &trusted_proxies) {
  if (forwarded_for.empty()) return peer_ip;
  const bool peer_trusted =
      std::find(trusted_proxies.begin(), trusted_proxies.end(), peer_ip) !=
      trusted_proxies.end();
  if (!peer_trusted) return peer_ip;  // do not trust a non-proxy's claim
  // forwarded_for may be a comma list "client, proxy1, proxy2"; the left-most
  // is the originating client as recorded by the trusted terminator.
  std::string v = forwarded_for;
  size_t comma = v.find(',');
  if (comma != std::string::npos) v = v.substr(0, comma);
  // trim
  size_t a = 0, b = v.size();
  while (a < b && std::isspace((unsigned char)v[a])) ++a;
  while (b > a && std::isspace((unsigned char)v[b - 1])) --b;
  v = v.substr(a, b - a);
  return v.empty() ? peer_ip : v;
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
