/* ==========================================================================
 * slvm11_intxxx_module.h  --  SLVM/11 "Sovereign" intXXX/uintXXX number support.
 *
 * SLVM/11 adds first-class admission for SLeeLa's fixed-width integer number
 * series -- the signed  int32 .. int16777216  (syntax 2.7) and the unsigned
 * uint32 .. uint16777216 (syntax 2.8). A member is the prefix ("int"/"uint")
 * followed by a bit-width that must be a power of two in [2^5, 2^24]; twenty
 * widths each. This module lets the VM PROVE, at admission, that it genuinely
 * carries the series through the shared native Core value model, using the
 * exact width -> runtime-slot split the compiler uses:
 *
 *     width <= 64 bits  (int32/int64, uint32/uint64)  -> SL_INT    (int64_t)
 *     width >= 128 bits (int128.., uint128..)         -> SL_INT128 (__int128)
 *
 * Unsigned-ness is a compile-time (analyzer) discipline -- positive-only,
 * width-strict -- so at runtime a uint<width> shares the SL_INT / SL_INT128
 * tag with the signed series. This module validates both the type-name series
 * and that real Core SLValue round-trips preserve a value in its slot.
 *
 * Copyright (c) Max Rupplin - MEARVK LLC - 2026
 * ========================================================================== */
#ifndef SLVM11_INTXXX_MODULE_H
#define SLVM11_INTXXX_MODULE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Series bounds, shared by the signed and unsigned families. Mirrors
 * impl/frontend/wide_integer.h (kMinimumWidth / kMaximumWidth) and
 * SLEELA.syntax Sections 24 (signed) and 25 (unsigned). */
#define SLVM11_INTXXX_MIN_WIDTH        32u         /* 2^5  */
#define SLVM11_INTXXX_MAX_WIDTH        16777216u   /* 2^24 */
#define SLVM11_INTXXX_NATIVE_NARROW    64u         /* <= this width uses SL_INT */
#define SLVM11_INTXXX_SERIES_COUNT     20u         /* widths per family (2^5..2^24) */

/* Runtime slot a width maps to, matching the Core SLType tags SL_INT / SL_INT128. */
typedef enum {
    SLVM11_SLOT_NONE  = 0,   /* not a valid series width                 */
    SLVM11_SLOT_INT   = 1,   /* width <= 64  -> SL_INT    (int64_t slot)  */
    SLVM11_SLOT_INT128 = 2   /* width >= 128 -> SL_INT128 (__int128 slot) */
} slvm11_intxxx_slot_t;

/* Signedness of a series member. */
typedef enum {
    SLVM11_SIGN_SIGNED   = 0,  /* int<width>  : signed, two's complement   */
    SLVM11_SIGN_UNSIGNED = 1   /* uint<width> : unsigned, positive-only     */
} slvm11_intxxx_sign_t;

/* An admission descriptor: the series this VM claims to support. A well-formed
 * descriptor names the real SLeeLa series; the validator checks it against the
 * recognizer rules below, not against a constant. */
typedef struct {
    uint32_t             min_width;     /* must equal SLVM11_INTXXX_MIN_WIDTH   */
    uint32_t             max_width;     /* must equal SLVM11_INTXXX_MAX_WIDTH   */
    uint32_t             narrow_width;  /* must equal SLVM11_INTXXX_NATIVE_NARROW */
    uint32_t             series_count;  /* must equal SLVM11_INTXXX_SERIES_COUNT */
    int                  has_signed;    /* 1: supports the int<width> family     */
    int                  has_unsigned;  /* 1: supports the uint<width> family    */
} slvm11_intxxx_module_t;

/* --- Recognizer helpers (header-faithful mirror of wide_integer.h) --- */

/* 1 iff `w` is a power of two within [MIN_WIDTH, MAX_WIDTH] -- i.e. a real
 * member width of the int<width>/uint<width> series. */
int slvm11_intxxx_is_series_width(uint32_t w);

/* The runtime slot a width maps to (SLVM11_SLOT_NONE for a non-series width). */
slvm11_intxxx_slot_t slvm11_intxxx_slot_for_width(uint32_t w);

/* Count of valid series widths in [MIN_WIDTH, MAX_WIDTH]; must be SERIES_COUNT. */
uint32_t slvm11_intxxx_count_series_widths(void);

/* --- Admission --- */

/* Validate an admission descriptor against the series rules: canonical bounds
 * (2^5..2^24), the 64-bit narrow boundary, the twenty-width count, at least one
 * family, and the width -> runtime-slot classification (SL_INT for <=64,
 * SL_INT128 for >=128). Returns 0 if well-formed, nonzero otherwise. The live
 * Core SLValue round-trip for each slot is proven by the VM runner's admission
 * (which links the Core), keeping this module Core-independent. */
int slvm11_intxxx_module_validate(const slvm11_intxxx_module_t* m);

/* 1 iff the descriptor is the full, canonical SLeeLa series (both families,
 * exact bounds). Used by the VM to confirm complete support. */
int slvm11_intxxx_module_is_full_series(const slvm11_intxxx_module_t* m);

#ifdef __cplusplus
}
#endif

#endif /* SLVM11_INTXXX_MODULE_H */
