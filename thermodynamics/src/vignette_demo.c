/*
 * vignette_demo.c - runnable Thermodynamics IV vignette + self-test
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Demonstrates every public routine on a worked "metal bar" example and
 * asserts the results against known physics. Exit status 0 means all
 * assertions held. Build target: `make selftest`.
 */
#include "thermodynamics.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int close_to(double a, double b, double tol)
{
    return fabs(a - b) <= tol;
}

static int check(const char *name, int ok)
{
    printf("  [%s] %s\n", ok ? "PASS" : "FAIL", name);
    return ok ? 0 : 1;
}

int main(void)
{
    int failures = 0;
    puts("SLeeLa Thermodynamics IV vignette - worked example: heating a copper bar");

    /* 1) Sensible heat for 2 kg of copper (c = 385 J/kg.K) raised by 50 K. */
    const double Q = sl_thermo_heat_energy(2.0, 385.0, 50.0);
    failures += check("Q = m c dT = 38500 J", close_to(Q, 38500.0, 1e-6));
    printf("      Q = %.1f J\n", Q);

    /* 2) Fourier flux: k=401 (copper), 100 K drop across 0.5 m -> -80200 W/m^2. */
    const double q = sl_thermo_fourier_flux_1d(401.0, 100.0, 0.5);
    failures += check("Fourier flux = -80200 W/m^2", close_to(q, -80200.0, 1e-6));
    printf("      q = %.1f W/m^2\n", q);

    /* 3) Newton cooling: starts at T_env, must stay at T_env for all time. */
    const double Teq = sl_thermo_newton_cooling(300.0, 300.0, 0.1, 7.0);
    failures += check("cooling from equilibrium stays at 300 K", close_to(Teq, 300.0, 1e-9));
    /* And it must decay monotonically toward the environment. */
    const double Ta = sl_thermo_newton_cooling(500.0, 300.0, 0.1, 1.0);
    const double Tb = sl_thermo_newton_cooling(500.0, 300.0, 0.1, 5.0);
    failures += check("cooling decays monotonically toward T_env", Ta > Tb && Tb > 300.0);
    printf("      T(1s) = %.3f K, T(5s) = %.3f K\n", Ta, Tb);

    /* 4) Carnot efficiency between 300 K and 600 K is exactly 0.5. */
    const double eta = sl_thermo_carnot_efficiency(300.0, 600.0);
    failures += check("Carnot efficiency = 0.5", close_to(eta, 0.5, 1e-12));

    /* 5) Statistical mechanics: a two-level system. Probabilities sum to 1 and
     *    the lower level is favoured. */
    const double levels[2] = {0.0, 1.0e-21};
    const double T = 300.0;
    const double Z = sl_thermo_partition_function(levels, 2, T);
    const double p0 = sl_thermo_boltzmann_probability(levels[0], T, Z);
    const double p1 = sl_thermo_boltzmann_probability(levels[1], T, Z);
    failures += check("Boltzmann probabilities sum to 1", close_to(p0 + p1, 1.0, 1e-12));
    failures += check("ground state more probable than excited", p0 > p1);
    const double Emean = sl_thermo_mean_energy(levels, 2, T);
    failures += check("mean energy lies between the two levels",
                      Emean > levels[0] && Emean < levels[1]);
    printf("      Z = %.6f, p0 = %.6f, p1 = %.6f, <E> = %.3e J\n", Z, p0, p1, Emean);

    /* 6) 3D heat equation: a hot spot in the center of a cube must cool, and
     *    the surrounding fixed-boundary field should relax smoothly. We check
     *    that one FTCS step lowers the hot center (positive Laplacian is
     *    negative at a local maximum). */
    const size_t N = 5;
    double *cur = calloc(N * N * N, sizeof(double));
    double *nxt = calloc(N * N * N, sizeof(double));
    if (!cur || !nxt) {
        free(cur);
        free(nxt);
        fputs("allocation failed\n", stderr);
        return 2;
    }
    sl_thermo_field3d fc = {N, N, N, 0.1, cur};
    sl_thermo_field3d fn = {N, N, N, 0.1, nxt};
    const size_t cc = sl_thermo_index(&fc, 2, 2, 2);
    cur[cc] = 1000.0; /* hot spot */

    const double lap = sl_thermo_laplacian(&fc, 2, 2, 2);
    failures += check("Laplacian negative at a hot maximum", lap < 0.0);

    const double alpha = 1.11e-4; /* copper thermal diffusivity [m^2/s] */
    const double dt = (0.1 * 0.1) / (6.0 * alpha) * 0.5; /* half the stability limit */
    const int rc = sl_thermo_heat_step(&fc, &fn, alpha, dt);
    failures += check("heat step returns success", rc == 0);
    failures += check("hot center cools after one step", nxt[cc] < cur[cc]);
    failures += check("neighbors warm up (energy spreads outward)",
                      nxt[sl_thermo_index(&fn, 3, 2, 2)] > 0.0);
    printf("      center: %.3f K -> %.3f K, neighbor: 0 K -> %.3f K\n",
           cur[cc], nxt[cc], nxt[sl_thermo_index(&fn, 3, 2, 2)]);

    /* A dt beyond the stability limit must be rejected. */
    const double bad_dt = (0.1 * 0.1) / (6.0 * alpha) * 2.0;
    failures += check("unstable dt is rejected", sl_thermo_heat_step(&fc, &fn, alpha, bad_dt) == 5);

    free(cur);
    free(nxt);

    if (failures == 0) {
        puts("\nAll thermodynamics vignette checks passed.");
        return 0;
    }
    printf("\n%d check(s) failed.\n", failures);
    return 1;
}
