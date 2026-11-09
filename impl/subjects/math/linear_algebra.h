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
    SLMATH_SINGULAR = 5,
    SLMATH_NO_CONVERGENCE = 6
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


/* LU factorization with partial pivoting. A is n x n row-major.
 * lu receives the combined L/U factors; pivots receives n row indices.
 * solve uses the factorization; inverse writes an n x n inverse. */
slmath_status slmath_matrix_lu(const double *a, size_t n, double *lu,
                               size_t *pivots, double tolerance);
slmath_status slmath_matrix_lu_solve(const double *lu, const size_t *pivots,
                                     size_t n, const double *b, double *x);
slmath_status slmath_matrix_inverse(const double *a, size_t n, double *out,
                                    double tolerance);

/* Thin QR factorization via modified Gram-Schmidt.
 * A is rows x cols, rows >= cols. Q is rows x cols; R is cols x cols. */
slmath_status slmath_matrix_qr(const double *a, size_t rows, size_t cols,
                               double *q, double *r, double tolerance);

/* Complex arrays use interleaved [re0, im0, re1, im1, ...] storage. */
typedef struct slmath_complex { double re; double im; } slmath_complex;
slmath_complex slmath_complex_add(slmath_complex a, slmath_complex b);
slmath_complex slmath_complex_subtract(slmath_complex a, slmath_complex b);
slmath_complex slmath_complex_multiply(slmath_complex a, slmath_complex b);
slmath_status slmath_complex_divide(slmath_complex a, slmath_complex b,
                                    slmath_complex *out);
double slmath_complex_abs(slmath_complex z);
double slmath_complex_phase(slmath_complex z);
slmath_complex slmath_complex_conjugate(slmath_complex z);

/* Typed numerical-array primitives. All storage is caller-owned. */
slmath_status slmath_array_copy(const double *src, double *dst, size_t n);
slmath_status slmath_array_slice(const double *src, size_t src_n,
                                 size_t start, size_t count, size_t stride,
                                 double *dst);
slmath_status slmath_array_binary(const double *a, const double *b,
                                  double *out, size_t n, int operation);

/* Symmetric Jacobi eigensolver. A is n x n symmetric row-major.
 * eigenvectors are columns, eigenvalues are unsorted. */
slmath_status slmath_symmetric_eigen(const double *a, size_t n,
                                     double *eigenvalues,
                                     double *eigenvectors,
                                     size_t max_sweeps, double tolerance);

/* In-place radix-2 FFT over interleaved complex values. Inverse is 0 or 1. */
slmath_status slmath_fft(slmath_complex *values, size_t n, int inverse);


/*
 * VM-facing typed dispatch contract. The VM must validate and pin each backing
 * array for the duration of the call; these descriptors never transfer
 * ownership. Dimensions are explicit so the bridge can reject malformed
 * object layouts before entering a numerical kernel.
 */
typedef struct slmath_buffer {
    double *data;
    size_t length;
    size_t rows;
    size_t cols;
} slmath_buffer;

typedef enum slmath_operation {
    SLMATH_OP_VECTOR_ADD = 1,
    SLMATH_OP_VECTOR_DOT = 2,
    SLMATH_OP_VECTOR_NORM = 3,
    SLMATH_OP_VECTOR_SCALE = 4,
    SLMATH_OP_MATRIX_MULTIPLY = 10,
    SLMATH_OP_MATRIX_TRANSPOSE = 11,
    SLMATH_OP_MATRIX_INVERSE = 12,
    SLMATH_OP_COMPLEX_MULTIPLY = 20,
    SLMATH_OP_QR = 30,
    SLMATH_OP_SYMMETRIC_EIGEN = 31,
    SLMATH_OP_FFT = 40
} slmath_operation;

slmath_status slmath_vm_dispatch(slmath_operation operation,
                                 const slmath_buffer *a,
                                 const slmath_buffer *b,
                                 slmath_buffer *out,
                                 slmath_buffer *aux,
                                 double scalar, size_t flags);

#ifdef __cplusplus
}
#endif
#endif
