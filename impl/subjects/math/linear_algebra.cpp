#include "linear_algebra.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <utility>
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


slmath_status slmath_matrix_lu(const double *a, size_t n, double *lu,
                               size_t *pivots, double tolerance) {
    size_t count = 0;
    if (!count_ok(n, n, &count)) return SLMATH_SIZE_OVERFLOW;
    if (!valid_range(a, count) || !valid_range(lu, count) ||
        !pivots || !std::isfinite(tolerance) || tolerance < 0.0)
        return SLMATH_INVALID_ARGUMENT;
    for (size_t i = 0; i < count; ++i) lu[i] = a[i];
    for (size_t i = 0; i < n; ++i) pivots[i] = i;
    for (size_t k = 0; k < n; ++k) {
        size_t pivot = k;
        double largest = std::fabs(lu[k*n+k]);
        for (size_t r = k + 1; r < n; ++r) {
            const double v = std::fabs(lu[r*n+k]);
            if (v > largest) { largest = v; pivot = r; }
        }
        if (!(largest > tolerance)) return SLMATH_SINGULAR;
        if (pivot != k) {
            for (size_t c = 0; c < n; ++c)
                std::swap(lu[k*n+c], lu[pivot*n+c]);
            std::swap(pivots[k], pivots[pivot]);
        }
        for (size_t r = k + 1; r < n; ++r) {
            lu[r*n+k] /= lu[k*n+k];
            for (size_t c = k + 1; c < n; ++c)
                lu[r*n+c] -= lu[r*n+k] * lu[k*n+c];
        }
    }
    return SLMATH_OK;
}

slmath_status slmath_matrix_lu_solve(const double *lu, const size_t *pivots,
                                     size_t n, const double *b, double *x) {
    size_t count = 0;
    if (!count_ok(n, n, &count)) return SLMATH_SIZE_OVERFLOW;
    if (!valid_range(lu, count) || !valid_range(b, n) ||
        !valid_range(x, n) || !pivots) return SLMATH_INVALID_ARGUMENT;
    std::vector<double> work;
    try { work.resize(n); } catch (...) { return SLMATH_ALLOCATION_FAILURE; }
    for (size_t i = 0; i < n; ++i) {
        if (pivots[i] >= n) return SLMATH_INVALID_ARGUMENT;
        work[i] = b[pivots[i]];
    }
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < i; ++j) work[i] -= lu[i*n+j] * work[j];
    for (size_t ii = n; ii-- > 0;) {
        for (size_t j = ii + 1; j < n; ++j)
            work[ii] -= lu[ii*n+j] * work[j];
        if (lu[ii*n+ii] == 0.0) return SLMATH_SINGULAR;
        work[ii] /= lu[ii*n+ii];
    }
    for (size_t i = 0; i < n; ++i) x[i] = work[i];
    return SLMATH_OK;
}

slmath_status slmath_matrix_inverse(const double *a, size_t n, double *out,
                                    double tolerance) {
    size_t count = 0;
    if (!count_ok(n, n, &count)) return SLMATH_SIZE_OVERFLOW;
    if (!valid_range(a, count) || !valid_range(out, count))
        return SLMATH_INVALID_ARGUMENT;
    std::vector<double> lu, rhs, solution, staged;
    std::vector<size_t> pivots;
    try {
        lu.resize(count); rhs.resize(n); solution.resize(n);
        staged.resize(count); pivots.resize(n);
    } catch (...) { return SLMATH_ALLOCATION_FAILURE; }
    slmath_status status = slmath_matrix_lu(a, n, lu.data(), pivots.data(), tolerance);
    if (status != SLMATH_OK) return status;
    for (size_t c = 0; c < n; ++c) {
        std::fill(rhs.begin(), rhs.end(), 0.0);
        rhs[c] = 1.0;
        status = slmath_matrix_lu_solve(lu.data(), pivots.data(), n,
                                        rhs.data(), solution.data());
        if (status != SLMATH_OK) return status;
        for (size_t r = 0; r < n; ++r) staged[r*n+c] = solution[r];
    }
    for (size_t i = 0; i < count; ++i) out[i] = staged[i];
    return SLMATH_OK;
}

slmath_status slmath_matrix_qr(const double *a, size_t rows, size_t cols,
                               double *q, double *r, double tolerance) {
    size_t ac = 0, qc = 0, rc = 0;
    if (rows < cols) return SLMATH_DIMENSION_MISMATCH;
    if (!count_ok(rows, cols, &ac) || !count_ok(rows, cols, &qc) ||
        !count_ok(cols, cols, &rc)) return SLMATH_SIZE_OVERFLOW;
    if (!valid_range(a, ac) || !valid_range(q, qc) || !valid_range(r, rc) ||
        !std::isfinite(tolerance) || tolerance < 0.0)
        return SLMATH_INVALID_ARGUMENT;
    std::vector<double> qwork, rwork, v;
    try { qwork.resize(qc); rwork.assign(rc, 0.0); v.resize(rows); }
    catch (...) { return SLMATH_ALLOCATION_FAILURE; }
    for (size_t c = 0; c < cols; ++c) {
        for (size_t i = 0; i < rows; ++i) v[i] = a[i*cols+c];
        for (size_t j = 0; j < c; ++j) {
            double projection = 0.0;
            for (size_t i = 0; i < rows; ++i)
                projection += qwork[i*cols+j] * v[i];
            rwork[j*cols+c] = projection;
            for (size_t i = 0; i < rows; ++i)
                v[i] -= projection * qwork[i*cols+j];
        }
        double norm = 0.0;
        for (size_t i = 0; i < rows; ++i) norm = std::hypot(norm, v[i]);
        if (!(norm > tolerance)) return SLMATH_SINGULAR;
        rwork[c*cols+c] = norm;
        for (size_t i = 0; i < rows; ++i) qwork[i*cols+c] = v[i] / norm;
    }
    for (size_t i = 0; i < qc; ++i) q[i] = qwork[i];
    for (size_t i = 0; i < rc; ++i) r[i] = rwork[i];
    return SLMATH_OK;
}

slmath_complex slmath_complex_add(slmath_complex a, slmath_complex b) {
    return {a.re+b.re, a.im+b.im};
}
slmath_complex slmath_complex_subtract(slmath_complex a, slmath_complex b) {
    return {a.re-b.re, a.im-b.im};
}
slmath_complex slmath_complex_multiply(slmath_complex a, slmath_complex b) {
    return {a.re*b.re-a.im*b.im, a.re*b.im+a.im*b.re};
}
slmath_status slmath_complex_divide(slmath_complex a, slmath_complex b,
                                    slmath_complex *out) {
    if (!out || (b.re == 0.0 && b.im == 0.0))
        return SLMATH_INVALID_ARGUMENT;
    /* Smith's scaled division avoids squaring large components. */
    if (std::fabs(b.re) >= std::fabs(b.im)) {
        const double ratio = b.im / b.re;
        const double den = b.re + b.im * ratio;
        out->re = (a.re + a.im * ratio) / den;
        out->im = (a.im - a.re * ratio) / den;
    } else {
        const double ratio = b.re / b.im;
        const double den = b.im + b.re * ratio;
        out->re = (a.re * ratio + a.im) / den;
        out->im = (a.im * ratio - a.re) / den;
    }
    return SLMATH_OK;
}
double slmath_complex_abs(slmath_complex z) { return std::hypot(z.re, z.im); }
double slmath_complex_phase(slmath_complex z) { return std::atan2(z.im, z.re); }
slmath_complex slmath_complex_conjugate(slmath_complex z) { return {z.re, -z.im}; }

slmath_status slmath_array_copy(const double *src, double *dst, size_t n) {
    if (!valid_range(src, n) || !valid_range(dst, n))
        return SLMATH_INVALID_ARGUMENT;
    if (src != dst) std::memmove(dst, src, n * sizeof(double));
    return SLMATH_OK;
}

slmath_status slmath_array_slice(const double *src, size_t src_n,
                                 size_t start, size_t count, size_t stride,
                                 double *dst) {
    if (!src || !dst || stride == 0 || start > src_n)
        return SLMATH_INVALID_ARGUMENT;
    if (count && (count - 1 > (std::numeric_limits<size_t>::max()-start)/stride ||
                  start + (count-1)*stride >= src_n))
        return SLMATH_INVALID_ARGUMENT;
    if (!valid_range(src, src_n) || !valid_range(dst, count))
        return SLMATH_INVALID_ARGUMENT;
    for (size_t i = 0; i < count; ++i) dst[i] = src[start+i*stride];
    return SLMATH_OK;
}

slmath_status slmath_array_binary(const double *a, const double *b,
                                  double *out, size_t n, int operation) {
    if (operation < 0 || operation > 3) return SLMATH_INVALID_ARGUMENT;
    return vector_binary(a, b, out, n, [operation](double x, double y) {
        switch (operation) {
            case 0: return x+y;
            case 1: return x-y;
            case 2: return x*y;
            default: return y == 0.0 ? std::numeric_limits<double>::quiet_NaN() : x/y;
        }
    });
}

slmath_status slmath_symmetric_eigen(const double *a, size_t n,
                                     double *eigenvalues, double *eigenvectors,
                                     size_t max_sweeps, double tolerance) {
    size_t count = 0;
    if (!count_ok(n, n, &count)) return SLMATH_SIZE_OVERFLOW;
    if (!valid_range(a, count) || !valid_range(eigenvectors, count) ||
        !valid_range(eigenvalues, n) || !std::isfinite(tolerance) || tolerance < 0.0)
        return SLMATH_INVALID_ARGUMENT;
    for (size_t row=0; row<n; ++row) for (size_t col=row; col<n; ++col) {
        if (!std::isfinite(a[row*n+col]) || !std::isfinite(a[col*n+row]) ||
            std::fabs(a[row*n+col]-a[col*n+row]) > tolerance)
            return SLMATH_INVALID_ARGUMENT;
    }
    std::vector<double> m, v;
    try { m.assign(a, a+count); v.assign(count, 0.0); }
    catch (...) { return SLMATH_ALLOCATION_FAILURE; }
    for (size_t i = 0; i < n; ++i) v[i*n+i] = 1.0;
    for (size_t sweep = 0; sweep < max_sweeps; ++sweep) {
        double largest = 0.0;
        for (size_t p = 0; p < n; ++p)
            for (size_t q = p+1; q < n; ++q)
                largest = std::max(largest, std::fabs(m[p*n+q]));
        if (largest <= tolerance) {
            for (size_t i = 0; i < n; ++i) eigenvalues[i] = m[i*n+i];
            for (size_t i = 0; i < count; ++i) eigenvectors[i] = v[i];
            return SLMATH_OK;
        }
        for (size_t p = 0; p < n; ++p) for (size_t q = p+1; q < n; ++q) {
            const double apq = m[p*n+q];
            if (std::fabs(apq) <= tolerance) continue;
            const double tau = (m[q*n+q]-m[p*n+p])/(2.0*apq);
            const double t = (tau >= 0.0 ? 1.0 : -1.0) /
                             (std::fabs(tau)+std::hypot(1.0,tau));
            const double c = 1.0/std::hypot(1.0,t);
            const double s = t*c;
            const double app=m[p*n+p], aqq=m[q*n+q];
            m[p*n+p]=app-t*apq; m[q*n+q]=aqq+t*apq;
            m[p*n+q]=m[q*n+p]=0.0;
            for (size_t k=0;k<n;++k) if(k!=p && k!=q) {
                const double mkp=m[k*n+p], mkq=m[k*n+q];
                m[k*n+p]=m[p*n+k]=c*mkp-s*mkq;
                m[k*n+q]=m[q*n+k]=s*mkp+c*mkq;
            }
            for (size_t k=0;k<n;++k) {
                const double vkp=v[k*n+p], vkq=v[k*n+q];
                v[k*n+p]=c*vkp-s*vkq;
                v[k*n+q]=s*vkp+c*vkq;
            }
        }
    }
    return SLMATH_NO_CONVERGENCE; /* Did not converge within caller's sweep budget. */
}

slmath_status slmath_fft(slmath_complex *values, size_t n, int inverse) {
    if (!values || n == 0 || (n & (n-1)) != 0 || (inverse != 0 && inverse != 1))
        return SLMATH_INVALID_ARGUMENT;
    if (n > std::numeric_limits<size_t>::max()/sizeof(slmath_complex))
        return SLMATH_SIZE_OVERFLOW;
    for (size_t i=1,j=0;i<n;++i) {
        size_t bit=n>>1;
        for (; j&bit; bit>>=1) j^=bit;
        j^=bit;
        if (i<j) std::swap(values[i],values[j]);
    }
    const double pi=3.1415926535897932384626433832795;
    for (size_t len=2;len<=n;len<<=1) {
        const double angle=(inverse?2.0:-2.0)*pi/static_cast<double>(len);
        const slmath_complex root={std::cos(angle),std::sin(angle)};
        for (size_t base=0;base<n;base+=len) {
            slmath_complex w={1.0,0.0};
            for (size_t j=0;j<len/2;++j) {
                const slmath_complex even=values[base+j];
                const slmath_complex odd=slmath_complex_multiply(values[base+j+len/2],w);
                values[base+j]=slmath_complex_add(even,odd);
                values[base+j+len/2]=slmath_complex_subtract(even,odd);
                w=slmath_complex_multiply(w,root);
            }
        }
        if (len > n/2) break; /* Prevent size_t shift overflow at the top stage. */
    }
    if (inverse) for(size_t i=0;i<n;++i) {
        values[i].re/=static_cast<double>(n);
        values[i].im/=static_cast<double>(n);
    }
    return SLMATH_OK;
}


slmath_status slmath_vm_dispatch(slmath_operation operation,
                                 const slmath_buffer *a,
                                 const slmath_buffer *b,
                                 slmath_buffer *out,
                                 slmath_buffer *aux,
                                 double scalar, size_t flags) {
    if (!a || !out || !a->data || !out->data)
        return SLMATH_INVALID_ARGUMENT;
    size_t a_shape = 0, out_shape = 0, b_shape = 0, aux_shape = 0;
    if (!count_ok(a->rows,a->cols,&a_shape) ||
        !count_ok(out->rows,out->cols,&out_shape))
        return SLMATH_SIZE_OVERFLOW;
    if (a->rows && a->cols && a->length < a_shape)
        return SLMATH_DIMENSION_MISMATCH;
    if (out->rows && out->cols && out->length < out_shape)
        return SLMATH_DIMENSION_MISMATCH;
    if (b && !count_ok(b->rows,b->cols,&b_shape))
        return SLMATH_SIZE_OVERFLOW;
    if (aux && !count_ok(aux->rows,aux->cols,&aux_shape))
        return SLMATH_SIZE_OVERFLOW;
    switch (operation) {
    case SLMATH_OP_VECTOR_ADD:
        if (!b || !b->data || a->length != b->length ||
            out->length < a->length) return SLMATH_DIMENSION_MISMATCH;
        return slmath_vector_add(a->data,b->data,out->data,a->length);
    case SLMATH_OP_VECTOR_DOT:
        if (!b || !b->data || a->length != b->length || out->length < 1)
            return SLMATH_DIMENSION_MISMATCH;
        return slmath_vector_dot(a->data,b->data,a->length,out->data);
    case SLMATH_OP_VECTOR_NORM:
        if (out->length < 1) return SLMATH_DIMENSION_MISMATCH;
        return slmath_vector_norm(a->data,a->length,out->data);
    case SLMATH_OP_VECTOR_SCALE:
        if (out->length < a->length) return SLMATH_DIMENSION_MISMATCH;
        return slmath_vector_scale(a->data,scalar,out->data,a->length);
    case SLMATH_OP_VECTOR_SUBTRACT:
        if (!b || !b->data || a->length != b->length ||
            out->length < a->length) return SLMATH_DIMENSION_MISMATCH;
        return slmath_vector_subtract(a->data,b->data,out->data,a->length);
    case SLMATH_OP_VECTOR_NORMALIZE:
        if (out->length < a->length) return SLMATH_DIMENSION_MISMATCH;
        return slmath_vector_normalize(a->data,out->data,a->length);
    case SLMATH_OP_VECTOR_CROSS3:
        if (!b || !b->data || a->length < 3 || b->length < 3 ||
            out->length < 3) return SLMATH_DIMENSION_MISMATCH;
        return slmath_vector_cross3(a->data,b->data,out->data);
    case SLMATH_OP_COMPLEX_ADD:
    case SLMATH_OP_COMPLEX_SUBTRACT:
    case SLMATH_OP_COMPLEX_MULTIPLY:
    case SLMATH_OP_COMPLEX_DIVIDE:
        if (!b || !b->data || a->length < 2 || b->length < 2 ||
            out->length < 2) return SLMATH_DIMENSION_MISMATCH;
        {
            const slmath_complex x={a->data[0],a->data[1]};
            const slmath_complex y={b->data[0],b->data[1]};
            slmath_complex z{};
            if (operation == SLMATH_OP_COMPLEX_ADD) z=slmath_complex_add(x,y);
            else if (operation == SLMATH_OP_COMPLEX_SUBTRACT) z=slmath_complex_subtract(x,y);
            else if (operation == SLMATH_OP_COMPLEX_MULTIPLY) z=slmath_complex_multiply(x,y);
            else {
                slmath_status status=slmath_complex_divide(x,y,&z);
                if(status!=SLMATH_OK) return status;
            }
            out->data[0]=z.re; out->data[1]=z.im;
        }
        return SLMATH_OK;
    case SLMATH_OP_MATRIX_MULTIPLY:
        if (!b || !b->data || a->cols != b->rows ||
            a->length < a_shape || b->length < b_shape ||
            out->rows != a->rows || out->cols != b->cols ||
            out->length < out_shape)
            return SLMATH_DIMENSION_MISMATCH;
        return slmath_matrix_multiply(a->data,a->rows,a->cols,b->data,
                                      b->rows,b->cols,out->data);
    case SLMATH_OP_MATRIX_TRANSPOSE:
        if (a->length < a_shape || out->rows != a->cols ||
            out->cols != a->rows || out->length < out_shape)
            return SLMATH_DIMENSION_MISMATCH;
        return slmath_matrix_transpose(a->data,a->rows,a->cols,out->data);
    case SLMATH_OP_MATRIX_INVERSE:
        if (a->rows != a->cols || a->length < a_shape ||
            out->rows != a->rows || out->cols != a->cols ||
            out->length < out_shape)
            return SLMATH_DIMENSION_MISMATCH;
        return slmath_matrix_inverse(a->data,a->rows,out->data,scalar);
    case SLMATH_OP_COMPLEX_MULTIPLY:
        if (!b || !b->data || a->length < 2 || b->length < 2 ||
            out->length < 2) return SLMATH_DIMENSION_MISMATCH;
        {
            const slmath_complex x={a->data[0],a->data[1]};
            const slmath_complex y={b->data[0],b->data[1]};
            const slmath_complex z=slmath_complex_multiply(x,y);
            out->data[0]=z.re; out->data[1]=z.im;
        }
        return SLMATH_OK;
    case SLMATH_OP_QR:
        if (!aux || !aux->data || a->rows < a->cols ||
            a->length < a_shape ||
            out->rows != a->rows || out->cols != a->cols ||
            out->length < a_shape ||
            aux->rows != a->cols || aux->cols != a->cols ||
            aux->length < aux_shape)
            return SLMATH_DIMENSION_MISMATCH;
        return slmath_matrix_qr(a->data,a->rows,a->cols,out->data,
                                aux->data,scalar);
    case SLMATH_OP_SYMMETRIC_EIGEN:
        if (!aux || !aux->data || a->rows != a->cols ||
            a->length < a_shape || out->rows != 1 || out->cols != a->rows ||
            out->length < a->rows || aux->rows != a->rows || aux->cols != a->rows ||
            aux->length < a_shape)
            return SLMATH_DIMENSION_MISMATCH;
        return slmath_symmetric_eigen(a->data,a->rows,out->data,aux->data,
                                      flags,scalar);
    case SLMATH_OP_FFT:
        if (out->length < a->length || (a->length % 2) != 0)
            return SLMATH_DIMENSION_MISMATCH;
        if (out->data != a->data)
            std::memmove(out->data,a->data,a->length*sizeof(double));
        return slmath_fft(reinterpret_cast<slmath_complex*>(out->data),
                          a->length/2,flags ? 1 : 0);
    default:
        return SLMATH_INVALID_ARGUMENT;
    }
}

} // extern "C"
