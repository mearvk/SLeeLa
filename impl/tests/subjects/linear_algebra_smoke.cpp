#include "../../subjects/math/linear_algebra.h"

#include <cmath>
#include <cstdio>
#include <cstring>

static int check(bool ok, const char *name) {
    if (!ok) std::fprintf(stderr, "FAIL %s\n", name);
    else std::printf("PASS %s\n", name);
    return ok ? 0 : 1;
}

int main() {
    int failures = 0;
    const double a[] = {3.0, 4.0, 12.0};
    const double b[] = {1.0, 2.0, 3.0};
    double out[9] = {};
    double scalar = 0.0;

    failures += check(slmath_vector_add(a, b, out, 3) == SLMATH_OK &&
                      out[0] == 4.0 && out[1] == 6.0 && out[2] == 15.0,
                      "vector addition");
    failures += check(slmath_vector_dot(a, b, 3, &scalar) == SLMATH_OK &&
                      scalar == 47.0, "vector dot product");
    failures += check(slmath_vector_norm(a, 3, &scalar) == SLMATH_OK &&
                      scalar == 13.0, "stable vector norm");
    failures += check(slmath_vector_normalize(a, out, 3) == SLMATH_OK &&
                      std::fabs(out[0] - 3.0/13.0) < 1e-14 &&
                      std::fabs(out[2] - 12.0/13.0) < 1e-14,
                      "vector normalization");
    failures += check(slmath_vector_cross3(a, b, out) == SLMATH_OK &&
                      out[0] == -12.0 && out[1] == 3.0 && out[2] == 2.0,
                      "3D cross product");
    failures += check(slmath_vector_add(a, b, const_cast<double *>(a), 3) == SLMATH_OK &&
                      a[0] == 4.0, "exact in-place vector operation");

    const double m1[] = {1, 2, 3, 4, 5, 6}; // 2x3
    const double m2[] = {7, 8, 9, 10, 11, 12}; // 3x2
    const double expected[] = {58, 64, 139, 154};
    failures += check(slmath_matrix_multiply(m1, 2, 3, m2, 3, 2, out) == SLMATH_OK &&
                      std::memcmp(out, expected, sizeof(expected)) == 0,
                      "row-major matrix multiplication");
    failures += check(slmath_matrix_multiply(m1, 2, 3, m2, 2, 3, out) ==
                      SLMATH_DIMENSION_MISMATCH, "matrix dimension rejection");
    failures += check(slmath_matrix_transpose(m1, 2, 3, out) == SLMATH_OK &&
                      out[0] == 1 && out[1] == 4 && out[2] == 2 &&
                      out[3] == 5 && out[4] == 3 && out[5] == 6,
                      "matrix transpose");
    failures += check(slmath_matrix_identity(out, 3) == SLMATH_OK &&
                      out[0] == 1 && out[4] == 1 && out[8] == 1 &&
                      out[1] == 0 && out[6] == 0,
                      "identity matrix");
    return failures ? 1 : 0;
}
