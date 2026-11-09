#include "sleela_java_string.h"
#include <stdio.h>
#include <string.h>

#define CHECK(expr, name) do { if (!(expr)) { \
    fprintf(stderr, "JAVA-STRING-FAIL: %s (line %d)\n", name, __LINE__); return 1; \
} } while (0)

int main(void) {
    /* Includes an embedded NUL, a valid surrogate pair, and an isolated low
       surrogate. All are legal Java String code-unit sequences. */
    const uint16_t source[] = { 'A', 0x0000, 0xd83d, 0xde00, 0xdc00, 'Z' };
    const uint16_t abc[] = { 'a', 'b', 'c' };
    const uint16_t expected_tail[] = { 0xdc00, 'Z' };
    SLJavaString value, copy, tail, joined, abc_value;
    uint16_t unit = 0;
    int32_t number = 0, hash = 0, order = 0, index = 0;
    int equal = 0;

    sljava_string_init_empty(&value);
    sljava_string_init_empty(&copy);
    sljava_string_init_empty(&tail);
    sljava_string_init_empty(&joined);
    sljava_string_init_empty(&abc_value);

    CHECK(sljava_string_from_utf16(&value, source, 6) == SL_JAVA_STRING_OK, "construct");
    CHECK(sljava_string_length(&value, &number) == 0 && number == 6, "UTF-16 length");
    CHECK(sljava_string_char_at(&value, 1, &unit) == 0 && unit == 0, "embedded NUL");
    CHECK(sljava_string_char_at(&value, 2, &unit) == 0 && unit == 0xd83d, "high surrogate");
    CHECK(sljava_string_code_point_at(&value, 2, &number) == 0 && number == 0x1f600,
          "supplementary code point");
    CHECK(sljava_string_code_point_at(&value, 4, &number) == 0 && number == 0xdc00,
          "isolated surrogate code point");
    CHECK(sljava_string_code_point_count(&value, 0, 6, &number) == 0 && number == 5,
          "code point count");
    CHECK(sljava_string_offset_by_code_points(&value, 2, 1, &index) == 0 && index == 4,
          "offset forward over pair");
    CHECK(sljava_string_offset_by_code_points(&value, 4, -1, &index) == 0 && index == 2,
          "offset backward over pair");
    CHECK(sljava_string_substring(&tail, &value, 4, 6) == 0, "substring");
    CHECK(sljava_string_from_utf16(&copy, expected_tail, 2) == 0, "construct expected");
    CHECK(sljava_string_equals(&tail, &copy, &equal) == 0 && equal, "substring retains raw units");
    CHECK(sljava_string_concat(&joined, &tail, &copy) == 0, "concat");
    CHECK(sljava_string_length(&joined, &number) == 0 && number == 4, "concat length");
    CHECK(sljava_string_from_utf16(&abc_value, abc, 3) == 0, "construct abc");
    CHECK(sljava_string_hash_code(&abc_value, &hash) == 0 && hash == 96354,
          "Java hashCode");
    CHECK(sljava_string_compare_to(&abc_value, &copy, &order) == 0 && order < 0,
          "UTF-16 compareTo");
    CHECK(sljava_string_char_at(&value, 6, &unit) == SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS,
          "bounds failure");

    sljava_string_destroy(&abc_value);
    sljava_string_destroy(&joined);
    sljava_string_destroy(&copy);
    sljava_string_destroy(&tail);
    sljava_string_destroy(&value);
    puts("JAVA-STRING-UTF16=PASS");
    puts("JAVA-STRING-NUL-SURROGATE-HASH=PASS");
    return 0;
}
