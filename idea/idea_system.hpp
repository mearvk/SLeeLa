/*
 * idea_system.hpp -- Idea System(TM) C++ facade.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * A thin, exception-light C++ layer over the C core (idea_system.h).  It gives
 * the VM / embedders an RAII handle (idea::System) and a small driver
 * (idea::SeriesRunner) that advances Time forward across the whole 0..30h
 * stream, so the special native code the Standard SLeeLa VM needs is available
 * from C++ without touching raw handles.  The State IS the System: callers
 * observe only what System::present() reports.
 */
#ifndef IDEA_SYSTEM_HPP
#define IDEA_SYSTEM_HPP

#include "idea_system.h"

#include <functional>
#include <string>
#include <utility>

namespace idea {

/* Mirror the C enums into the namespace for ergonomic C++ use. */
using StateKind = idea_state_kind;
using Status    = idea_status;
using Scheme    = idea_scheme;

/*
 * RAII owner of an idea_system*.  Movable, non-copyable (a System is a unique,
 * signal-distributed thing; you share a reference, you do not clone it).
 */
class System {
public:
    System() : sys_(idea_create()) {}
    ~System() { if (sys_) idea_destroy(sys_); }

    System(System &&other) noexcept : sys_(other.sys_) { other.sys_ = nullptr; }
    System &operator=(System &&other) noexcept {
        if (this != &other) {
            if (sys_) idea_destroy(sys_);
            sys_ = other.sys_;
            other.sys_ = nullptr;
        }
        return *this;
    }
    System(const System &) = delete;
    System &operator=(const System &) = delete;

    bool valid() const { return sys_ != nullptr; }
    idea_system *raw() const { return sys_; }

    /* ---- source / fetcher ---- */
    Status setSourceUrl(const std::string &url) {
        return idea_set_source_url(sys_, url.c_str());
    }
    std::string sourceUrl() const { return idea_source_url(sys_); }

    /*
     * Register a C++ fetcher.  The std::function is kept alive by this System
     * and trampolined through the C ABI.  Returns the bytes written, or a
     * negative Status, exactly like idea_fetch_fn.
     */
    using Fetcher = std::function<long(const std::string &url, char *buf, size_t cap)>;
    Status setFetcher(Fetcher fetcher) {
        fetcher_ = std::move(fetcher);
        return idea_set_fetcher(sys_, &System::trampoline, this);
    }

    /* ---- signal ---- */
    Status receiveSignal()                 { return idea_receive_signal(sys_); }
    Status feedHex(const std::string &hex) { return idea_feed_hex(sys_, hex.c_str()); }
    size_t signalLen() const               { return idea_signal_len(sys_); }

    /* ---- series / time ---- */
    Status buildSeries()                   { return idea_build_series(sys_); }
    Status tick(long deltaSeconds)         { return idea_tick(sys_, deltaSeconds); }

    /* ---- presentation ---- */
    StateKind presentState() const         { return idea_present_state(sys_); }
    long      elapsedSeconds() const       { return idea_elapsed_seconds(sys_); }
    double    progress() const             { return idea_progress(sys_); }
    int       decisionsTaken() const       { return idea_decisions_taken(sys_); }
    int       seriesDepth() const          { return idea_series_depth(sys_); }
    bool      isCapable(const std::string &cap) const {
        return idea_is_capable(sys_, cap.c_str()) != 0;
    }
    std::string present() const {
        char buf[512];
        long n = idea_present(sys_, buf, sizeof(buf));
        return n > 0 ? std::string(buf, static_cast<size_t>(n)) : std::string();
    }

    /* ---- organization ---- */
    Status setScheme(Scheme s)             { return idea_set_scheme(sys_, s); }
    Scheme scheme() const                  { return idea_scheme_of(sys_); }
    Status addObjective(const std::string &o) { return idea_add_objective(sys_, o.c_str()); }
    int    objectiveCount() const          { return idea_objective_count(sys_); }

    /* ---- names ---- */
    static std::string name(StateKind k) { return idea_state_name(k); }
    static std::string name(Status s)    { return idea_status_name(s); }
    static std::string name(Scheme s)    { return idea_scheme_name(s); }

private:
    static long trampoline(const char *url, char *buf, size_t cap, void *user) {
        auto *self = static_cast<System *>(user);
        if (!self || !self->fetcher_) return IDEA_INPUT_UNAVAILABLE;
        return self->fetcher_(url ? url : "", buf, cap);
    }

    idea_system *sys_;
    Fetcher      fetcher_;
};

/*
 * Drives a System forward through the entire Series.  Time is ultimate and
 * company to the Series: run() steps by a fixed cadence until Finality, and
 * reports each presented State to an optional observer.  It is consistent in
 * Time even when the System seems to loop/retry.
 */
class SeriesRunner {
public:
    using Observer = std::function<void(const System &)>;

    explicit SeriesRunner(System &sys, long stepSeconds = 3600)
        : sys_(sys), step_(stepSeconds > 0 ? stepSeconds : 1) {}

    /* Run to Finality, calling observer after each forward step.  Returns the
     * number of steps taken.  Guards against a runaway loop with a hard cap
     * derived from the 30h design goal. */
    int run(const Observer &observer = nullptr) {
        int steps = 0;
        const int cap = (IDEA_MAX_SECONDS / static_cast<int>(step_)) + 2;
        while (steps < cap) {
            Status st = sys_.tick(step_);
            if (observer) observer(sys_);
            ++steps;
            if (st == IDEA_EXHAUSTED) break;
        }
        return steps;
    }

private:
    System &sys_;
    long     step_;
};

} /* namespace idea */

#endif /* IDEA_SYSTEM_HPP */
