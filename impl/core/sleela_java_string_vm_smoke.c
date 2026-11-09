#include "sleela_core.h"

#include <stdio.h>
#include <string.h>

static int fail(SLVM *vm, const char *what) {
    fprintf(stderr, "java-string-vm smoke failed: %s (vm=%s)\n",
            what, slvm_error(vm) ? slvm_error(vm) : "no diagnostic");
    slvm_free(vm);
    return 1;
}

int main(void) {
    static const uint16_t units[] = { 0x0041u, 0x0000u, 0xd83du, 0xde00u, 0xd800u };
    SLVM *vm = slvm_new();
    if (!vm) return 1;

    SLValue value = slvm_java_string_new(vm, units, sizeof(units) / sizeof(units[0]));
    if (value.type != SL_UTF16 || slvm_java_string_root(vm, value) != 0)
        return fail(vm, "construct/root UTF-16 value");

    int32_t length = -1, cp_count = -1, cp = -1, offset = -1;
    uint16_t unit = 0;
    if (slvm_java_string_length(vm, value, &length) != SL_JAVA_STRING_OK || length != 5)
        return fail(vm, "UTF-16 length includes embedded NUL and surrogate units");
    if (slvm_java_string_char_at(vm, value, 1, &unit) != SL_JAVA_STRING_OK || unit != 0)
        return fail(vm, "charAt preserves embedded NUL");
    if (slvm_java_string_char_at(vm, value, 4, &unit) != SL_JAVA_STRING_OK || unit != 0xd800u)
        return fail(vm, "charAt preserves isolated high surrogate");
    if (slvm_java_string_code_point_at(vm, value, 2, &cp) != SL_JAVA_STRING_OK || cp != 0x1f600)
        return fail(vm, "codePointAt combines valid surrogate pair");
    if (slvm_java_string_code_point_count(vm, value, 0, 5, &cp_count) != SL_JAVA_STRING_OK || cp_count != 4)
        return fail(vm, "codePointCount treats isolated surrogate as one code point");
    if (slvm_java_string_offset_by_code_points(vm, value, 2, 1, &offset) != SL_JAVA_STRING_OK || offset != 4)
        return fail(vm, "offsetByCodePoints advances across pair");

    SLValue pair = slvm_java_string_substring(vm, value, 2, 4);
    if (pair.type != SL_UTF16 || slvm_java_string_root(vm, pair) != 0)
        return fail(vm, "substring returns rooted UTF-16 value");
    int equal = 0;
    if (slvm_java_string_equals(vm, pair, pair, &equal) != SL_JAVA_STRING_OK || !equal)
        return fail(vm, "equals");
    int32_t hash = 0, order = 1;
    if (slvm_java_string_hash_code(vm, pair, &hash) != SL_JAVA_STRING_OK)
        return fail(vm, "hashCode");
    if (slvm_java_string_compare_to(vm, pair, pair, &order) != SL_JAVA_STRING_OK || order != 0)
        return fail(vm, "compareTo");

    SLValue joined = slvm_java_string_concat(vm, pair, pair);
    if (joined.type != SL_UTF16 || slvm_java_string_root(vm, joined) != 0)
        return fail(vm, "concat returns rooted UTF-16 value");
    if (slvm_java_string_length(vm, joined, &length) != SL_JAVA_STRING_OK || length != 4)
        return fail(vm, "concat preserves UTF-16 units");

    SLJavaString copy;
    sljava_string_init_empty(&copy);
    if (slvm_java_string_copy(vm, value, &copy) != SL_JAVA_STRING_OK ||
        copy.length != 5 || copy.units[1] != 0 || copy.units[4] != 0xd800u) {
        sljava_string_destroy(&copy);
        return fail(vm, "copy preserves all code units");
    }
    sljava_string_destroy(&copy);

    if (slvm_java_string_unroot(vm, joined) != 0 ||
        slvm_java_string_unroot(vm, pair) != 0 ||
        slvm_java_string_unroot(vm, value) != 0)
        return fail(vm, "release explicit host roots");
    slvm_free(vm);
    puts("JAVA-STRING-VM-UTF16=PASS");
    puts("JAVA-STRING-VM-NUL-SURROGATE-GC-ROOTS=PASS");
    return 0;
}
