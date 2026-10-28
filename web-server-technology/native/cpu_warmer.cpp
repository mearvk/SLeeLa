/*
 * web-server-technology/native/cpu_warmer.cpp
 *
 * Closed-loop CPU keep-warm idle floor. See cpu_warmer.h for the rationale.
 */
#include "cpu_warmer.h"

#include <algorithm>
#include <cmath>
#include <ctime>

#if !defined(_WIN32) && !defined(_WIN64)
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#endif

namespace sleela {
namespace wst {

CpuWarmerConfig sanitize(CpuWarmerConfig c) {
  if (c.floor_pct < 1) c.floor_pct = 1;
  if (c.floor_pct > 90) c.floor_pct = 90;
  if (c.ceiling_pct < c.floor_pct) c.ceiling_pct = c.floor_pct;
  if (c.ceiling_pct > 95) c.ceiling_pct = 95;
  if (c.max_cores < 1) c.max_cores = 1;
  if (c.max_cores > 16) c.max_cores = 16;
  if (c.window < std::chrono::milliseconds(50)) c.window = std::chrono::milliseconds(50);
  if (c.window > std::chrono::milliseconds(2000)) c.window = std::chrono::milliseconds(2000);
  return c;
}

unsigned warm_core_count(unsigned max_cores) {
  unsigned n = std::thread::hardware_concurrency();
  if (n == 0) n = 1;
  if (n > max_cores) n = max_cores;
  if (n < 1) n = 1;
  return n;
}

// ---- per-thread CPU time (seconds) -----------------------------------------
namespace {
#if !defined(_WIN32) && !defined(_WIN64)
double thread_cpu_seconds() {
  struct timespec ts;
  if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) != 0) return 0.0;
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}
double wall_seconds() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}
#else
double thread_cpu_seconds() { return (double)clock() / (double)CLOCKS_PER_SEC; }
double wall_seconds() {
  using namespace std::chrono;
  return duration<double>(steady_clock::now().time_since_epoch()).count();
}
#endif

// Lower this warmer thread to the least-disruptive scheduling class so real
// work always preempts it. Best-effort: failures are non-fatal.
void lower_priority() {
#if !defined(_WIN32) && !defined(_WIN64)
  sched_param sp{};
  sp.sched_priority = 0;
  // SCHED_IDLE runs only when nothing else is runnable on the CPU.
#ifdef SCHED_IDLE
  if (pthread_setschedparam(pthread_self(), SCHED_IDLE, &sp) == 0) return;
#endif
  // Fallback: a high nice value via the per-thread setpriority is not portable,
  // so we simply leave the default class; the duty cycle keeps load modest.
#endif
}

// A small, non-optimizable unit of work: the compiler cannot elide it because
// the result is fed back through a volatile sink. Keeps the core's pipeline and
// caches warm without allocating or touching I/O.
volatile double g_warm_sink = 0.0;
double burn(double seed) {
  double x = seed;
  for (int i = 0; i < 4096; ++i) {
    x = x * 1.0000001 + 0.5;
    x = std::sqrt(x + 1.0);
  }
  return x;
}
}  // namespace

CpuWarmer::CpuWarmer(CpuWarmerConfig cfg)
    : cfg_(sanitize(cfg)), measured_(warm_core_count(cfg_.max_cores)) {
  for (auto &m : measured_) m.store(0.0);
}

CpuWarmer::~CpuWarmer() { stop(); }

void CpuWarmer::start() {
  if (!cfg_.enabled) return;
  if (running_.exchange(true)) return;  // already running
  stop_.store(false);
  unsigned n = (unsigned)measured_.size();
  for (unsigned i = 0; i < n; ++i)
    workers_.emplace_back([this, i] { worker_loop(i); });
}

void CpuWarmer::stop() {
  if (!running_.exchange(false)) {
    // not running; still join any residual threads defensively
  }
  stop_.store(true);
  for (auto &t : workers_)
    if (t.joinable()) t.join();
  workers_.clear();
}

double CpuWarmer::measured_pct() const {
  if (measured_.empty()) return 0.0;
  double sum = 0.0;
  for (const auto &m : measured_) sum += m.load();
  return sum / (double)measured_.size();
}

std::vector<double> CpuWarmer::measured_per_core() const {
  std::vector<double> out;
  if (!running_.load()) return out;
  out.reserve(measured_.size());
  for (const auto &m : measured_) out.push_back(m.load());
  return out;
}

void CpuWarmer::worker_loop(unsigned index) {
  lower_priority();

  // Target the middle of the band so we sit comfortably inside [floor, ceiling]
  // and the natural jitter of a loaded machine does not push us outside it.
  const double floor = (double)cfg_.floor_pct;
  const double ceiling = (double)cfg_.ceiling_pct;
  const double target = (floor + ceiling) / 2.0;

  // duty = fraction of each window we intend to spend burning CPU. Seed near
  // the target; the controller corrects from the first measurement.
  double duty = target / 100.0;
  const double win_s = (double)cfg_.window.count() / 1000.0;

  double seed = 1.0 + index;  // diversify per thread

  while (!stop_.load()) {
    const double cpu0 = thread_cpu_seconds();
    const double wall0 = wall_seconds();

    // Busy phase: burn for duty * window of wall time.
    const double busy_target = win_s * duty;
    double acc = 0.0;
    while (!stop_.load()) {
      seed = burn(seed);
      g_warm_sink = seed;
      if (wall_seconds() - wall0 >= busy_target) break;
      (void)acc;
    }

    // Idle phase: sleep the remainder of the window.
    const double elapsed = wall_seconds() - wall0;
    double sleep_s = win_s - elapsed;
    if (sleep_s > 0.0 && !stop_.load()) {
      // Sleep in small slices so shutdown stays responsive.
      double remaining = sleep_s;
      while (remaining > 0.0 && !stop_.load()) {
        double slice = std::min(remaining, 0.05);
        std::this_thread::sleep_for(std::chrono::duration<double>(slice));
        remaining -= slice;
      }
    }

    // Measure actual utilization over the whole window (busy + idle).
    const double cpu1 = thread_cpu_seconds();
    const double wall1 = wall_seconds();
    const double dw = wall1 - wall0;
    double util_pct = 0.0;
    if (dw > 0.0) util_pct = ((cpu1 - cpu0) / dw) * 100.0;
    if (index < measured_.size()) measured_[index].store(util_pct);

    // Proportional controller: nudge duty toward the target. Gain is modest so
    // it converges smoothly without oscillating. The duty is the lever; the
    // measured utilization is the process variable.
    const double err = target - util_pct;        // percent points
    duty += (err / 100.0) * 0.5;                  // 0.5 proportional gain
    // Keep duty in a safe band: never 0 (would stop warming) and never a
    // runaway hot loop (cap well under 100%).
    const double duty_min = 0.005;                // >=0.5% so it always warms
    const double duty_max = std::min(0.95, ceiling / 100.0 + 0.10);
    if (duty < duty_min) duty = duty_min;
    if (duty > duty_max) duty = duty_max;
  }
}

}  // namespace wst
}  // namespace sleela
