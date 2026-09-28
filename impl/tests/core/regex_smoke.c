#include "sleela_regex.h"
#include <assert.h>
#include <stdio.h>
static void expect(const char *text, const char *pattern, SleelaRegexLevel level, int matched) {
    SleelaRegexResult r;
    SleelaRegexStatus s = sleela_regex_match(text, pattern, level, &r);
    assert(s == SLEELA_REGEX_OK); assert(r.matched == matched);
}
int main(void) {
    expect("hello SLeeLa", "contains SLeeLa", SLEELA_REGEX_LEVEL_FIND, 1);
    expect("SLeeLa Regex", "starts SLeeLa", SLEELA_REGEX_LEVEL_FIND, 1);
    expect("report.txt", "ends .txt", SLEELA_REGEX_LEVEL_FIND, 1);
    expect("ready", "exact ready", SLEELA_REGEX_LEVEL_FIND, 1);
    expect("2026", "exact digit{4}", SLEELA_REGEX_LEVEL_SHAPE, 1);
    expect("1234", "exact digit{2,4}", SLEELA_REGEX_LEVEL_SHAPE, 1);
    expect("1a", "exact digit+", SLEELA_REGEX_LEVEL_SHAPE, 0);
    expect("hello", "exact word+", SLEELA_REGEX_LEVEL_SHAPE, 1);
    expect("   ", "exact space+", SLEELA_REGEX_LEVEL_SHAPE, 1);
    {
        SleelaRegexResult r;
        assert(sleela_regex_match("cat", "exact choice(cat, dog)",
                                  SLEELA_REGEX_LEVEL_STRUCTURE, &r) ==
               SLEELA_REGEX_UNSUPPORTED_LEVEL);
    }
    puts("SLeeLa Regex native foundation: PASS");
    return 0;
}
