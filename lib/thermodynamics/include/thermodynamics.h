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

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_THERMODYNAMICS_H */
