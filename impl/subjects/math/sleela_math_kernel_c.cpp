#include "sleela_math_kernel_c.h"
#include "math_kernel.hpp"

#include <algorithm>
#include <cmath>

using namespace sleela::native::math_kernel;

namespace {
static const char* const kNames[SL_MATH_FUNCTION_COUNT] = {
    "__native_math_pi", "__native_math_e", "__native_math_tau",
    "__native_math_abs", "__native_math_sign", "__native_math_min",
    "__native_math_max", "__native_math_clamp", "__native_math_sqrt",
    "__native_math_exp", "__native_math_log", "__native_math_log10",
    "__native_math_trunc", "__native_math_floor", "__native_math_ceil",
    "__native_math_pow", "__native_math_sin", "__native_math_cos",
    "__native_math_tan", "__native_math_hypot", "__native_math_fmod"
};
static const int kArities[SL_MATH_FUNCTION_COUNT] = {
    0, 0, 0, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 2
};
}

extern "C" int sl_math_kernel_arity(int function_id) {
    return function_id >= 0 && function_id < SL_MATH_FUNCTION_COUNT
        ? kArities[function_id] : -1;
}

extern "C" const char* sl_math_kernel_method_name(int function_id) {
    return function_id >= 0 && function_id < SL_MATH_FUNCTION_COUNT
        ? kNames[function_id] : nullptr;
}

extern "C" int sl_math_kernel_call(int function_id, const double* a,
                                    uint32_t argc, double* result) {
    if (!result || sl_math_kernel_arity(function_id) < 0 ||
        argc != static_cast<uint32_t>(kArities[function_id]) ||
        (argc && !a)) return -1;
    switch (function_id) {
        case SL_MATH_PI: *result = 3.14159265358979323846264338327950288; break;
        case SL_MATH_E: *result = 2.71828182845904523536028747135266250; break;
        case SL_MATH_TAU: *result = 6.28318530717958647692528676655900576; break;
        case SL_MATH_ABS: *result = abs(a[0]); break;
        case SL_MATH_SIGN:
            *result = std::isnan(a[0]) ? a[0] : (a[0] > 0.0 ? 1.0 : (a[0] < 0.0 ? -1.0 : a[0]));
            break;
        case SL_MATH_MIN: *result = std::fmin(a[0], a[1]); break;
        case SL_MATH_MAX: *result = std::fmax(a[0], a[1]); break;
        case SL_MATH_CLAMP: *result = std::fmin(std::fmax(a[0], a[1]), a[2]); break;
        case SL_MATH_SQRT: *result = sqrt(a[0]); break;
        case SL_MATH_EXP: *result = exp(a[0]); break;
        case SL_MATH_LOG: *result = log(a[0]); break;
        case SL_MATH_LOG10: *result = log10(a[0]); break;
        case SL_MATH_TRUNC: *result = trunc(a[0]); break;
        case SL_MATH_FLOOR: *result = floor(a[0]); break;
        case SL_MATH_CEIL: *result = ceil(a[0]); break;
        case SL_MATH_POW: *result = pow(a[0], a[1]); break;
        case SL_MATH_SIN: *result = sin(a[0]); break;
        case SL_MATH_COS: *result = cos(a[0]); break;
        case SL_MATH_TAN: *result = tan(a[0]); break;
        case SL_MATH_HYPOT: *result = hypot(a[0], a[1]); break;
        case SL_MATH_FMOD: *result = fmod(a[0], a[1]); break;
        default: return -1;
    }
    return 0;
}
