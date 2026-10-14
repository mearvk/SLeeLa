/*
 * idea_main.cpp -- Idea System(TM) reference driver.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Demonstrates the whole narrative end to end against the C core, using the
 * C++ facade:
 *
 *   1. Load the signal-distributed System.
 *   2. Receive signal input from the known internet resource -- a hex code the
 *      Content Author controls, served from github.com.  (Here the fetch is a
 *      deterministic offline stand-in that returns Author-provided hex; a real
 *      deployment registers a fetcher that pulls the Author's wiki page.  The
 *      core performs no network I/O itself.)
 *   3. Build the forward, linear solution Series (<= 30h, <= 9 seemly
 *      decisions) seeded by the signal.
 *   4. Let Time progress -- the ultimate, companion force to the Series --
 *      presenting the System's State at each step.  The State IS the System.
 *
 * This program is deterministic and does no networking, so it runs anywhere.
 */
#include "idea_system.hpp"

#include <cstring>
#include <iostream>
#include <string>

/*
 * The Author provides the URL data; the Idea(TM) System will be open on the
 * user's computer, well, usually.  This stand-in fetcher returns an
 * Author-controlled hex code without touching the network, so the demo is
 * consistent in Time.  Swap this for a real HTTP fetcher to receive the live
 * hex code from the Author's github.com wiki page.
 */
static long offline_author_fetch(const std::string &url, char *buf, size_t cap) {
    // These are numbers the Content Author / controller owns.  The leading
    // bytes read as "IDEA" so the provenance is legible in the stream.
    static const char *author_hex =
        "49 44 45 41 2d 53 59 53 54 45 4d c0 ffee 2a 7f 10 99 42 13 88";
    (void)url;
    size_t n = std::strlen(author_hex);
    if (n >= cap) n = cap - 1;
    std::memcpy(buf, author_hex, n);
    buf[n] = '\0';
    return static_cast<long>(n);
}

int main() {
    idea::System sys;
    if (!sys.valid()) {
        std::cerr << "Idea System(TM): out of memory on create\n";
        return 1;
    }

    std::cout << "== " << IDEA_SYSTEM_NAME << " " << IDEA_SYSTEM_VERSION
              << " -- From Author; distributive ==\n";
    std::cout << "known internet resource: " << sys.sourceUrl() << "\n";

    // Organization: pursue a few bounded AI objectives on the Concept scheme.
    sys.setScheme(IDEA_SCHEME_CONCEPT);
    sys.addObjective("maintain System in maintenance of State");
    sys.addObjective("present only what is present/capable");
    sys.addObjective("advance the Series forward in Time");
    std::cout << "objectives: " << sys.objectiveCount()
              << "  scheme: " << idea::System::name(sys.scheme()) << "\n";

    // 2. Receive the signal (Author-controlled hex from the resource).
    sys.setFetcher(offline_author_fetch);
    idea::Status rs = sys.receiveSignal();
    if (rs != IDEA_OK) {
        std::cerr << "receive failed: " << idea::System::name(rs) << "\n";
        return 1;
    }
    std::cout << "received signal: " << sys.signalLen() << " bytes "
              << "(capable to receive=" << (sys.isCapable("receive") ? "yes" : "no") << ")\n";

    // 3. Build the Series from the signal.
    idea::Status bs = sys.buildSeries();
    if (bs != IDEA_OK) {
        std::cerr << "build failed: " << idea::System::name(bs) << "\n";
        return 1;
    }
    std::cout << "series built; " << sys.present() << "\n";

    // Show that it may SEEM to loop/retry (a zero-Time tick) but stays
    // consistent -- the presented State reflects it without moving Time.
    sys.tick(0);
    std::cout << "zero-tick (seems to loop): " << sys.present() << "\n";

    // 4. Let Time progress to Finality, presenting the State each hour.
    std::cout << "-- Time progresses (the ultimate companion to the Series) --\n";
    idea::SeriesRunner runner(sys, /*stepSeconds=*/3600);
    int steps = runner.run([](const idea::System &s) {
        std::cout << "  " << s.present() << "\n";
    });

    std::cout << "-- Finality reached in " << steps << " forward step(s) --\n";
    std::cout << "final state: " << idea::System::name(sys.presentState())
              << "  (capable to finalize="
              << (sys.isCapable("finalize") ? "yes" : "no") << ")\n";
    return 0;
}
