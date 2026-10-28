/*
 * web-server-technology/native/cpu_warmer-test.cpp
 *
 * Scientific check that the keep-warm controller converges into the configured
 * band. Runs a few warmer threads for a short interval, then reports the
 * measured per-thread utilization (from the controller) AND an independent
 * process-level measurement, and asserts both land within a tolerance of the
 * [floor, ceiling] band.
 *
 * Exit 0 = converged within tolerance; non-zero otherwise.
 */
#include "cpu_warmer.h"

#include <cstdio>
#include <ctime>
#include <thread>

using namespace sleela::wst;

static double process_cpu_seconds() {
  struct timespec ts;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}
static double wall_seconds() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

int main() {
  CpuWarmerConfig cfg;
  cfg.enabled = true;
  cfg.floor_pct = 11;
  cfg.ceiling_pct = 14;
  cfg.max_cores = 2;  // keep the test light and deterministic on CI

  CpuWarmer warmer(cfg);
  const unsigned n = warm_core_count(cfg.max_cores);
  std::printf("warmer threads: %u (band %u%%-%u%% per core)\n", n,
              cfg.floor_pct, cfg.ceiling_pct);

  // Let the controller settle, then sample over a measurement interval.
  warmer.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));  // settle

  const double c0 = process_cpu_seconds();
  const double w0 = wall_seconds();
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));  // measure
  const double c1 = process_cpu_seconds();
  const double w1 = wall_seconds();
  const double controller_pct = warmer.measured_pct();
  warmer.stop();

  // Independent measurement: total process CPU over the interval, divided by
  // the number of warmer threads, gives per-thread utilization. (The main
  // thread is asleep during the interval, so process CPU ~= warmer CPU.)
  const double proc_total_pct = ((c1 - c0) / (w1 - w0)) * 100.0;
  const double per_thread_pct = proc_total_pct / (double)n;

  std::printf("controller-reported per-thread: %.2f%%\n", controller_pct);
  std::printf("independent per-thread (process cpu / threads): %.2f%%\n",
              per_thread_pct);

  // Tolerance: the band is 11-14; allow +/- 4 points of slack for scheduler
  // jitter, turbo, and measurement noise in a shared CI box.
  const double lo = (double)cfg.floor_pct - 4.0;
  const double hi = (double)cfg.ceiling_pct + 4.0;
  bool ok_ctrl = controller_pct >= lo && controller_pct <= hi;
  bool ok_indep = per_thread_pct >= lo && per_thread_pct <= hi;

  std::printf("tolerance band for test: %.0f%%-%.0f%%\n", lo, hi);
  std::printf("controller in band: %s\n", ok_ctrl ? "yes" : "NO");
  std::printf("independent in band: %s\n", ok_indep ? "yes" : "NO");

  if (ok_ctrl && ok_indep) {
    std::printf("\ncpu-warmer converged within tolerance\n");
    return 0;
  }
  std::printf("\ncpu-warmer did NOT converge within tolerance\n");
  return 1;
}
