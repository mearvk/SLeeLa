#include "voltage_field.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace utf4088 {
namespace {
bool finite_sample(const VoltageSample& p) {
    return std::isfinite(p.x) && std::isfinite(p.y) &&
           std::isfinite(p.volts) && std::isfinite(p.time);
}
}

FieldState analyze_field(const std::vector<VoltageSample>& samples, std::size_t i) {
    FieldState s{};
    if (samples.empty() || i >= samples.size() || !finite_sample(samples[i])) return s;

    const auto& c = samples[i];
    s.voltage = c.volts;
    const VoltageSample* l = nullptr; const VoltageSample* r = nullptr;
    const VoltageSample* d = nullptr; const VoltageSample* u = nullptr;
    double ld = std::numeric_limits<double>::infinity(), rd = ld, dd = ld, ud = ld;

    for (const auto& p : samples) {
        if (&p == &c || !finite_sample(p)) continue;
        const double dx = p.x - c.x, dy = p.y - c.y;
        const double distance2 = dx * dx + dy * dy;
        if (!std::isfinite(distance2)) continue;
        if (std::abs(dy) < 1e-9 && dx < 0 && distance2 < ld) { l = &p; ld = distance2; }
        if (std::abs(dy) < 1e-9 && dx > 0 && distance2 < rd) { r = &p; rd = distance2; }
        if (std::abs(dx) < 1e-9 && dy < 0 && distance2 < dd) { d = &p; dd = distance2; }
        if (std::abs(dx) < 1e-9 && dy > 0 && distance2 < ud) { u = &p; ud = distance2; }
    }

    if (l && r) s.dVdx = (r->volts - l->volts) / (r->x - l->x);
    else if (r) s.dVdx = (r->volts - c.volts) / (r->x - c.x);
    else if (l) s.dVdx = (c.volts - l->volts) / (c.x - l->x);
    if (d && u) s.dVdy = (u->volts - d->volts) / (u->y - d->y);
    else if (u) s.dVdy = (u->volts - c.volts) / (u->y - c.y);
    else if (d) s.dVdy = (c.volts - d->volts) / (c.y - d->y);

    if (!std::isfinite(s.dVdx)) s.dVdx = 0;
    if (!std::isfinite(s.dVdy)) s.dVdy = 0;
    s.magnitude = std::hypot(s.dVdx, s.dVdy);
    s.direction = std::atan2(s.dVdy, s.dVdx);

    double mean = 0;
    std::size_t count = 0;
    for (const auto& p : samples) if (finite_sample(p)) { mean += p.volts; ++count; }
    if (count == 0) return FieldState{};
    mean /= static_cast<double>(count);
    double variance = 0;
    for (const auto& p : samples) if (finite_sample(p)) {
        const double z = p.volts - mean;
        variance += z * z;
    }
    variance /= static_cast<double>(count);
    s.uniformity = std::isfinite(variance) ? 1.0 / (1.0 + std::sqrt(variance)) : 0.0;
    return s;
}

std::uint64_t field_to_input(const FieldState& s) {
    auto quantize = [](double x) -> std::uint64_t {
        if (!std::isfinite(x)) return 0;
        const double bounded = std::min(std::abs(x), 1048575.0 / 1000.0);
        return static_cast<std::uint64_t>(bounded * 1000.0);
    };
    // Fixed 64-bit layout: 20 bits each for voltage/magnitude, 20 for
    // uniformity, and 4 for direction. Mask before shifting to avoid overlap.
    const auto v = quantize(s.voltage) & 0xFFFFFULL;
    const auto m = quantize(s.magnitude) & 0xFFFFFULL;
    const auto u = quantize(s.uniformity) & 0xFFFFFULL;
    const auto d = quantize(s.direction) & 0xFULL;
    return (v << 44) | (m << 24) | (u << 4) | d;
}

} // namespace utf4088
