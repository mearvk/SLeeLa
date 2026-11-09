#ifndef SLEELA_LINEAR_ALGEBRA_H
#define SLEELA_LINEAR_ALGEBRA_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Stable status values for the native mathematical-structure ABI. */
typedef enum slmath_status {
    SLMATH_OK = 0,
    SLMATH_INVALID_ARGUMENT = 1,
    SLMATH_DIMENSION_MISMATCH = 2,
    SLMATH_SIZE_OVERFLOW = 3,
    SLMATH_ALLOCATION_FAILURE = 4,
    SLMATH_SINGULAR = 5
} slmath_status;

/*
 * Dense vector operations.
 * Inputs and outputs contain n doubles. Exact in-place aliases are supported.
 * Partially overlapping ranges are not supported.
 */
slmath_status slmath_vector_add(const double *a, const double *b,
                                double *out, size_t n);
slmath_status slmath_vector_subtract(const double *a, const double *b,
                                     double *out, size_t n);
slmath_status slmath_vector_scale(const double *a, double scalar,
                                  double *out, size_t n);
slmath_status slmath_vector_dot(const double *a, const double *b,
                                size_t n, double *out);
slmath_status slmath_vector_norm(const double *a, size_t n, double *out);
slmath_status slmath_vector_normalize(const double *a, double *out, size_t n);
slmath_status slmath_vector_cross3(const double a[3], const double b[3],
                                   double out[3]);

/*
 * Dense row-major matrix operations.
 * A is rows_a x cols_a; B is rows_b x cols_b; C is rows_a x cols_b.
 * Matrix multiplication supports C aliasing A or B by staging its result.
 */
slmath_status slmath_matrix_multiply(const double *a, size_t rows_a,
                                     size_t cols_a, const double *b,
                                     size_t rows_b, size_t cols_b,
                                     double *out);
slmath_status slmath_matrix_transpose(const double *a, size_t rows,
                                      size_t cols, double *out);
slmath_status slmath_matrix_identity(double *out, size_t order);

#ifdef __cplusplus
}
#endif
#endif
