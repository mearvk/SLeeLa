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
    double a[] = {3.0, 4.0, 12.0};
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
    failures += check(slmath_vector_add(a, b, a, 3) == SLMATH_OK &&
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

    const double system[] = {4, 3, 6, 3};
    double lu[4], rhs[] = {10, 12}, solution[2];
    size_t pivots[2];
    failures += check(slmath_matrix_lu(system,2,lu,pivots,1e-14) == SLMATH_OK &&
                      slmath_matrix_lu_solve(lu,pivots,2,rhs,solution) == SLMATH_OK &&
                      std::fabs(solution[0]-1.0) < 1e-12 &&
                      std::fabs(solution[1]-2.0) < 1e-12,
                      "LU factorization and solve");
    double inverse[4];
    failures += check(slmath_matrix_inverse(system,2,inverse,1e-14) == SLMATH_OK &&
                      std::fabs(inverse[0]+0.5) < 1e-12 &&
                      std::fabs(inverse[1]-0.5) < 1e-12,
                      "matrix inverse");
    double determinant = 0.0;
    failures += check(slmath_matrix_determinant(system,2,&determinant,1e-14) == SLMATH_OK &&
                      std::fabs(determinant+6.0) < 1e-12,
                      "matrix determinant");
    const double tall[] = {1,0, 1,1, 1,2};
    double q[6], rr[4];
    failures += check(slmath_matrix_qr(tall,3,2,q,rr,1e-14) == SLMATH_OK &&
                      std::fabs(q[0]*q[0]+q[2]*q[2]+q[4]*q[4]-1.0) < 1e-12 &&
                      std::fabs(q[0]*q[1]+q[2]*q[3]+q[4]*q[5]) < 1e-12,
                      "thin QR factorization");
    slmath_complex ca{3,2}, cb{1,-1}, cc{};
    failures += check(slmath_complex_multiply(ca,cb).re == 5.0 &&
                      slmath_complex_multiply(ca,cb).im == -1.0 &&
                      slmath_complex_divide(ca,cb,&cc) == SLMATH_OK &&
                      std::fabs(cc.re-0.5) < 1e-12 &&
                      std::fabs(cc.im-2.5) < 1e-12,
                      "complex arithmetic");
    failures += check(std::fabs(slmath_complex_abs(ca)-std::sqrt(13.0)) < 1e-12 &&
                      std::fabs(slmath_complex_phase(ca)-std::atan2(2.0,3.0)) < 1e-12 &&
                      slmath_complex_conjugate(ca).im == -2.0,
                      "complex magnitude, phase, and conjugate");
    const double array_src[] = {0,1,2,3,4,5};
    double sliced[3] = {};
    failures += check(slmath_array_slice(array_src,6,1,3,2,sliced) == SLMATH_OK &&
                      sliced[0] == 1 && sliced[1] == 3 && sliced[2] == 5,
                      "strided numerical array slice");
    const double symmetric[] = {2,1,1,2};
    double eigenvalues[2], eigenvectors[4];
    failures += check(slmath_symmetric_eigen(symmetric,2,eigenvalues,eigenvectors,
                                              32,1e-13) == SLMATH_OK &&
                      std::fabs(eigenvalues[0]+eigenvalues[1]-4.0) < 1e-10 &&
                      std::fabs(eigenvalues[0]*eigenvalues[1]-3.0) < 1e-10,
                      "symmetric Jacobi eigen decomposition");
    slmath_complex spectrum[4] = {{1,0},{0,0},{0,0},{0,0}};
    failures += check(slmath_fft(spectrum,4,0) == SLMATH_OK &&
                      std::fabs(spectrum[0].re-1.0) < 1e-12 &&
                      std::fabs(spectrum[1].re-1.0) < 1e-12 &&
                      slmath_fft(spectrum,4,1) == SLMATH_OK &&
                      std::fabs(spectrum[0].re-1.0) < 1e-12 &&
                      std::fabs(spectrum[1].re) < 1e-12,
                      "radix-2 FFT and inverse round-trip");
    double vmout[3] = {};
    slmath_buffer va{const_cast<double*>(b),3,0,0};
    slmath_buffer vb{const_cast<double*>(a),3,0,0};
    slmath_buffer vo{vmout,3,0,0};
    failures += check(slmath_vm_dispatch(SLMATH_OP_VECTOR_ADD,&va,&vb,&vo,
                                          nullptr,0.0,0) == SLMATH_OK &&
                      vmout[0] == 5.0 && vmout[1] == 8.0,
                      "typed VM dispatch contract");

    return failures ? 1 : 0;
}
