#ifndef SLEELA_WST_HARDENING_H
#define SLEELA_WST_HARDENING_H
/*
 * web-server-technology/native/hardening.h
 *
 * Section-3 production-hardening building blocks, kept as a self-contained,
 * transport-agnostic module so they can be unit-tested on their own and so the
 * base server depends on them only through small, explicit seams:
 *
 *   RateLimiter  -- a global and per-peer sliding-window request limiter that
 *                   provides real admission control beyond the connection-queue
 *                   back-pressure.
 *   WriteAuth    -- a bearer-token gate for mutating /db use, compared in
 *                   constant time so a token is not leaked by timing.
 *   client_ip_from -- extracts the effective client IP, honouring a trusted
 *                   reverse proxy's Forwarded / X-Forwarded-For header only when
 *                   the immediate peer is an allowlisted proxy (so the server
 *                   behaves correctly behind a TLS-terminating front end without
 *                   letting arbitrary clients spoof their address).
 *
 * None of this includes sockets or HTTP wire handling.
 */
#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "../../lib/web-server-technology/native/include/slweb.h"

namespace sleela {
namespace wst {

/*
 * Sliding-window rate limiter. Two independent windows are enforced:
 *   - a global cap:   at most `global_max` requests per `window` across all peers;
 *   - a per-peer cap: at most `per_peer_max` requests per `window` for each IP.
 * A limit of 0 disables that dimension. Thread-safe.
 *
 * This is a thin, thread-safe C++ wrapper over the shared C core
 * (slweb_rate in lib/web-server-technology): the server and the library now use
 * ONE implementation of the sliding-window logic. The wrapper adds the mutex
 * (the C core is single-threaded) and converts steady_clock time points to the
 * monotonic millisecond counter the C ABI takes.
 */
class RateLimiter {
 public:
  using clock = std::chrono::steady_clock;

  RateLimiter() { configure(0, 0, std::chrono::milliseconds(1000)); }
  RateLimiter(unsigned per_peer_max, unsigned global_max,
              std::chrono::milliseconds window) {
    configure(per_peer_max, global_max, window);
  }

  void configure(unsigned per_peer_max, unsigned global_max,
                 std::chrono::milliseconds window) {
    std::lock_guard<std::mutex> g(m_);
    per_peer_max_ = per_peer_max;
    global_max_ = global_max;
    rate_.reset(slweb_rate_new(per_peer_max, global_max,
                               (unsigned)window.count()));
    epoch_ = clock::now();
  }

  bool enabled() const { return per_peer_max_ != 0 || global_max_ != 0; }

  /*
   * Record one request from `peer` at time `now` and report whether it is
   * allowed. When it would exceed either cap the request is rejected and NOT
   * counted (so a flood does not keep the window permanently saturated).
   */
  bool allow(const std::string &peer, clock::time_point now) {
    std::lock_guard<std::mutex> g(m_);
    if (!rate_) return true;
    return slweb_rate_allow(rate_.get(), peer.c_str(), to_ms(now)) != 0;
  }

  /* Convenience overload using the current time. */
  bool allow(const std::string &peer) { return allow(peer, clock::now()); }

  /* Drop per-peer windows that have fully aged out; bounds memory use. */
  void sweep(clock::time_point now) {
    std::lock_guard<std::mutex> g(m_);
    if (rate_) slweb_rate_sweep(rate_.get(), to_ms(now));
  }

 private:
  unsigned long long to_ms(clock::time_point t) const {
    return (unsigned long long)std::chrono::duration_cast<std::chrono::milliseconds>(
               t - epoch_)
        .count();
  }
  struct RateDeleter {
    void operator()(slweb_rate *r) const { slweb_rate_free(r); }
  };
  mutable std::mutex m_;
  unsigned per_peer_max_ = 0;
  unsigned global_max_ = 0;
  clock::time_point epoch_{};
  std::unique_ptr<slweb_rate, RateDeleter> rate_;
};

/*
 * Bearer-token authorization for write-capable /db use. When a token is set,
 * a mutating statement must present Authorization: Bearer <token>. The compare
 * is constant time over the max of the two lengths.
 */
class WriteAuth {
 public:
  void set_token(const std::string &t) { token_ = t; }
  bool required() const { return !token_.empty(); }

  /* authorization_header is the raw value of the Authorization header ("" if
   * absent). Returns true when no token is required, or the presented bearer
   * token matches in constant time. */
  bool authorize(const std::string &authorization_header) const;

 private:
  std::string token_;
};

/* Constant-time string equality (independent of early-mismatch position). */
bool constant_time_equals(const std::string &a, const std::string &b);

/*
 * Hardening settings that may be supplied in the configuration file as well as
 * on the command line (the design calls for both control surfaces). CLI flags
 * override file values. Config keys (line-oriented key=value, '#' comments):
 *   rate-per-peer = N
 *   rate-global   = N
 *   rate-window-ms= N
 *   write-token   = <token>
 *   trusted-proxy = <ip>        (repeatable)
 */
struct HardeningSettings {
  bool has_rate_per_peer = false; unsigned rate_per_peer = 0;
  bool has_rate_global = false;   unsigned rate_global = 0;
  bool has_rate_window = false;   unsigned rate_window_ms = 1000;
  bool has_write_token = false;   std::string write_token;
  std::vector<std::string> trusted_proxies;
  // CPU keep-warm idle floor (also controllable from the config file):
  //   cpu-warm        = on | off      (default on)
  //   cpu-warm-floor  = N             (percent, per core)
  //   cpu-warm-ceiling= N
  //   cpu-warm-cores  = N             (clamp 1..16)
  bool has_warm = false;          bool warm_enabled = true;
  bool has_warm_floor = false;    unsigned warm_floor = 11;
  bool has_warm_ceiling = false;  unsigned warm_ceiling = 14;
  bool has_warm_cores = false;    unsigned warm_cores = 16;
};

/*
 * Read hardening settings from a config file. Unknown keys are ignored (so the
 * same file can also carry envelopment and base-server settings). Returns true
 * on a clean parse; false with err set on a malformed numeric value.
 */
bool load_hardening_config(const std::string &path, HardeningSettings &out,
                           std::string &err);

/*
 * Effective client IP. `peer_ip` is the immediate TCP peer. `forwarded_for` is
 * the right-most-untrusted value the caller already extracted from a Forwarded
 * or X-Forwarded-For header (may be empty). If `peer_ip` is in `trusted_proxies`
 * and a forwarded value is present, the forwarded client IP is returned;
 * otherwise the immediate peer IP is authoritative. This prevents address
 * spoofing by clients that are not behind the trusted terminator.
 */
std::string client_ip_from(const std::string &peer_ip,
                           const std::string &forwarded_for,
                           const std::vector<std::string> &trusted_proxies);

}  // namespace wst
}  // namespace sleela
#endif  // SLEELA_WST_HARDENING_H
