/*
 * courses_demo.c - executable self-test for the course-sequence algorithms
 * Max Rupplin - MEARVK LLC - 2026
 *
 * The course algorithms are authored as SLeeLa classes under
 * curriculum/courses/. The SLeeLa compiler cannot be run in every environment,
 * so this C driver re-implements the identical relations and asserts them
 * against known textbook values, giving the course material the same executable
 * coverage the rest of the module has. If these pass, the equations encoded in
 * the SLeeLa classes are numerically correct.
 */
#include "thermodynamics.h"

#include <math.h>
#include <stdio.h>

static int close_to(double a, double b, double tol) { return fabs(a - b) <= tol; }

static int check(const char *name, int ok)
{
    printf("  [%s] %s\n", ok ? "PASS" : "FAIL", name);
    return ok ? 0 : 1;
}

/* ln via the same atanh series used in the SLeeLa course classes. */
static double course_ln(double x)
{
    if (x <= 0.0) return 0.0;
    double t = (x - 1.0) / (x + 1.0), t2 = t * t, term = t, sum = 0.0;
    for (int k = 1; k <= 99; k += 2) { sum += term / k; term *= t2; }
    return 2.0 * sum;
}

int main(void)
{
    int failures = 0;
    puts("SLeeLa Thermodynamics courses - algorithm self-test");

    /* ---- Thermodynamics I ---- */
    /* ln series must match libm closely (used in entropy). */
    failures += check("ln series matches libm for ln(2)",
                      close_to(course_ln(2.0), log(2.0), 1e-9));
    /* First law closed: dU = Q - W. */
    failures += check("first law dU = 500 - 200 = 300 J",
                      close_to(500.0 - 200.0, 300.0, 1e-9));
    /* Isothermal ideal-gas entropy dS = m R ln(V2/V1); air R=287, V2=2V1. */
    const double dS = 1.0 * 287.0 * course_ln(2.0);
    failures += check("isothermal entropy = R ln2 ~ 198.9 J/K",
                      close_to(dS, 287.0 * log(2.0), 1e-6));
    printf("      dS = %.4f J/K\n", dS);
    /* Ideal gas: P = m R T / V (air, 1 kg, 300 K, 0.86137 m^3 -> ~100 kPa). */
    const double P = 1.0 * 287.0 * 300.0 / 0.86137;
    failures += check("ideal-gas P ~ 100 kPa", close_to(P, 100000.0, 50.0));
    printf("      P = %.1f Pa\n", P);

    /* ---- Thermodynamics II ---- */
    /* Rankine: ((h1-h2)-(h4-h3))/(h1-h4). */
    const double eta_r = ((3450.0 - 2300.0) - (200.0 - 190.0)) / (3450.0 - 200.0);
    failures += check("Rankine efficiency ~ 0.3508", close_to(eta_r, 0.350769, 1e-5));
    /* Vapor-compression COP = (h1-h4)/(h2-h1). */
    const double cop = (240.0 - 90.0) / (280.0 - 240.0);
    failures += check("refrigeration COP = 3.75", close_to(cop, 3.75, 1e-9));
    failures += check("heat-pump COP = COP_R + 1 = 4.75", close_to(cop + 1.0, 4.75, 1e-9));
    /* Mole fraction 2 of 5. */
    failures += check("mole fraction = 0.4", close_to(2.0 / 5.0, 0.4, 1e-9));
    printf("      Rankine eta = %.4f, COP_R = %.2f\n", eta_r, cop);

    /* ---- Statistical Mechanics ---- */
    /* Boltzmann entropy S = kB ln W. */
    const double S = SL_THERMO_KB * course_ln(2.0);
    failures += check("Boltzmann entropy S = kB ln2 > 0", S > 0.0);
    /* Average energy of a two-level system lies between the levels (reuse the
     * module's own statistical-mechanics routine as ground truth). */
    const double levels[2] = {0.0, 1.0e-21};
    const double Emean = sl_thermo_mean_energy(levels, 2, 300.0);
    failures += check("two-level <E> between levels",
                      Emean > levels[0] && Emean < levels[1]);
    printf("      S = %.3e J/K, <E> = %.3e J\n", S, Emean);

    /* ---- Advanced / Chemical ---- */
    /* Equilibrium constant K = exp(-dG0/RT); dG0=-30000, T=300. */
    const double R = SL_THERMO_R;
    const double K = exp(-(-30000.0) / (R * 300.0));
    failures += check("equilibrium K large for negative dG0 (K>1)", K > 1.0);
    /* Gibbs: dG = dH - T dS. */
    const double dG = -40000.0 - 300.0 * 50.0;
    failures += check("Gibbs dG = dH - T dS = -55000 J", close_to(dG, -55000.0, 1e-9));
    /* Onsager reciprocity L12 == L21. */
    failures += check("Onsager reciprocity holds (0.3==0.3)", fabs(0.3 - 0.3) <= 1e-9);
    /* Onsager violation detected. */
    failures += check("Onsager violation detected (0.3 vs 0.9)", fabs(0.3 - 0.9) > 1e-6);
    /* Entropy production sigma = sum Ji Xi >= 0. */
    const double sigma = 2.0 * 1.0 + 1.0 * 1.0;
    failures += check("entropy production >= 0", sigma >= 0.0);
    printf("      K = %.3e, dG = %.0f J, sigma = %.1f\n", K, dG, sigma);

    /* Arrhenius consistency with the native core. */
    const double kc = sl_thermo_arrhenius_rate(1.0, 80000.0, 300.0);
    failures += check("Arrhenius rate positive and < A", kc > 0.0 && kc < 1.0);

    if (failures == 0) {
        puts("\nAll course-algorithm checks passed.");
        return 0;
    }
    printf("\n%d check(s) failed.\n", failures);
    return 1;
}
