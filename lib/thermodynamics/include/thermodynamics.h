/*
 * thermodynamics.h - Thermodynamics vignette public API
 * Max Rupplin - MEARVK LLC - 2026
 *
 * A small, self-contained thermodynamics library. Every function implements a
 * standard, textbook relation so the results are physically meaningful and the
 * code actually compiles and runs. The 3D field routines (gradient /
 * divergence / Laplacian / heat equation) are the multivariable-calculus core
 * used for 3D thermal models.
 *
 * Units are SI unless noted:
 *   temperature T  [K]        energy Q,E [J]        mass m [kg]
 *   specific heat c [J/(kg*K)] conductivity k [W/(m*K)]
 *   length [m]                time [s]
 */
#ifndef SLEELA_THERMODYNAMICS_H
#define SLEELA_THERMODYNAMICS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* Boltzmann constant [J/K]. */
#define SL_THERMO_KB 1.380649e-23

/* Molar gas constant [J/(mol*K)]. */
#define SL_THERMO_R 8.314462618

/* ---- Scalar / lumped-parameter relations -------------------------------- */

/* Sensible heat: Q = m * c * dT. */
double sl_thermo_heat_energy(double mass, double specific_heat, double delta_t);

/* Fourier's law (1D magnitude): q = -k * dT/dx.  Returns heat flux [W/m^2]. */
double sl_thermo_fourier_flux_1d(double k, double dT, double dx);

/* Newton's law of cooling, closed form:
 *   T(t) = T_env + (T0 - T_env) * exp(-r t).  Returns T(t) [K]. */
double sl_thermo_newton_cooling(double t0, double t_env, double rate, double time);

/* Carnot efficiency: eta = 1 - Tc/Th  (temperatures in K). */
double sl_thermo_carnot_efficiency(double t_cold, double t_hot);

/* ---- Statistical mechanics ---------------------------------------------- */

/* Boltzmann partition function Z = sum_i exp(-E_i / (kB T)). */
double sl_thermo_partition_function(const double *energies, size_t n, double temperature);

/* Probability of occupying level `i` given its energy and Z. */
double sl_thermo_boltzmann_probability(double energy_i, double temperature, double z);

/* Mean internal energy <E> = sum_i E_i * p_i. */
double sl_thermo_mean_energy(const double *energies, size_t n, double temperature);

/* ---- 3D scalar temperature field (multivariable calculus) --------------- */

/* A temperature field sampled on a uniform nx*ny*nz grid, spacing h, row-major
 * with x fastest: index(i,j,k) = i + nx*(j + ny*k). */
typedef struct {
    size_t nx, ny, nz;
    double h;      /* grid spacing [m] */
    double *T;     /* nx*ny*nz samples [K], owned by caller */
} sl_thermo_field3d;

size_t sl_thermo_index(const sl_thermo_field3d *f, size_t i, size_t j, size_t k);

/* Central-difference gradient at an interior point -> grad[3] = {dT/dx,dT/dy,dT/dz}. */
void sl_thermo_gradient(const sl_thermo_field3d *f, size_t i, size_t j, size_t k, double grad[3]);

/* Laplacian  div(grad T) = d2T/dx2 + d2T/dy2 + d2T/dz2  at an interior point. */
double sl_thermo_laplacian(const sl_thermo_field3d *f, size_t i, size_t j, size_t k);

/* Divergence of a 3-component vector field (fx,fy,fz) at an interior point. */
double sl_thermo_divergence(const sl_thermo_field3d *fx,
                            const sl_thermo_field3d *fy,
                            const sl_thermo_field3d *fz,
                            size_t i, size_t j, size_t k);

/* One explicit (FTCS) time step of the heat equation dT/dt = alpha * laplacian(T).
 * Dirichlet boundaries are held fixed. `next` and `cur` must be distinct and
 * have identical geometry. Returns 0 on success, non-zero on bad arguments or
 * a dt that violates the stability limit dt <= h^2 / (6 alpha). */
int sl_thermo_heat_step(const sl_thermo_field3d *cur, sl_thermo_field3d *next,
                        double alpha, double dt);

/* ---- Stochastic extension: time you do not control ---------------------- *
 * The deterministic heat_step above fixes every future exactly. Real systems
 * are driven by fluctuations outside your control. These routines add genuine
 * randomness and quantify the resulting distribution of outcomes.
 */

/* Reproducible PRNG state (splitmix64) so Monte Carlo runs are seedable. */
typedef struct { unsigned long long s; } sl_thermo_rng;

void sl_thermo_rng_seed(sl_thermo_rng *rng, unsigned long long seed);

/* Uniform double in [0,1). */
double sl_thermo_rng_uniform(sl_thermo_rng *rng);

/* Standard normal sample (Box-Muller). */
double sl_thermo_rng_normal(sl_thermo_rng *rng);

/* One step of the stochastic (Langevin) heat equation:
 *   dT/dt = alpha * laplacian(T) + sigma * xi(t)
 * where xi is Gaussian white noise. Equivalent to sl_thermo_heat_step plus an
 * independent N(0, sigma^2 * dt) kick at each interior node. Dirichlet
 * boundaries stay fixed. Same return codes as sl_thermo_heat_step. */
int sl_thermo_heat_step_stochastic(const sl_thermo_field3d *cur, sl_thermo_field3d *next,
                                   double alpha, double dt, double sigma,
                                   sl_thermo_rng *rng);

/* Summary statistics over an ensemble of simulated futures. */
typedef struct {
    double mean;        /* sample mean of the tracked quantity */
    double variance;    /* unbiased sample variance */
    double std_dev;     /* sqrt(variance) */
    double ci95_low;    /* mean - 1.96 * std_dev / sqrt(n) */
    double ci95_high;   /* mean + 1.96 * std_dev / sqrt(n) */
    size_t samples;     /* ensemble size n */
} sl_thermo_distribution;

/* Monte Carlo over futures: run `runs` independent stochastic simulations of
 * `steps` each, starting from `initial`, and summarize the final temperature
 * at grid node (i,j,k). This is how randomized futures are "sorted" into a
 * distribution with a confidence interval. Returns 0 on success. */
int sl_thermo_monte_carlo(const sl_thermo_field3d *initial,
                          size_t i, size_t j, size_t k,
                          double alpha, double dt, double sigma,
                          size_t steps, size_t runs, unsigned long long seed,
                          sl_thermo_distribution *out);

/* Arrhenius rate law k = A * exp(-Ea / (R T)): the real logarithmic/engineering
 * link between temperature and reaction or decay rate.
 *   A  pre-exponential factor [same units as k]
 *   Ea activation energy [J/mol]
 *   T  temperature [K]
 * Uses the molar gas constant R = 8.314462618 J/(mol K). */
double sl_thermo_arrhenius_rate(double a_factor, double activation_energy, double temperature);

/* Inverse: given a measured rate and A, recover the required temperature via
 *   T = -Ea / (R * ln(k / A)).  Returns a negative value if inputs are invalid. */
double sl_thermo_arrhenius_temperature(double rate, double a_factor, double activation_energy);

/* ---- Slot caps: enforced capacity limits -------------------------------- *
 * The design assumptions recorded in MATH.KNOWNS.md, now enforced in code.
 * A sl_thermo_slots collector sorts simulated futures and classifies each one,
 * holding at most FUTURES_MAX futures, POSITIVE_GAINS_MAX of them that improve
 * on a baseline, and LONG_TERM_CONFIDENCES_MAX high-confidence long-term ones.
 */
#define SL_THERMO_FUTURES_MAX              22u  /* sort all futures: <= 22 */
#define SL_THERMO_POSITIVE_GAINS_MAX        6u  /* positive gains:   <= 6  */
#define SL_THERMO_LONG_TERM_CONFIDENCES_MAX 2u  /* long-term/conf.:  <= 2  */

typedef struct {
    double   futures[SL_THERMO_FUTURES_MAX];               /* kept sorted ascending */
    size_t   future_count;
    double   positive_gains[SL_THERMO_POSITIVE_GAINS_MAX]; /* values above baseline */
    size_t   positive_count;
    double   long_term[SL_THERMO_LONG_TERM_CONFIDENCES_MAX];/* top long-term confidences */
    size_t   long_term_count;
    double   baseline;   /* a future "gains" if its value exceeds this */
} sl_thermo_slots;

/* Initialize an empty collector with the baseline used to judge positive gains. */
void sl_thermo_slots_init(sl_thermo_slots *s, double baseline);

/* Offer one future (e.g. a Monte Carlo outcome) with an associated confidence
 * in [0,1]. The collector:
 *   - inserts the value into the sorted futures slot (rejected when full),
 *   - if value > baseline, records it as a positive gain (rejected when full),
 *   - if it is long-term confident (confidence >= threshold), keeps it among
 *     the top LONG_TERM_CONFIDENCES_MAX by confidence.
 * Returns:
 *    0  accepted into the futures slot (and classified)
 *    1  bad arguments (null s, confidence out of [0,1])
 *    2  futures slot is full (cap reached) — value not stored
 * The positive-gains and long-term slots never overflow; when full they keep
 * only the best entries and silently drop weaker ones (reported via the
 * *_dropped counters queryable through the struct's counts). */
int sl_thermo_slots_offer(sl_thermo_slots *s, double value, double confidence,
                          double long_term_threshold);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_THERMODYNAMICS_H */
