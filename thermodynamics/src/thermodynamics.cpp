/*
 * thermodynamics.cpp - C++17 orchestration over the C Thermodynamics IV ABI
 * Max Rupplin - MEARVK LLC - 2026
 *
 * Mirrors the repository's "C ABI + C++ orchestration" boundary: the physics
 * lives in thermodynamics.c; this translation unit provides an ergonomic,
 * RAII C++ surface (owning 3D field, convenience simulation driver) that calls
 * straight through to the C functions. No duplicated math.
 */
#include "thermodynamics.h"

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace sleela::thermo {

/* Owning 3D temperature field. Wraps sl_thermo_field3d and manages its
 * storage so callers get value semantics and bounds-checked construction. */
class Field3D {
public:
    Field3D(std::size_t nx, std::size_t ny, std::size_t nz, double h, double fill = 0.0)
        : data_(nx * ny * nz, fill)
    {
        if (nx < 3 || ny < 3 || nz < 3) {
            throw std::invalid_argument("Field3D requires at least 3 samples per axis");
        }
        if (h <= 0.0) {
            throw std::invalid_argument("Field3D grid spacing must be positive");
        }
        view_.nx = nx;
        view_.ny = ny;
        view_.nz = nz;
        view_.h = h;
        view_.T = data_.data();
    }

    double &at(std::size_t i, std::size_t j, std::size_t k)
    {
        return data_[sl_thermo_index(&view_, i, j, k)];
    }
    double at(std::size_t i, std::size_t j, std::size_t k) const
    {
        return data_[sl_thermo_index(&view_, i, j, k)];
    }

    double laplacian(std::size_t i, std::size_t j, std::size_t k) const
    {
        return sl_thermo_laplacian(&view_, i, j, k);
    }

    std::array<double, 3> gradient(std::size_t i, std::size_t j, std::size_t k) const
    {
        std::array<double, 3> g{};
        sl_thermo_gradient(&view_, i, j, k, g.data());
        return g;
    }

    const sl_thermo_field3d *c_view() const { return &view_; }
    sl_thermo_field3d *c_view() { return &view_; }

    std::size_t nx() const { return view_.nx; }
    std::size_t ny() const { return view_.ny; }
    std::size_t nz() const { return view_.nz; }

private:
    std::vector<double> data_;
    sl_thermo_field3d view_{};
};

/* Advance a field `steps` times under the 3D heat equation. Throws if the C
 * stepper rejects the parameters (e.g. a dt beyond the stability limit). */
void simulate_heat(Field3D &field, double alpha, double dt, std::size_t steps)
{
    Field3D scratch(field.nx(), field.ny(), field.nz(), field.c_view()->h);
    Field3D *a = &field;
    Field3D *b = &scratch;
    for (std::size_t s = 0; s < steps; ++s) {
        const int rc = sl_thermo_heat_step(a->c_view(), b->c_view(), alpha, dt);
        if (rc != 0) {
            throw std::runtime_error("sl_thermo_heat_step failed with code " +
                                     std::to_string(rc));
        }
        std::swap(a, b);
    }
    if (a != &field) {
        /* Final result ended up in scratch; copy it back into the caller field. */
        for (std::size_t k = 0; k < field.nz(); ++k) {
            for (std::size_t j = 0; j < field.ny(); ++j) {
                for (std::size_t i = 0; i < field.nx(); ++i) {
                    field.at(i, j, k) = a->at(i, j, k);
                }
            }
        }
    }
}

/* Thin typed wrappers so C++ callers need not touch the C names directly. */
double heat_energy(double mass, double c, double dT) { return sl_thermo_heat_energy(mass, c, dT); }
double carnot_efficiency(double tc, double th) { return sl_thermo_carnot_efficiency(tc, th); }
double mean_energy(const std::vector<double> &levels, double temperature)
{
    return sl_thermo_mean_energy(levels.data(), levels.size(), temperature);
}

} // namespace sleela::thermo
