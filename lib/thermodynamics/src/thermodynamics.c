/*
 * thermodynamics.c - C implementation of the thermodynamics vignette
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Pure C11. Every routine is a direct transcription of a standard physical
 * relation; see thermodynamics.h for the equations and units.
 */
#include "thermodynamics.h"

#include <math.h>

/* ---- Scalar / lumped-parameter relations -------------------------------- */

double sl_thermo_heat_energy(double mass, double specific_heat, double delta_t)
{
    return mass * specific_heat * delta_t;
}

double sl_thermo_fourier_flux_1d(double k, double dT, double dx)
{
    if (dx == 0.0) {
        return 0.0;
    }
    return -k * (dT / dx);
}

double sl_thermo_newton_cooling(double t0, double t_env, double rate, double time)
{
    return t_env + (t0 - t_env) * exp(-rate * time);
}

double sl_thermo_carnot_efficiency(double t_cold, double t_hot)
{
    if (t_hot <= 0.0) {
        return 0.0;
    }
    return 1.0 - (t_cold / t_hot);
}

/* ---- Statistical mechanics ---------------------------------------------- */

double sl_thermo_partition_function(const double *energies, size_t n, double temperature)
{
    if (!energies || n == 0 || temperature <= 0.0) {
        return 0.0;
    }
    const double beta = 1.0 / (SL_THERMO_KB * temperature);
    double z = 0.0;
    for (size_t i = 0; i < n; ++i) {
        z += exp(-beta * energies[i]);
    }
    return z;
}

double sl_thermo_boltzmann_probability(double energy_i, double temperature, double z)
{
    if (temperature <= 0.0 || z <= 0.0) {
        return 0.0;
    }
    const double beta = 1.0 / (SL_THERMO_KB * temperature);
    return exp(-beta * energy_i) / z;
}

double sl_thermo_mean_energy(const double *energies, size_t n, double temperature)
{
    const double z = sl_thermo_partition_function(energies, n, temperature);
    if (z <= 0.0) {
        return 0.0;
    }
    double mean = 0.0;
    for (size_t i = 0; i < n; ++i) {
        mean += energies[i] * sl_thermo_boltzmann_probability(energies[i], temperature, z);
    }
    return mean;
}

/* ---- 3D scalar temperature field ---------------------------------------- */

size_t sl_thermo_index(const sl_thermo_field3d *f, size_t i, size_t j, size_t k)
{
    return i + f->nx * (j + f->ny * k);
}

void sl_thermo_gradient(const sl_thermo_field3d *f, size_t i, size_t j, size_t k, double grad[3])
{
    const double inv2h = 1.0 / (2.0 * f->h);
    grad[0] = (f->T[sl_thermo_index(f, i + 1, j, k)] -
               f->T[sl_thermo_index(f, i - 1, j, k)]) * inv2h;
    grad[1] = (f->T[sl_thermo_index(f, i, j + 1, k)] -
               f->T[sl_thermo_index(f, i, j - 1, k)]) * inv2h;
    grad[2] = (f->T[sl_thermo_index(f, i, j, k + 1)] -
               f->T[sl_thermo_index(f, i, j, k - 1)]) * inv2h;
}

double sl_thermo_laplacian(const sl_thermo_field3d *f, size_t i, size_t j, size_t k)
{
    const double c = f->T[sl_thermo_index(f, i, j, k)];
    const double inv_h2 = 1.0 / (f->h * f->h);
    const double sum =
        f->T[sl_thermo_index(f, i + 1, j, k)] + f->T[sl_thermo_index(f, i - 1, j, k)] +
        f->T[sl_thermo_index(f, i, j + 1, k)] + f->T[sl_thermo_index(f, i, j - 1, k)] +
        f->T[sl_thermo_index(f, i, j, k + 1)] + f->T[sl_thermo_index(f, i, j, k - 1)];
    return (sum - 6.0 * c) * inv_h2;
}

double sl_thermo_divergence(const sl_thermo_field3d *fx,
                            const sl_thermo_field3d *fy,
                            const sl_thermo_field3d *fz,
                            size_t i, size_t j, size_t k)
{
    const double inv2h = 1.0 / (2.0 * fx->h);
    const double dfx = (fx->T[sl_thermo_index(fx, i + 1, j, k)] -
                        fx->T[sl_thermo_index(fx, i - 1, j, k)]) * inv2h;
    const double dfy = (fy->T[sl_thermo_index(fy, i, j + 1, k)] -
                        fy->T[sl_thermo_index(fy, i, j - 1, k)]) * inv2h;
    const double dfz = (fz->T[sl_thermo_index(fz, i, j, k + 1)] -
                        fz->T[sl_thermo_index(fz, i, j, k - 1)]) * inv2h;
    return dfx + dfy + dfz;
}

int sl_thermo_heat_step(const sl_thermo_field3d *cur, sl_thermo_field3d *next,
                        double alpha, double dt)
{
    if (!cur || !next || cur == next) {
        return 1;
    }
    if (cur->nx != next->nx || cur->ny != next->ny || cur->nz != next->nz) {
        return 2;
    }
    if (cur->nx < 3 || cur->ny < 3 || cur->nz < 3) {
        return 3;
    }
    if (alpha <= 0.0 || dt <= 0.0) {
        return 4;
    }
    /* FTCS stability limit in 3D. */
    if (dt > (cur->h * cur->h) / (6.0 * alpha)) {
        return 5;
    }

    const size_t total = cur->nx * cur->ny * cur->nz;
    for (size_t idx = 0; idx < total; ++idx) {
        next->T[idx] = cur->T[idx]; /* copy, keeps Dirichlet boundaries fixed */
    }
    for (size_t k = 1; k < cur->nz - 1; ++k) {
        for (size_t j = 1; j < cur->ny - 1; ++j) {
            for (size_t i = 1; i < cur->nx - 1; ++i) {
                const size_t idx = sl_thermo_index(cur, i, j, k);
                next->T[idx] = cur->T[idx] +
                               alpha * dt * sl_thermo_laplacian(cur, i, j, k);
            }
        }
    }
    return 0;
}
