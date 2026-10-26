/* SPDX-License-Identifier: MIT */
/* cdma-sim: a laptop command-line driver for the SLeeLa CDMA link-level
 * reference stack. Pick a standard, number of users, Eb/N0, multipath, and
 * trial count; it reports the measured bit-error rate for the reference user.
 *
 * Reference / simulation only; conveys no air-interface patent rights and is
 * not a conforming modem. See cdma/LEGAL-AND-SCOPE.md.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdma_common.h"
#include "cdma_link.h"

/* Per-standard spreading-factor caps and default code family. */
typedef struct {
    const char *name;
    size_t max_sf;
    cdma_code_kind codes;
    const char *note;
} std_profile;

static const std_profile PROFILES[] = {
    { "is95",   64,  CDMA_CODE_WALSH, "IS-95 cdmaOne, Walsh-64" },
    { "1xrtt",  128, CDMA_CODE_WALSH, "CDMA2000 1xRTT, Walsh spreading" },
    { "evdo",   512, CDMA_CODE_WALSH, "CDMA2000 1x EV-DO, Walsh spreading" },
    { "wcdma",  512, CDMA_CODE_OVSF,  "W-CDMA / UMTS, OVSF SF 4-512" },
};
static const size_t NPROFILES = sizeof(PROFILES) / sizeof(PROFILES[0]);

static const std_profile *find_profile(const char *name) {
    size_t i;
    for (i = 0; i < NPROFILES; ++i)
        if (strcmp(PROFILES[i].name, name) == 0) return &PROFILES[i];
    return NULL;
}

static void usage(const char *argv0) {
    size_t i;
    fprintf(stderr,
        "Usage: %s --std <name> [options]\n"
        "\n"
        "  --std <name>     is95 | 1xrtt | evdo | wcdma\n"
        "  --sf <n>         spreading factor (power of two; default: std max)\n"
        "  --users <n>      active users 1..sf (default 2)\n"
        "  --ebn0 <dB>      Eb/N0 in dB (default 8.0)\n"
        "  --paths <k>      multipath resolvable paths 1..%d (default 1)\n"
        "  --trials <n>     Monte-Carlo trials (default 20000)\n"
        "  --seed <n>       PRNG seed (default 1)\n"
        "  --help           show this help\n"
        "\n"
        "Standards:\n", argv0, CDMA_MAX_PATHS);
    for (i = 0; i < NPROFILES; ++i)
        fprintf(stderr, "  %-7s %s (max SF %zu)\n",
                PROFILES[i].name, PROFILES[i].note, PROFILES[i].max_sf);
}

int main(int argc, char **argv) {
    const char *std_name = NULL;
    size_t sf = 0, users = 2, paths = 1, trials = 20000;
    double ebn0_db = 8.0;
    unsigned long long seed = 1;
    int i;

    for (i = 1; i < argc; ++i) {
        if      (!strcmp(argv[i], "--help")) { usage(argv[0]); return 0; }
        else if (!strcmp(argv[i], "--std")   && i + 1 < argc) std_name = argv[++i];
        else if (!strcmp(argv[i], "--sf")     && i + 1 < argc) sf     = (size_t)strtoul(argv[++i], NULL, 10);
        else if (!strcmp(argv[i], "--users")  && i + 1 < argc) users  = (size_t)strtoul(argv[++i], NULL, 10);
        else if (!strcmp(argv[i], "--ebn0")   && i + 1 < argc) ebn0_db = strtod(argv[++i], NULL);
        else if (!strcmp(argv[i], "--paths")  && i + 1 < argc) paths  = (size_t)strtoul(argv[++i], NULL, 10);
        else if (!strcmp(argv[i], "--trials") && i + 1 < argc) trials = (size_t)strtoul(argv[++i], NULL, 10);
        else if (!strcmp(argv[i], "--seed")   && i + 1 < argc) seed   = strtoull(argv[++i], NULL, 10);
        else { fprintf(stderr, "error: unknown or incomplete argument '%s'\n\n", argv[i]); usage(argv[0]); return 2; }
    }

    if (!std_name) { fprintf(stderr, "error: --std is required\n\n"); usage(argv[0]); return 2; }
    const std_profile *p = find_profile(std_name);
    if (!p) { fprintf(stderr, "error: unknown standard '%s'\n\n", std_name); usage(argv[0]); return 2; }

    if (sf == 0) sf = p->max_sf;
    if (sf < 2 || sf > p->max_sf || (sf & (sf - 1)) != 0) {
        fprintf(stderr, "error: --sf must be a power of two in [2, %zu] for %s\n", p->max_sf, p->name);
        return 2;
    }
    if (users < 1 || users > sf) {
        fprintf(stderr, "error: --users must be in [1, %zu]\n", sf);
        return 2;
    }
    if (paths < 1 || paths > CDMA_MAX_PATHS) {
        fprintf(stderr, "error: --paths must be in [1, %d]\n", CDMA_MAX_PATHS);
        return 2;
    }

    /* Equal-power users (open loop). Build a simple exponentially-decaying
     * multipath profile for the requested number of paths. */
    double amp[CDMA_MAX_CODE_LEN];
    { size_t u; for (u = 0; u < users; ++u) amp[u] = 1.0; }

    cdma_multipath mp;
    cdma_multipath_flat(&mp);
    { size_t k; for (k = 1; k < paths; ++k) cdma_multipath_add(&mp, 0.6 / (double)k, k); }

    double sigma = cdma_ebn0_to_sigma(ebn0_db, sf);

    cdma_rng rng;
    cdma_rng_seed(&rng, seed);

    double ber = cdma_link_ber(sf, users, amp, p->codes, &mp, sigma, trials, &rng);
    if (ber < 0.0) { fprintf(stderr, "error: simulation failed (bad parameters)\n"); return 1; }

    printf("cdma-sim  standard=%s  SF=%zu  users=%zu  Eb/N0=%.1f dB  paths=%zu  trials=%zu\n",
           p->name, sf, users, ebn0_db, paths, trials);
    printf("          per-chip noise sigma=%.4f   reference-user BER=%.5f  (%s)\n",
           sigma, ber,
           ber == 0.0 ? "no errors in this run" : "Monte-Carlo estimate");
    return 0;
}
