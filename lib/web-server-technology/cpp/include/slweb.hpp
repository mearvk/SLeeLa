/*
 * lib/web-server-technology/cpp/include/slweb.hpp
 *
 * SLeeLa Web Server Technology -- C++17 RAII facade over the native C ABI
 * (../../native/include/slweb.h). Same model, idiomatic C++: owning handles,
 * std::string, std::optional, exceptions only at construction on OOM.
 *
 * Link against both the C++ facade and the C core (as with lib/sleela-sql).
 */
#ifndef SLEELA_SLWEB_HPP
#define SLEELA_SLWEB_HPP

#include "slweb.h"

#include <chrono>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace sleela {
namespace web {

inline std::string version() { return slweb_version(); }

enum class ProgramKind {
  Meme = SLWEB_PROGRAM_MEME,
  Static = SLWEB_PROGRAM_STATIC,
  Sql = SLWEB_PROGRAM_SQL
};

inline std::string program_kind_name(ProgramKind k) {
  return slweb_program_kind_name((slweb_program_kind)k);
}

/* The result of resolving a request against an envelopment. */
struct Resolution {
  std::string program;
  std::string cause;
  ProgramKind kind;
};

/* RAII wrapper over slweb_envelopment. Non-copyable, movable. */
class Envelopment {
 public:
  explicit Envelopment(const std::string &domain = "")
      : e_(slweb_env_new(domain.c_str())) {
    if (!e_) throw std::bad_alloc();
  }
  ~Envelopment() { slweb_env_free(e_); }
  Envelopment(const Envelopment &) = delete;
  Envelopment &operator=(const Envelopment &) = delete;
  Envelopment(Envelopment &&o) noexcept : e_(o.e_) { o.e_ = nullptr; }
  Envelopment &operator=(Envelopment &&o) noexcept {
    if (this != &o) { slweb_env_free(e_); e_ = o.e_; o.e_ = nullptr; }
    return *this;
  }

  void set_enabled(bool v) { slweb_env_set_enabled(e_, v ? 1 : 0); }
  void set_scientist(bool v) { slweb_env_set_scientist(e_, v ? 1 : 0); }
  bool enabled() const { return slweb_env_enabled(e_) != 0; }
  bool scientist() const { return slweb_env_scientist(e_) != 0; }

  bool add_program(const std::string &name, ProgramKind kind,
                   const std::string &mount, const std::string &resource = "",
                   bool writable = false) {
    return slweb_env_add_program(e_, name.c_str(), (slweb_program_kind)kind,
                                 mount.c_str(), resource.c_str(),
                                 writable ? 1 : 0) == SLWEB_OK;
  }
  bool add_warrant(const std::string &cause, const std::string &subject,
                   const std::string &path_prefix, const std::string &program,
                   bool enabled = true) {
    return slweb_env_add_warrant(e_, cause.c_str(), subject.c_str(),
                                 path_prefix.c_str(), program.c_str(),
                                 enabled ? 1 : 0) == SLWEB_OK;
  }

  std::size_t program_count() const { return slweb_env_program_count(e_); }
  std::size_t warrant_count() const { return slweb_env_warrant_count(e_); }
  bool active() const { return slweb_env_active(e_) != 0; }

  /* Resolve (host, path); nullopt when no warrant matches or inactive. */
  std::optional<Resolution> resolve(const std::string &host,
                                    const std::string &path) const {
    char prog[SLWEB_MAX_NAME], cause[SLWEB_MAX_PATH];
    slweb_program_kind kind;
    if (slweb_env_resolve(e_, host.c_str(), path.c_str(), prog, cause, &kind) !=
        SLWEB_OK)
      return std::nullopt;
    return Resolution{std::string(prog), std::string(cause), (ProgramKind)kind};
  }

  slweb_envelopment *handle() { return e_; }

 private:
  slweb_envelopment *e_;
};

/* RAII wrapper over slweb_rate. */
class RateLimiter {
 public:
  RateLimiter(unsigned per_peer_max, unsigned global_max,
              std::chrono::milliseconds window)
      : r_(slweb_rate_new(per_peer_max, global_max,
                          (unsigned)window.count())) {
    if (!r_) throw std::bad_alloc();
  }
  ~RateLimiter() { slweb_rate_free(r_); }
  RateLimiter(const RateLimiter &) = delete;
  RateLimiter &operator=(const RateLimiter &) = delete;

  bool enabled() const { return slweb_rate_enabled(r_) != 0; }
  bool allow(const std::string &peer, unsigned long long now_ms) {
    return slweb_rate_allow(r_, peer.c_str(), now_ms) != 0;
  }
  void sweep(unsigned long long now_ms) { slweb_rate_sweep(r_, now_ms); }

 private:
  slweb_rate *r_;
};

inline bool constant_time_equals(const std::string &a, const std::string &b) {
  return slweb_constant_time_equals(a.c_str(), b.c_str()) != 0;
}

/* Bearer-token write authorization (empty token => always authorized). */
class WriteAuth {
 public:
  void set_token(const std::string &t) { token_ = t; }
  bool required() const { return !token_.empty(); }
  bool authorize(const std::string &authorization_header) const {
    return slweb_write_authorize(token_.empty() ? nullptr : token_.c_str(),
                                 authorization_header.c_str()) != 0;
  }

 private:
  std::string token_;
};

/* Effective client IP, honouring only trusted proxies' forwarded header. */
inline std::string client_ip(const std::string &peer_ip,
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

/* Keep-warm band policy (value type; the threaded controller lives in the
 * server). */
struct Warm {
  bool enabled = true;
  unsigned floor_pct = 11;
  unsigned ceiling_pct = 14;
  unsigned max_cores = 16;
  unsigned window_ms = 200;

  static Warm make_default() {
    slweb_warm w = slweb_warm_default();
    return {w.enabled != 0, w.floor_pct, w.ceiling_pct, w.max_cores, w.window_ms};
  }
  Warm sanitized() const {
    slweb_warm w{enabled ? 1 : 0, floor_pct, ceiling_pct, max_cores, window_ms};
    w = slweb_warm_sanitize(w);
    return {w.enabled != 0, w.floor_pct, w.ceiling_pct, w.max_cores, w.window_ms};
  }
  double target_pct() const {
    slweb_warm w{enabled ? 1 : 0, floor_pct, ceiling_pct, max_cores, window_ms};
    return slweb_warm_target_pct(w);
  }
  static unsigned core_count(unsigned max) { return slweb_warm_core_count(max); }
};

}  // namespace web
}  // namespace sleela
#endif  // SLEELA_SLWEB_HPP
