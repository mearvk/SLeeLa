/* Standalone self-test for the SLVM/11 intXXX/uintXXX recognizer + classifier.
 * Exercises the Core-independent rules (power-of-two 2^5..2^24, the 64-bit
 * slot boundary, the twenty-width count). The full Core SLValue round-trip
 * (slval_int / slval_int128) is proven by the VM's own --selfcheck, which
 * links libslcore.a; this test needs no Core and so builds on its own. */
#include "slvm11_intxxx_module.h"
#include <stdio.h>

int main(void) {
    /* Valid series widths at the family boundaries. */
    if (!slvm11_intxxx_is_series_width(32u))       return 1; /* floor      */
    if (!slvm11_intxxx_is_series_width(64u))       return 1;
    if (!slvm11_intxxx_is_series_width(128u))      return 1;
    if (!slvm11_intxxx_is_series_width(16777216u)) return 1; /* ceiling    */

    /* Non-members: below floor, not a power of two, above ceiling. */
    if (slvm11_intxxx_is_series_width(16u))        return 2;
    if (slvm11_intxxx_is_series_width(48u))        return 2;
    if (slvm11_intxxx_is_series_width(300u))       return 2;
    if (slvm11_intxxx_is_series_width(33554432u))  return 2;

    /* Width -> runtime slot split must match the compiler: <=64 SL_INT. */
    if (slvm11_intxxx_slot_for_width(32u)       != SLVM11_SLOT_INT)    return 3;
    if (slvm11_intxxx_slot_for_width(64u)       != SLVM11_SLOT_INT)    return 3;
    if (slvm11_intxxx_slot_for_width(128u)      != SLVM11_SLOT_INT128) return 3;
    if (slvm11_intxxx_slot_for_width(16777216u) != SLVM11_SLOT_INT128) return 3;
    if (slvm11_intxxx_slot_for_width(48u)       != SLVM11_SLOT_NONE)   return 3;

    /* Exactly twenty widths per family (2^5 .. 2^24). */
    if (slvm11_intxxx_count_series_widths() != SLVM11_INTXXX_SERIES_COUNT) return 4;
    if (slvm11_intxxx_count_series_widths() != 20u)                        return 4;

    puts("SLVM/11 intXXX/uintXXX module self-test: PASS");
    return 0;
}
