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

#include <cstdlib>
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
    // An Idea(TM) payload the Content Author / controller owns, as hex:
    //   49 44 = 'I''D' magic
    //   01    = format version 1
    //   17    = 23 hours of content (0x17)
    //   06    = 6 seemly decisions
    //   00    = scheme 0 (Concepts)
    //   07 BC = depth 1980 (big-endian)
    //   01    = flags: bit0 set -> Author-designed (else the Society)
    //   ...   = opaque Author content bytes
    static const char *author_hex =
        "49 44 01 17 06 00 07 BC 01 2a 7f 10 99 42 13 88 de ad be ef";
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

    // Canvas Hz: present at 60 Hz, simulate the live Idea(TM) System at 120 Hz.
    std::cout << "canvas: present=" << idea::System::presentHz()
              << "Hz  sim=" << idea::System::simHz() << "Hz"
              << " (fixed step " << idea::System::simTimestep() << "s)\n";

    // 2. Receive the signal (Author-controlled hex from the resource).
    //    By default the System uses its built-in HTTP(S) fetcher (Option 2:
    //    networking in the core).  Set IDEA_LIVE=1 to really contact the
    //    resource; otherwise we use a deterministic offline payload so the
    //    demo runs anywhere, consistently in Time.
    const char *live = std::getenv("IDEA_LIVE");
    if (!(live && live[0] == '1')) {
        sys.setFetcher(offline_author_fetch);
        std::cout << "fetch mode: offline Author payload (set IDEA_LIVE=1 for the network)\n";
    } else {
        std::cout << "fetch mode: live HTTP(S) via the core fetcher\n";
    }
    idea::Status rs = sys.receiveSignal();
    if (rs != IDEA_OK) {
        std::cerr << "receive failed: " << idea::System::name(rs) << "\n";
        return 1;
    }
    std::cout << "received signal: " << sys.signalLen() << " bytes"
              << "  payload=" << (sys.payloadValid() ? "Idea(TM) v" : "legacy raw")
              << (sys.payloadValid() ? std::to_string(sys.payloadVersion()) : std::string())
              << "  shows=" << (sys.isAuthored() ? "Author design" : "the Society")
              << "  (capable to receive=" << (sys.isCapable("receive") ? "yes" : "no") << ")\n";

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
