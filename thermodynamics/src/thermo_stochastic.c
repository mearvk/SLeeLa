/*
 * thermo_stochastic.c - stochastic extension of the thermodynamics vignette
 * Max Rupplin - MEARVK LLC - 2026
 *
 * The deterministic heat equation (thermodynamics.c) fixes every future
 * exactly. This file models the part of the dynamics that is NOT under your
 * control: thermal fluctuations. It adds Gaussian white-noise forcing to the
 * stepper (a Langevin/stochastic-heat-equation form), a Monte Carlo driver
 * that turns many random futures into a distribution with a confidence
 * interval, and the Arrhenius log-rate law for temperature-driven decay.
 *
 * All routines are standard, named methods; see thermodynamics.h.
 */
#include "thermodynamics.h"

#include <math.h>
#include <stdlib.h>

/* M_PI is not in strict ISO C; define a fallback for -std=c11 -Wpedantic. */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ---- PRNG: splitmix64 + Box-Muller -------------------------------------- */

void sl_thermo_rng_seed(sl_thermo_rng *rng, unsigned long long seed)
{
    rng->s = seed;
}

double sl_thermo_rng_uniform(sl_thermo_rng *rng)
{
    /* splitmix64 */
    unsigned long long z = (rng->s += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    z = z ^ (z >> 31);
    /* 53-bit mantissa -> [0,1) */
    return (double)(z >> 11) * (1.0 / 9007199254740992.0);
}

double sl_thermo_rng_normal(sl_thermo_rng *rng)
{
    /* Box-Muller; guard against log(0). */
    double u1 = sl_thermo_rng_uniform(rng);
    double u2 = sl_thermo_rng_uniform(rng);
    if (u1 < 1e-300) {
        u1 = 1e-300;
    }
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

/* ---- Stochastic heat step ----------------------------------------------- */

int sl_thermo_heat_step_stochastic(const sl_thermo_field3d *cur, sl_thermo_field3d *next,
                                   double alpha, double dt, double sigma,
                                   sl_thermo_rng *rng)
{
    /* Reuse the deterministic step for the drift term and all validation. */
    const int rc = sl_thermo_heat_step(cur, next, alpha, dt);
    if (rc != 0) {
        return rc;
    }
    if (sigma < 0.0 || !rng) {
        return 6;
    }
    if (sigma == 0.0) {
        return 0; /* pure deterministic case */
    }
    /* Add independent N(0, sigma^2 * dt) noise at each interior node. */
    const double scale = sigma * sqrt(dt);
    for (size_t k = 1; k < cur->nz - 1; ++k) {
        for (size_t j = 1; j < cur->ny - 1; ++j) {
            for (size_t i = 1; i < cur->nx - 1; ++i) {
                const size_t idx = sl_thermo_index(cur, i, j, k);
                next->T[idx] += scale * sl_thermo_rng_normal(rng);
            }
        }
    }
    return 0;
}

/* ---- Monte Carlo over futures ------------------------------------------- */

int sl_thermo_monte_carlo(const sl_thermo_field3d *initial,
                          size_t i, size_t j, size_t k,
                          double alpha, double dt, double sigma,
                          size_t steps, size_t runs, unsigned long long seed,
                          sl_thermo_distribution *out)
{
    if (!initial || !out || runs == 0 || steps == 0) {
        return 1;
    }
    const size_t nx = initial->nx, ny = initial->ny, nz = initial->nz;
    if (nx < 3 || ny < 3 || nz < 3) {
        return 3;
    }
    if (i == 0 || j == 0 || k == 0 || i >= nx - 1 || j >= ny - 1 || k >= nz - 1) {
        return 7; /* tracked node must be interior */
    }

    const size_t total = nx * ny * nz;
    /* Two scratch buffers that ping-pong each step. Allocated on the C heap. */
    double *a = (double *)malloc(total * sizeof(double));
    double *b = (double *)malloc(total * sizeof(double));
    if (!a || !b) {
        free(a);
        free(b);
        return 8;
    }

    sl_thermo_rng rng;
    sl_thermo_rng_seed(&rng, seed);

    /* Welford's online algorithm for stable mean/variance over the runs. */
    double mean = 0.0, m2 = 0.0;
    size_t count = 0;

    for (size_t r = 0; r < runs; ++r) {
        for (size_t idx = 0; idx < total; ++idx) {
            a[idx] = initial->T[idx];
        }
        sl_thermo_field3d fa = {nx, ny, nz, initial->h, a};
        sl_thermo_field3d fb = {nx, ny, nz, initial->h, b};
        sl_thermo_field3d *src = &fa, *dst = &fb;

        for (size_t s = 0; s < steps; ++s) {
            const int rc = sl_thermo_heat_step_stochastic(src, dst, alpha, dt, sigma, &rng);
            if (rc != 0) {
                free(a);
                free(b);
                return rc;
            }
            sl_thermo_field3d *tmp = src;
            src = dst;
            dst = tmp;
        }

        const double x = src->T[i + nx * (j + ny * k)];
        ++count;
        const double delta = x - mean;
        mean += delta / (double)count;
        m2 += delta * (x - mean);
    }

    free(a);
    free(b);

    out->samples = count;
    out->mean = mean;
    out->variance = (count > 1) ? m2 / (double)(count - 1) : 0.0;
    out->std_dev = sqrt(out->variance);
    const double se = out->std_dev / sqrt((double)count);
    out->ci95_low = mean - 1.96 * se;
    out->ci95_high = mean + 1.96 * se;
    return 0;
}

/* ---- Arrhenius log-rate law --------------------------------------------- */

double sl_thermo_arrhenius_rate(double a_factor, double activation_energy, double temperature)
{
    if (temperature <= 0.0) {
        return 0.0;
    }
    return a_factor * exp(-activation_energy / (SL_THERMO_R * temperature));
}

double sl_thermo_arrhenius_temperature(double rate, double a_factor, double activation_energy)
{
    if (rate <= 0.0 || a_factor <= 0.0 || rate >= a_factor) {
        return -1.0; /* ln(rate/A) must be negative and finite */
    }
    const double ln_ratio = log(rate / a_factor);
    return -activation_energy / (SL_THERMO_R * ln_ratio);
}
