/*
 * telephony-skya/native/sleela_skya_main.cpp
 * SLeeLa — sleela-skya command-line program: entry point.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Thin main(): parse argv into Options and dispatch through App. All behavior
 * lives in sleela_skya_cli.{h,cpp} so the command handlers stay unit-testable.
 */

#include "sleela_skya_cli.h"
#include <cstdio>

int main(int argc, char **argv) {
    sleela::skya::Options opts;
    if (!sleela::skya::parseOptions(argc, argv, opts)) {
        std::fprintf(stderr, "sleela-skya: invalid options (try 'sleela-skya help')\n");
        return 2;
    }
    sleela::skya::App app(opts);
    return app.run();
}
