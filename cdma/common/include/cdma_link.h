/* SPDX-License-Identifier: MIT */
/* SLeeLa CDMA link-level model: AWGN, multipath, RAKE combining, power
 * control, and bit-error-rate evaluation.
 *
 * Reference / link-level simulation model only. This is NOT a conforming
 * baseband, NOT real-time DSP firmware, and conveys NO right to practice any
 * CDMA air-interface patent. See cdma/LEGAL-AND-SCOPE.md.
 */
#ifndef SLEELA_CDMA_LINK_H
#define SLEELA_CDMA_LINK_H

#include <stddef.h>
#include "cdma_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Deterministic PRNG (xorshift128+ style) so simulations are reproducible
 * across runs and platforms without depending on the C library's rand(). */
typedef struct {
    unsigned long long s0;
    unsigned long long s1;
} cdma_rng;

/* Seed the generator. Any seed is accepted; 0 is remapped internally so the
 * state is never all-zero. */
void cdma_rng_seed(cdma_rng *rng, unsigned long long seed);

/* Uniform double in [0, 1). */
double cdma_rng_uniform(cdma_rng *rng);

/* Standard normal sample (mean 0, variance 1) via Box-Muller. */
double cdma_rng_gaussian(cdma_rng *rng);

/* A multipath profile: `taps` complex-free real path gains applied at integer
 * chip delays `delay[k]` (in chips). A single tap with gain 1.0 and delay 0 is
 * the ideal flat channel. At most CDMA_MAX_PATHS taps are honored. */
#define CDMA_MAX_PATHS 8
typedef struct {
    size_t count;
    double gain[CDMA_MAX_PATHS];
    size_t delay[CDMA_MAX_PATHS];
} cdma_multipath;

/* Initialize a flat (single-path, unit-gain, zero-delay) channel. */
void cdma_multipath_flat(cdma_multipath *mp);

/* Add one resolvable path. Returns -1 if the profile is full or `delay`
 * is not representable, else 0. */
int cdma_multipath_add(cdma_multipath *mp, double gain, size_t delay);

/* Pass a real chip stream `in` (length `length`) through the multipath
 * channel, writing `out` (also length `length`). Energy spilling past the end
 * of the window is dropped. `out` and `in` must not alias. Returns -1 on bad
 * arguments, else 0. */
int cdma_channel_multipath(double *out, const int *in, size_t length,
                           const cdma_multipath *mp);

/* Add white Gaussian noise with the given standard deviation (per chip) to a
 * real chip stream in place. Returns -1 on bad arguments, else 0. */
int cdma_channel_awgn(double *chips, size_t length, double sigma, cdma_rng *rng);

/* Convert a target Eb/N0 in dB (energy per bit to noise density) into the
 * per-chip noise standard deviation for a BPSK symbol spread over
 * `spreading_factor` unit-energy chips. Returns a non-negative sigma; a very
 * large Eb/N0 yields a small sigma (near-noiseless). */
double cdma_ebn0_to_sigma(double ebn0_db, size_t spreading_factor);

/* RAKE receiver: despread `rx` against `code` at each path delay in `mp` and
 * maximal-ratio-combine the fingers (weighted by path gain). Returns the
 * combined soft correlation; the hard decision is cdma_decide_symbol() of it.
 * Returns 0.0 and sets *ok (if non-NULL) to 0 on bad arguments. */
double cdma_rake_combine(const double *rx, size_t length, const int *code,
                         const cdma_multipath *mp, int *ok);

/* One end-to-end link trial for a single user over a shared channel of
 * `users` interferers:
 *   - each of `users` users spreads a random +/-1 symbol with a distinct
 *     Walsh/OVSF code index (0..users-1),
 *   - per-user transmit amplitude comes from `amplitude[]` (power control),
 *   - the sum passes through `mp` multipath and AWGN at `sigma`,
 *   - user 0 is recovered with a RAKE receiver.
 * Returns 1 if user 0's symbol was recovered correctly, 0 if not, -1 on error.
 * If `codes` is CDMA_CODE_WALSH uses Walsh codes, CDMA_CODE_OVSF uses OVSF. */
typedef enum { CDMA_CODE_WALSH = 0, CDMA_CODE_OVSF = 1 } cdma_code_kind;

int cdma_link_trial(size_t spreading_factor, size_t users,
                    const double *amplitude, cdma_code_kind codes,
                    const cdma_multipath *mp, double sigma, cdma_rng *rng);

/* Run `trials` independent link trials and return the measured bit-error rate
 * for user 0 (errors / trials). Returns a negative value on error. */
double cdma_link_ber(size_t spreading_factor, size_t users,
                     const double *amplitude, cdma_code_kind codes,
                     const cdma_multipath *mp, double sigma,
                     size_t trials, cdma_rng *rng);

/* Closed-loop power control step: given each user's measured received power
 * and a common target, nudge transmit amplitudes toward the target by
 * `step_db` dB per call (a simple fixed-step inner loop). Amplitudes are
 * updated in place and floored at a small positive value. Returns -1 on bad
 * arguments, else 0. */
int cdma_power_control_step(double *amplitude, const double *rx_power,
                            size_t users, double target_power, double step_db);

#ifdef __cplusplus
}
#endif
#endif
