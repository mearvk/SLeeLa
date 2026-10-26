/* SPDX-License-Identifier: MIT */
/* SLeeLa CDMA link-level model implementation. Reference / simulation only;
 * see cdma/LEGAL-AND-SCOPE.md. */
#include "../include/cdma_link.h"
#include <math.h>

/* ---- Deterministic PRNG: xorshift128+ ---------------------------------- */

void cdma_rng_seed(cdma_rng *rng, unsigned long long seed) {
    if (!rng) return;
    /* SplitMix64 to expand the seed into two non-zero words. */
    unsigned long long z = seed + 0x9E3779B97F4A7C15ULL;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    rng->s0 = z ^ (z >> 31);
    z = rng->s0 + 0x9E3779B97F4A7C15ULL;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    rng->s1 = z ^ (z >> 31);
    if (rng->s0 == 0ULL && rng->s1 == 0ULL) rng->s1 = 1ULL;
}

static unsigned long long rng_next(cdma_rng *rng) {
    unsigned long long x = rng->s0, y = rng->s1;
    rng->s0 = y;
    x ^= x << 23;
    rng->s1 = x ^ y ^ (x >> 17) ^ (y >> 26);
    return rng->s1 + y;
}

double cdma_rng_uniform(cdma_rng *rng) {
    if (!rng) return 0.0;
    /* Top 53 bits -> [0, 1). */
    return (double)(rng_next(rng) >> 11) * (1.0 / 9007199254740992.0);
}

double cdma_rng_gaussian(cdma_rng *rng) {
    if (!rng) return 0.0;
    double u1, u2;
    do { u1 = cdma_rng_uniform(rng); } while (u1 <= 1e-300);
    u2 = cdma_rng_uniform(rng);
    return sqrt(-2.0 * log(u1)) * cos(6.283185307179586 * u2);
}

/* ---- Multipath --------------------------------------------------------- */

void cdma_multipath_flat(cdma_multipath *mp) {
    if (!mp) return;
    mp->count = 1;
    mp->gain[0] = 1.0;
    mp->delay[0] = 0;
}

int cdma_multipath_add(cdma_multipath *mp, double gain, size_t delay) {
    if (!mp || mp->count >= CDMA_MAX_PATHS) return -1;
    mp->gain[mp->count] = gain;
    mp->delay[mp->count] = delay;
    mp->count++;
    return 0;
}

int cdma_channel_multipath(double *out, const int *in, size_t length,
                           const cdma_multipath *mp) {
    size_t i, k;
    if (!out || !in || !mp || mp->count == 0) return -1;
    for (i = 0; i < length; ++i) out[i] = 0.0;
    for (k = 0; k < mp->count; ++k) {
        size_t d = mp->delay[k];
        double g = mp->gain[k];
        if (d >= length) continue;
        for (i = 0; i + d < length; ++i) out[i + d] += g * (double)in[i];
    }
    return 0;
}

int cdma_channel_awgn(double *chips, size_t length, double sigma, cdma_rng *rng) {
    size_t i;
    if (!chips || !rng || sigma < 0.0) return -1;
    for (i = 0; i < length; ++i) chips[i] += sigma * cdma_rng_gaussian(rng);
    return 0;
}

double cdma_ebn0_to_sigma(double ebn0_db, size_t spreading_factor) {
    /* BPSK, unit-energy chips: Eb = SF * Ec, Ec = 1. N0/2 = sigma^2 per chip.
     * Eb/N0 (linear) = SF / (2 * sigma^2)  =>  sigma = sqrt(SF / (2*Eb/N0)). */
    double lin = pow(10.0, ebn0_db / 10.0);
    if (lin <= 0.0 || spreading_factor == 0) return 0.0;
    return sqrt((double)spreading_factor / (2.0 * lin));
}

/* ---- RAKE receiver ----------------------------------------------------- */

double cdma_rake_combine(const double *rx, size_t length, const int *code,
                         const cdma_multipath *mp, int *ok) {
    size_t i, k;
    double combined = 0.0;
    if (!rx || !code || !mp || mp->count == 0) { if (ok) *ok = 0; return 0.0; }
    for (k = 0; k < mp->count; ++k) {
        size_t d = mp->delay[k];
        double g = mp->gain[k], finger = 0.0;
        if (d >= length) continue;
        /* Despread the finger aligned to this path delay. */
        for (i = 0; i + d < length; ++i) finger += rx[i + d] * (double)code[i];
        /* Maximal-ratio combining: weight each finger by its path gain. */
        combined += g * finger;
    }
    if (ok) *ok = 1;
    return combined;
}

/* ---- End-to-end link trial / BER --------------------------------------- */

static int make_code(int *code, size_t sf, unsigned idx, cdma_code_kind kind) {
    return kind == CDMA_CODE_OVSF ? cdma_ovsf_code(code, sf, idx)
                                  : cdma_walsh_code(code, sf, idx);
}

int cdma_link_trial(size_t spreading_factor, size_t users,
                    const double *amplitude, cdma_code_kind codes,
                    const cdma_multipath *mp, double sigma, cdma_rng *rng) {
    int code[CDMA_MAX_CODE_LEN];
    int target_symbol = 0;
    double sum[CDMA_MAX_CODE_LEN];
    double rxd[CDMA_MAX_CODE_LEN];
    size_t sf = spreading_factor, i, u;

    if (sf < 2 || sf > CDMA_MAX_CODE_LEN || users == 0 || users > sf ||
        !amplitude || !mp || !rng || sigma < 0.0) return -1;

    for (i = 0; i < sf; ++i) sum[i] = 0.0;

    /* Superpose all users' spread signals (each with its own amplitude). */
    for (u = 0; u < users; ++u) {
        int sym = cdma_rng_uniform(rng) < 0.5 ? -1 : 1;
        if (u == 0) target_symbol = sym;
        if (make_code(code, sf, (unsigned)u, codes) != 0) return -1;
        for (i = 0; i < sf; ++i) sum[i] += amplitude[u] * (double)sym * (double)code[i];
    }

    /* Multipath then AWGN. We quantize the superposition to int for the
     * multipath convolution interface, then add real-valued noise. */
    {
        int chips[CDMA_MAX_CODE_LEN];
        for (i = 0; i < sf; ++i) {
            double v = sum[i];
            chips[i] = (int)(v < 0.0 ? v - 0.5 : v + 0.5);
        }
        if (cdma_channel_multipath(rxd, chips, sf, mp) != 0) return -1;
    }
    if (cdma_channel_awgn(rxd, sf, sigma, rng) != 0) return -1;

    /* Recover user 0 with a RAKE receiver against its own code. */
    if (make_code(code, sf, 0U, codes) != 0) return -1;
    {
        int ok = 0;
        double soft = cdma_rake_combine(rxd, sf, code, mp, &ok);
        if (!ok) return -1;
        return cdma_decide_symbol((long)(soft < 0.0 ? soft - 0.5 : soft + 0.5))
               == target_symbol ? 1 : 0;
    }
}

double cdma_link_ber(size_t spreading_factor, size_t users,
                     const double *amplitude, cdma_code_kind codes,
                     const cdma_multipath *mp, double sigma,
                     size_t trials, cdma_rng *rng) {
    size_t t, errors = 0;
    if (trials == 0) return -1.0;
    for (t = 0; t < trials; ++t) {
        int r = cdma_link_trial(spreading_factor, users, amplitude, codes,
                                mp, sigma, rng);
        if (r < 0) return -1.0;
        if (r == 0) errors++;
    }
    return (double)errors / (double)trials;
}

/* ---- Power control ----------------------------------------------------- */

int cdma_power_control_step(double *amplitude, const double *rx_power,
                            size_t users, double target_power, double step_db) {
    size_t u;
    double up = pow(10.0, step_db / 20.0);   /* amplitude ratio for +step dB */
    double down = pow(10.0, -step_db / 20.0);
    if (!amplitude || !rx_power || users == 0 || target_power <= 0.0) return -1;
    for (u = 0; u < users; ++u) {
        if (rx_power[u] < target_power) amplitude[u] *= up;
        else                            amplitude[u] *= down;
        if (amplitude[u] < 1e-6) amplitude[u] = 1e-6;
    }
    return 0;
}
