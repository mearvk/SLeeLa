#ifndef SLEELA_MATH_KERNEL_C_H
#define SLEELA_MATH_KERNEL_C_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Stable IDs encoded by OP_MATH_CALL. Keep append-only within this table. */
enum SLMATHFunction {
    SL_MATH_PI = 0,
    SL_MATH_E,
    SL_MATH_TAU,
    SL_MATH_ABS,
    SL_MATH_SIGN,
    SL_MATH_MIN,
    SL_MATH_MAX,
    SL_MATH_CLAMP,
    SL_MATH_SQRT,
    SL_MATH_EXP,
    SL_MATH_LOG,
    SL_MATH_LOG10,
    SL_MATH_TRUNC,
    SL_MATH_FLOOR,
    SL_MATH_CEIL,
    SL_MATH_POW,
    SL_MATH_SIN,
    SL_MATH_COS,
    SL_MATH_TAN,
    SL_MATH_HYPOT,
    SL_MATH_FMOD,
    SL_MATH_FUNCTION_COUNT
};

/* Returns required arity, or -1 for an unknown function ID. */
int sl_math_kernel_arity(int function_id);
/* Exact synthesized Core helper name, used for bytecode fallback. */
const char* sl_math_kernel_method_name(int function_id);
/* Returns 0 on success, -1 when the typed kernel declines the request. */
int sl_math_kernel_call(int function_id, const double* args,
                        uint32_t argc, double* result);

#ifdef __cplusplus
}
#endif
#endif
