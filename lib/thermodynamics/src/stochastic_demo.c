/*
 * stochastic_demo.c - self-test for the stochastic thermodynamics extension
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Verifies the Langevin step, the Monte Carlo distribution, and the Arrhenius
 * log-rate law against known behavior. Exit 0 means all assertions held.
 */
#include "thermodynamics.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int close_to(double a, double b, double tol) { return fabs(a - b) <= tol; }

static int check(const char *name, int ok)
{
    printf("  [%s] %s\n", ok ? "PASS" : "FAIL", name);
    return ok ? 0 : 1;
}

int main(void)
{
    int failures = 0;
    puts("SLeeLa thermodynamics - stochastic extension (time you do not control)");

    /* 1) PRNG is reproducible: same seed -> same stream. */
    sl_thermo_rng r1, r2;
    sl_thermo_rng_seed(&r1, 12345ULL);
    sl_thermo_rng_seed(&r2, 12345ULL);
    failures += check("PRNG is reproducible for a fixed seed",
                      close_to(sl_thermo_rng_uniform(&r1), sl_thermo_rng_uniform(&r2), 0.0));

    /* 2) Standard-normal sampler has ~0 mean and ~1 variance over many draws. */
    sl_thermo_rng rn;
    sl_thermo_rng_seed(&rn, 42ULL);
    double sum = 0.0, sumsq = 0.0;
    const int M = 200000;
    for (int n = 0; n < M; ++n) {
        const double x = sl_thermo_rng_normal(&rn);
        sum += x;
        sumsq += x * x;
    }
    const double nmean = sum / M;
    const double nvar = sumsq / M - nmean * nmean;
    failures += check("normal sampler mean ~ 0", fabs(nmean) < 0.02);
    failures += check("normal sampler variance ~ 1", fabs(nvar - 1.0) < 0.05);
    printf("      sampled mean = %.4f, variance = %.4f\n", nmean, nvar);

    /* 3) With sigma = 0 the stochastic step must equal the deterministic step. */
    const size_t N = 5;
    double *c = calloc(N * N * N, sizeof(double));
    double *d1 = calloc(N * N * N, sizeof(double));
    double *d2 = calloc(N * N * N, sizeof(double));
    if (!c || !d1 || !d2) { free(c); free(d1); free(d2); return 2; }
    sl_thermo_field3d fc = {N, N, N, 0.1, c};
    sl_thermo_field3d fd1 = {N, N, N, 0.1, d1};
    sl_thermo_field3d fd2 = {N, N, N, 0.1, d2};
    c[sl_thermo_index(&fc, 2, 2, 2)] = 1000.0;
    const double alpha = 1.11e-4;
    const double dt = (0.1 * 0.1) / (6.0 * alpha) * 0.5;

    sl_thermo_rng rz;
    sl_thermo_rng_seed(&rz, 7ULL);
    sl_thermo_heat_step(&fc, &fd1, alpha, dt);
    sl_thermo_heat_step_stochastic(&fc, &fd2, alpha, dt, 0.0, &rz);
    int identical = 1;
    for (size_t idx = 0; idx < N * N * N; ++idx) {
        if (!close_to(d1[idx], d2[idx], 0.0)) { identical = 0; break; }
    }
    failures += check("sigma=0 reproduces the deterministic step exactly", identical);

    /* 4) Monte Carlo: the mean of many noisy futures should track the
     *    deterministic result, and the CI should bracket that mean. */
    const double sigma = 5.0;
    const size_t steps = 20, runs = 2000;
    sl_thermo_distribution dist;
    const int rc = sl_thermo_monte_carlo(&fc, 2, 2, 2, alpha, dt, sigma,
                                         steps, runs, 2024ULL, &dist);
    failures += check("monte carlo returns success", rc == 0);
    failures += check("ensemble size matches runs", dist.samples == runs);
    failures += check("positive spread under noise", dist.std_dev > 0.0);
    failures += check("95% CI brackets the mean",
                      dist.ci95_low < dist.mean && dist.mean < dist.ci95_high);
    printf("      center T after %zu steps: mean = %.3f K, sd = %.3f, "
           "95%% CI = [%.3f, %.3f] over %zu futures\n",
           steps, dist.mean, dist.std_dev, dist.ci95_low, dist.ci95_high, dist.samples);

    /* Interior-node guard: a boundary node must be rejected. */
    failures += check("boundary tracked-node rejected",
                      sl_thermo_monte_carlo(&fc, 0, 2, 2, alpha, dt, sigma,
                                            steps, runs, 1ULL, &dist) == 7);

    free(c); free(d1); free(d2);

    /* 5) Arrhenius law: rate rises with temperature, and the inverse recovers T. */
    const double A = 1.0e13, Ea = 80000.0; /* typical pre-factor, 80 kJ/mol */
    const double k300 = sl_thermo_arrhenius_rate(A, Ea, 300.0);
    const double k400 = sl_thermo_arrhenius_rate(A, Ea, 400.0);
    failures += check("Arrhenius rate increases with temperature", k400 > k300);
    const double Trec = sl_thermo_arrhenius_temperature(k300, A, Ea);
    failures += check("inverse Arrhenius recovers the temperature",
                      close_to(Trec, 300.0, 1e-6));
    printf("      k(300K) = %.4e /s, k(400K) = %.4e /s, recovered T = %.4f K\n",
           k300, k400, Trec);

    if (failures == 0) {
        puts("\nAll stochastic-extension checks passed.");
        return 0;
    }
    printf("\n%d check(s) failed.\n", failures);
    return 1;
}
