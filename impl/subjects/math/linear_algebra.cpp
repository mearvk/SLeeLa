#include "linear_algebra.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <new>
#include <vector>

namespace {
bool count_ok(size_t a, size_t b, size_t *out) {
    if (!out || (b && a > std::numeric_limits<size_t>::max() / b))
        return false;
    *out = a * b;
    return true;
}

bool valid_range(const double *p, size_t n) {
    size_t bytes = 0;
    return p && count_ok(n, sizeof(double), &bytes) &&
           reinterpret_cast<uintptr_t>(p) <=
               std::numeric_limits<uintptr_t>::max() - bytes;
}

bool valid_range(double *p, size_t n) {
    return valid_range(static_cast<const double *>(p), n);
}

template <typename F>
slmath_status vector_binary(const double *a, const double *b,
                            double *out, size_t n, F fn) {
    if (!valid_range(a, n) || !valid_range(b, n) || !valid_range(out, n))
        return SLMATH_INVALID_ARGUMENT;
    for (size_t i = 0; i < n; ++i) out[i] = fn(a[i], b[i]);
    return SLMATH_OK;
}
} // namespace

extern "C" {

slmath_status slmath_vector_add(const double *a, const double *b,
                                double *out, size_t n) {
    return vector_binary(a, b, out, n,
                         [](double x, double y) { return x + y; });
}

slmath_status slmath_vector_subtract(const double *a, const double *b,
                                     double *out, size_t n) {
    return vector_binary(a, b, out, n,
                         [](double x, double y) { return x - y; });
}

slmath_status slmath_vector_scale(const double *a, double scalar,
                                  double *out, size_t n) {
    if (!valid_range(a, n) || !valid_range(out, n))
        return SLMATH_INVALID_ARGUMENT;
    for (size_t i = 0; i < n; ++i) out[i] = a[i] * scalar;
    return SLMATH_OK;
}

slmath_status slmath_vector_dot(const double *a, const double *b,
                                size_t n, double *out) {
    if (!valid_range(a, n) || !valid_range(b, n) || !out)
        return SLMATH_INVALID_ARGUMENT;
    double sum = 0.0;
    double correction = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double product = a[i] * b[i];
        const double adjusted = product - correction;
        const double next = sum + adjusted;
        correction = (next - sum) - adjusted;
        sum = next;
    }
    *out = sum;
    return SLMATH_OK;
}

slmath_status slmath_vector_norm(const double *a, size_t n, double *out) {
    if (!valid_range(a, n) || !out) return SLMATH_INVALID_ARGUMENT;
    /* Repeated hypot avoids the avoidable overflow/underflow of sum(x*x). */
    double norm = 0.0;
    for (size_t i = 0; i < n; ++i) norm = std::hypot(norm, a[i]);
    *out = norm;
    return SLMATH_OK;
}

slmath_status slmath_vector_normalize(const double *a, double *out, size_t n) {
    if (!valid_range(a, n) || !valid_range(out, n))
        return SLMATH_INVALID_ARGUMENT;
    double norm = 0.0;
    for (size_t i = 0; i < n; ++i) norm = std::hypot(norm, a[i]);
    if (norm == 0.0 || !std::isfinite(norm)) return SLMATH_INVALID_ARGUMENT;
    for (size_t i = 0; i < n; ++i) out[i] = a[i] / norm;
    return SLMATH_OK;
}

slmath_status slmath_vector_cross3(const double a[3], const double b[3],
                                   double out[3]) {
    if (!a || !b || !out) return SLMATH_INVALID_ARGUMENT;
    const double x = a[1] * b[2] - a[2] * b[1];
    const double y = a[2] * b[0] - a[0] * b[2];
    const double z = a[0] * b[1] - a[1] * b[0];
    out[0] = x; out[1] = y; out[2] = z;
    return SLMATH_OK;
}

slmath_status slmath_matrix_multiply(const double *a, size_t rows_a,
                                     size_t cols_a, const double *b,
                                     size_t rows_b, size_t cols_b,
                                     double *out) {
    if (cols_a != rows_b) return SLMATH_DIMENSION_MISMATCH;
    size_t ac = 0, bc = 0, oc = 0;
    if (!count_ok(rows_a, cols_a, &ac) ||
        !count_ok(rows_b, cols_b, &bc) ||
        !count_ok(rows_a, cols_b, &oc))
        return SLMATH_SIZE_OVERFLOW;
    if (!valid_range(a, ac) || !valid_range(b, bc) || !valid_range(out, oc))
        return SLMATH_INVALID_ARGUMENT;
    try {
        std::vector<double> staged(oc, 0.0);
        for (size_t r = 0; r < rows_a; ++r) {
            for (size_t k = 0; k < cols_a; ++k) {
                const double av = a[r * cols_a + k];
                for (size_t c = 0; c < cols_b; ++c)
                    staged[r * cols_b + c] += av * b[k * cols_b + c];
            }
        }
        for (size_t i = 0; i < oc; ++i) out[i] = staged[i];
    } catch (...) {
        /* No C++ exception may cross the stable C ABI boundary. */
        return SLMATH_ALLOCATION_FAILURE;
    }
    return SLMATH_OK;
}

slmath_status slmath_matrix_transpose(const double *a, size_t rows,
                                      size_t cols, double *out) {
    size_t count = 0;
    if (!count_ok(rows, cols, &count)) return SLMATH_SIZE_OVERFLOW;
    if (!valid_range(a, count) || !valid_range(out, count))
        return SLMATH_INVALID_ARGUMENT;
    try {
        std::vector<double> staged(count);
        for (size_t r = 0; r < rows; ++r)
            for (size_t c = 0; c < cols; ++c)
                staged[c * rows + r] = a[r * cols + c];
        for (size_t i = 0; i < count; ++i) out[i] = staged[i];
    } catch (const std::bad_alloc &) {
        return SLMATH_ALLOCATION_FAILURE;
    }
    return SLMATH_OK;
}

slmath_status slmath_matrix_identity(double *out, size_t order) {
    size_t count = 0;
    if (!count_ok(order, order, &count)) return SLMATH_SIZE_OVERFLOW;
    if (!valid_range(out, count)) return SLMATH_INVALID_ARGUMENT;
    for (size_t i = 0; i < count; ++i) out[i] = 0.0;
    for (size_t i = 0; i < order; ++i) out[i * order + i] = 1.0;
    return SLMATH_OK;
}

} // extern "C"
