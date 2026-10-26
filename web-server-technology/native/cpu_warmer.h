#ifndef SLEELA_WST_CPU_WARMER_H
#define SLEELA_WST_CPU_WARMER_H
/*
 * web-server-technology/native/cpu_warmer.h
 *
 * CPU keep-warm idle floor.
 *
 * A modest, scientifically-bounded background load that keeps each of the
 * server's CPUs lightly busy -- a target band of roughly 11% (floor) up to 14%
 * (ceiling) per core -- even when no request traffic is present. The intent is
 * to keep the server "warm": discourage cores from dropping into deep idle
 * C-states / low P-states, so the first request after a quiet period does not
 * pay a frequency ramp / cold-path penalty.
 *
 * Why a controller, not a fixed sleep ratio
 * -----------------------------------------
 * "11% of a CPU" is not a fixed busy/sleep ratio: measured utilization drifts
 * with frequency scaling, turbo, and scheduler placement, so a hard-coded duty
 * cycle reads as very different utilization across machines. This warmer is
 * therefore a CLOSED-LOOP controller: each per-core thread measures its OWN CPU
 * time against wall-clock time over a short window and adjusts its busy/idle
 * duty cycle to converge into the configured band. That is the defensible way
 * to actually land near 11-14% rather than hope a magic ratio does.
 *
 * Deference to real work
 * ----------------------
 * Warmer threads run at the lowest practical scheduling priority (SCHED_IDLE on
 * Linux where available, otherwise a high nice value). They fill otherwise-idle
 * time only; genuine request handling always preempts them, so the floor never
 * steals measurable throughput from real load.
 *
 * Policy
 * ------
 * The feature is ON BY DEFAULT (a deliberate, twice-stated preference: keep the
 * server warm). An administrator may disable it (--no-warm) or retune the band.
 * The thread count is the detected CPU count clamped to [1, 16].
 */
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

namespace sleela {
namespace wst {

struct CpuWarmerConfig {
  bool enabled = true;      // on by default; admin may turn off
  unsigned floor_pct = 11;  // lower edge of the target band, per core
  unsigned ceiling_pct = 14;// upper edge of the target band, per core
  unsigned max_cores = 16;  // never run more than this many warmer threads
  // Control window: how often each thread measures and re-tunes its duty cycle.
  std::chrono::milliseconds window{200};
};

/*
 * Clamp/validate a config into a sane, in-contract state:
 *   - floor in [1, 90], ceiling in [floor, 95] (never a runaway hot loop);
 *   - max_cores in [1, 16];
 *   - window in [50ms, 2000ms].
 * Returns the corrected config.
 */
CpuWarmerConfig sanitize(CpuWarmerConfig c);

/* Detected online CPU count, clamped to [1, 16]. */
unsigned warm_core_count(unsigned max_cores);

class CpuWarmer {
 public:
  explicit CpuWarmer(CpuWarmerConfig cfg);
  ~CpuWarmer();

  CpuWarmer(const CpuWarmer &) = delete;
  CpuWarmer &operator=(const CpuWarmer &) = delete;

  /* Start one low-priority warmer thread per core (no-op if disabled). */
  void start();
  /* Signal all threads to stop and join them. Safe to call more than once. */
  void stop();

  bool running() const { return running_.load(); }
  unsigned threads() const { return (unsigned)workers_.size(); }
  const CpuWarmerConfig &config() const { return cfg_; }

  /*
   * Most recent measured utilization (percent, 0..100) averaged across warmer
   * threads. Updated by the threads each control window; approximate.
   */
  double measured_pct() const;

  /*
   * Per-core most-recent measured utilization (percent), one entry per warmer
   * thread, in core order. Empty when not running. Approximate, like
   * measured_pct().
   */
  std::vector<double> measured_per_core() const;

 private:
  void worker_loop(unsigned index);

  CpuWarmerConfig cfg_;
  std::atomic<bool> stop_{false};
  std::atomic<bool> running_{false};
  std::vector<std::thread> workers_;
  // Per-thread last-measured utilization (percent), for reporting.
  std::vector<std::atomic<double>> measured_;
};

}  // namespace wst
}  // namespace sleela
#endif  // SLEELA_WST_CPU_WARMER_H
