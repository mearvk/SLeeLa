/* ==========================================================================
 * slvm11_intxxx_module.c  --  SLVM/11 intXXX/uintXXX number-series support.
 *
 * Implements the recognizer + admission for the SLeeLa fixed-width integer
 * number series: the power-of-two width rule (2^5..2^24), the twenty-width
 * count per family, and the width -> runtime-slot split (SL_INT for <=64,
 * SL_INT128 for >=128) that matches the compiler. This is the same "admit a
 * real, well-formed instance; a malformed one is rejected" discipline every
 * SLVM/NN grade feature uses -- here applied to the number series so the VM
 * demonstrably supports intXXX, not merely claims to. The live Core SLValue
 * round-trip for each slot is proven alongside, in the VM runner's admission
 * (which links libslcore.a); this module stays Core-independent.
 *
 * Copyright (c) Max Rupplin - MEARVK LLC - 2026
 * ========================================================================== */
#include "slvm11_intxxx_module.h"

/* 1 iff w is an exact power of two (w != 0 and only one bit set). */
static int slvm11_is_power_of_two(uint32_t w) {
    return w != 0u && (w & (w - 1u)) == 0u;
}

int slvm11_intxxx_is_series_width(uint32_t w) {
    if (!slvm11_is_power_of_two(w)) return 0;
    if (w < SLVM11_INTXXX_MIN_WIDTH) return 0;
    if (w > SLVM11_INTXXX_MAX_WIDTH) return 0;
    return 1;
}

slvm11_intxxx_slot_t slvm11_intxxx_slot_for_width(uint32_t w) {
    if (!slvm11_intxxx_is_series_width(w)) return SLVM11_SLOT_NONE;
    return (w <= SLVM11_INTXXX_NATIVE_NARROW) ? SLVM11_SLOT_INT : SLVM11_SLOT_INT128;
}

uint32_t slvm11_intxxx_count_series_widths(void) {
    uint32_t count = 0u;
    uint32_t w;
    /* Walk the powers of two from MIN_WIDTH to MAX_WIDTH inclusive. */
    for (w = SLVM11_INTXXX_MIN_WIDTH; w <= SLVM11_INTXXX_MAX_WIDTH; w <<= 1) {
        if (slvm11_intxxx_is_series_width(w)) count++;
    }
    return count;
}

int slvm11_intxxx_module_validate(const slvm11_intxxx_module_t* m) {
    if (m == 0) return -1;

    /* 1) Descriptor must declare the canonical series bounds. */
    if (m->min_width    != SLVM11_INTXXX_MIN_WIDTH)     return -1;
    if (m->max_width    != SLVM11_INTXXX_MAX_WIDTH)     return -1;
    if (m->narrow_width != SLVM11_INTXXX_NATIVE_NARROW) return -1;
    if (m->series_count != SLVM11_INTXXX_SERIES_COUNT)  return -1;

    /* 2) It must claim at least one family, and the declared count must match
     *    the recognizer's own enumeration (guards a bogus series_count). */
    if (!m->has_signed && !m->has_unsigned)             return -1;
    if (slvm11_intxxx_count_series_widths() != m->series_count) return -1;

    /* 3) Width -> slot classification must match the compiler's split exactly
     *    at the family boundaries: int32/int64 are SL_INT; int128 and wider
     *    are SL_INT128; just-out-of-range widths name no slot. */
    if (slvm11_intxxx_slot_for_width(32u)       != SLVM11_SLOT_INT)    return -1;
    if (slvm11_intxxx_slot_for_width(64u)       != SLVM11_SLOT_INT)    return -1;
    if (slvm11_intxxx_slot_for_width(128u)      != SLVM11_SLOT_INT128) return -1;
    if (slvm11_intxxx_slot_for_width(16777216u) != SLVM11_SLOT_INT128) return -1;
    if (slvm11_intxxx_slot_for_width(16u)       != SLVM11_SLOT_NONE)   return -1; /* below floor  */
    if (slvm11_intxxx_slot_for_width(48u)       != SLVM11_SLOT_NONE)   return -1; /* not pow2      */
    if (slvm11_intxxx_slot_for_width(33554432u) != SLVM11_SLOT_NONE)   return -1; /* above ceiling */

    /* The live Core SLValue round-trip (that SL_INT / SL_INT128 actually carry
     * a value of each width) is proven by the VM runner's admission, which
     * links libslcore.a. This module stays Core-independent so it validates the
     * series DISCIPLINE on its own, exactly like the sibling grade features. */
    return 0;
}

int slvm11_intxxx_module_is_full_series(const slvm11_intxxx_module_t* m) {
    if (m == 0) return 0;
    return (m->has_signed && m->has_unsigned &&
            m->min_width    == SLVM11_INTXXX_MIN_WIDTH &&
            m->max_width    == SLVM11_INTXXX_MAX_WIDTH &&
            m->series_count == SLVM11_INTXXX_SERIES_COUNT) ? 1 : 0;
}
