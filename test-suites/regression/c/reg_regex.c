/*
 * reg_regex.c -- regression suite for impl/core/sleela_regex.c
 *
 * Contract (impl/core/sleela_regex.h, REGEX-NATIVE.md): a small, explicit
 * matcher.  Levels 1 (FIND) and 2 (SHAPE) are supported; 3 and 4 report
 * SLEELA_REGEX_UNSUPPORTED_LEVEL.  Patterns are "contains <lit>",
 * "starts <lit>", "ends <lit>" and "exact <lit-or-shape>", where a shape is
 * digit|word|space|any with an optional + * {n} {n,} {n,m} quantifier.
 */
#include "../../../impl/core/sleela_regex.h"
#include "regression.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef SleelaRegexResult R;

static SleelaRegexStatus run(const char *t, const char *p, int level, R *r) {
    return sleela_regex_match(t, p, (SleelaRegexLevel)level, r);
}

static int matched(const char *t, const char *p, int level) {
    R r; SleelaRegexStatus s = run(t, p, level, &r);
    return s == SLEELA_REGEX_OK && r.matched;
}

static void t_status_names(void) {
    REG_EQ_S("ok", sleela_regex_status_name(SLEELA_REGEX_OK));
    REG_EQ_S("invalid-argument", sleela_regex_status_name(SLEELA_REGEX_INVALID_ARGUMENT));
    REG_EQ_S("invalid-pattern", sleela_regex_status_name(SLEELA_REGEX_INVALID_PATTERN));
    REG_EQ_S("unsupported-level", sleela_regex_status_name(SLEELA_REGEX_UNSUPPORTED_LEVEL));
    REG_EQ_S("unsupported-pattern", sleela_regex_status_name(SLEELA_REGEX_UNSUPPORTED_PATTERN));
    REG_EQ_S("unknown", sleela_regex_status_name((SleelaRegexStatus)99));
    REG_EQ_S("unknown", sleela_regex_status_name((SleelaRegexStatus)-1));
    REG_EQ_I(0, SLEELA_REGEX_OK); REG_EQ_I(1, SLEELA_REGEX_LEVEL_FIND); REG_EQ_I(4, SLEELA_REGEX_LEVEL_EXPRESSION);
}

static void t_argument_and_level_validation(void) {
    R r; memset(&r, 0xAB, sizeof r);
    REG_EQ_I(SLEELA_REGEX_INVALID_ARGUMENT, run(NULL, "contains a", 1, &r));
    REG_EQ_I(SLEELA_REGEX_INVALID_ARGUMENT, run("a", NULL, 1, &r));
    REG_EQ_I(SLEELA_REGEX_INVALID_ARGUMENT, run("a", "contains a", 1, NULL));
    REG_EQ_I(SLEELA_REGEX_INVALID_ARGUMENT, run("a", "contains a", 0, &r));
    REG_CHECK(r.matched == 0 && r.length == 0);            /* result is always reset */
    REG_EQ_I(SLEELA_REGEX_INVALID_ARGUMENT, run("a", "contains a", 5, &r));
    REG_EQ_I(SLEELA_REGEX_INVALID_ARGUMENT, run("a", "contains a", -1, &r));
    /* levels 3 and 4 are recognised but unsupported, even for a valid pattern */
    memset(&r, 0xAB, sizeof r);
    REG_EQ_I(SLEELA_REGEX_UNSUPPORTED_LEVEL, run("a", "contains a", 3, &r));
    REG_CHECK(r.matched == 0); REG_EQ_I(3, r.level);
    REG_EQ_I(SLEELA_REGEX_UNSUPPORTED_LEVEL, run("a", "exact digit+", 4, &r));
    REG_EQ_I(4, r.level);
}

static void t_pattern_prefix_grammar(void) {
    R r;
    REG_EQ_I(SLEELA_REGEX_INVALID_PATTERN, run("a", "", 1, &r));
    REG_EQ_I(SLEELA_REGEX_INVALID_PATTERN, run("a", "a", 1, &r));
    REG_EQ_I(SLEELA_REGEX_INVALID_PATTERN, run("a", "contains", 1, &r));   /* needs the space */
    REG_EQ_I(SLEELA_REGEX_INVALID_PATTERN, run("a", "exact", 2, &r));
    REG_EQ_I(SLEELA_REGEX_INVALID_PATTERN, run("a", "Contains a", 1, &r)); /* case-sensitive */
    REG_EQ_I(SLEELA_REGEX_INVALID_PATTERN, run("a", " contains a", 1, &r));
    REG_EQ_I(SLEELA_REGEX_INVALID_PATTERN, run("a", "match a", 2, &r));
    REG_CHECK(r.matched == 0);
}

static void t_find_family_positions(void) {
    R r;
    for (int level = 1; level <= 2; ++level) {
        REG_EQ_I(0, run("hello", "contains lo", level, &r));
        REG_CHECK(r.matched); REG_EQ_U(3, r.start); REG_EQ_U(5, r.end); REG_EQ_U(2, r.length); REG_EQ_I(level, r.level);
        REG_EQ_I(0, run("aaa", "contains aa", level, &r)); REG_CHECK(r.matched); REG_EQ_U(0, r.start);   /* first hit */
        REG_EQ_I(0, run("hello", "contains xyz", level, &r)); REG_CHECK(!r.matched); REG_EQ_U(0, r.length);
        REG_EQ_I(0, run("hello", "contains Hello", level, &r)); REG_CHECK(!r.matched);                    /* case */
        REG_EQ_I(0, run("hello", "contains ", level, &r)); REG_CHECK(r.matched); REG_EQ_U(0, r.length);   /* empty lit */
        REG_EQ_I(0, run("", "contains a", level, &r)); REG_CHECK(!r.matched);
        REG_EQ_I(0, run("", "contains ", level, &r)); REG_CHECK(r.matched);

        REG_EQ_I(0, run("hello", "starts he", level, &r)); REG_CHECK(r.matched); REG_EQ_U(0, r.start); REG_EQ_U(2, r.end);
        REG_EQ_I(0, run("hello", "starts el", level, &r)); REG_CHECK(!r.matched);
        REG_EQ_I(0, run("he", "starts hello", level, &r)); REG_CHECK(!r.matched);                         /* longer */
        REG_EQ_I(0, run("hello", "starts hello", level, &r)); REG_CHECK(r.matched); REG_EQ_U(5, r.end);

        REG_EQ_I(0, run("hello", "ends llo", level, &r)); REG_CHECK(r.matched); REG_EQ_U(2, r.start); REG_EQ_U(5, r.end);
        REG_EQ_I(0, run("hello", "ends hel", level, &r)); REG_CHECK(!r.matched);
        REG_EQ_I(0, run("lo", "ends hello", level, &r)); REG_CHECK(!r.matched);                           /* longer */
        REG_EQ_I(0, run("hello", "ends ", level, &r)); REG_CHECK(r.matched); REG_EQ_U(5, r.start); REG_EQ_U(5, r.end);
        /* a space is an ordinary literal character after the keyword's own space */
        REG_EQ_I(0, run("a b c", "contains  b", level, &r)); REG_CHECK(r.matched); REG_EQ_U(1, r.start);
    }
}

static void t_exact_literal(void) {
    R r;
    REG_EQ_I(0, run("hello", "exact hello", 1, &r)); REG_CHECK(r.matched); REG_EQ_U(5, r.length);
    REG_EQ_I(0, run("hello!", "exact hello", 1, &r)); REG_CHECK(!r.matched);
    REG_EQ_I(0, run("", "exact ", 1, &r)); REG_CHECK(r.matched);
    REG_EQ_I(0, run("x", "exact ", 1, &r)); REG_CHECK(!r.matched);
    /* at SHAPE level a plain word is still a literal comparison */
    REG_EQ_I(0, run("hello", "exact hello", 2, &r)); REG_CHECK(r.matched);
    REG_EQ_I(0, run("hellO", "exact hello", 2, &r)); REG_CHECK(!r.matched);
    /* FIND level never interprets shapes: "digit+" is just text */
    REG_CHECK(!matched("123", "exact digit+", 1));
    REG_CHECK(matched("digit+", "exact digit+", 1));
}

static void t_shapes(void) {
    static const struct { const char *shape; const char *text; int want; } c[] = {
        {"digit+", "123", 1}, {"digit+", "", 0}, {"digit+", "12a", 0}, {"digit+", "a12", 0}, {"digit+", " 1", 0},
        {"digit*", "", 1}, {"digit*", "0", 1}, {"digit*", "x", 0},
        {"digit", "7", 1}, {"digit", "77", 0}, {"digit", "", 0},
        {"digit{3}", "123", 1}, {"digit{3}", "12", 0}, {"digit{3}", "1234", 0},
        {"digit{0}", "", 1}, {"digit{0}", "1", 0},
        {"digit{2,4}", "1", 0}, {"digit{2,4}", "12", 1}, {"digit{2,4}", "1234", 1}, {"digit{2,4}", "12345", 0},
        {"digit{3,3}", "123", 1}, {"digit{3,3}", "12", 0},
        {"digit{2,}", "1", 0}, {"digit{2,}", "12", 1}, {"digit{2,}", "1234567890123456789", 1},
        {"word+", "ab_9", 1}, {"word+", "a b", 0}, {"word+", "", 0}, {"word+", "a-b", 0}, {"word+", "_", 1},
        {"word{2}", "ab", 1}, {"word{2}", "abc", 0},
        {"space+", " \t\r\n\v\f", 1}, {"space+", "a", 0}, {"space+", "", 0}, {"space*", "", 1},
        {"any+", "x y\n!", 1}, {"any+", "", 0}, {"any*", "", 1}, {"any{2}", "ab", 1}, {"any{2}", "a", 0},
    };
    for (size_t i = 0; i < sizeof c / sizeof c[0]; ++i) {
        char pat[64]; R r; SleelaRegexStatus s;
        strcpy(pat, "exact "); strcat(pat, c[i].shape);
        s = run(c[i].text, pat, 2, &r);
        reg_note_check();
        if (s != SLEELA_REGEX_OK || r.matched != c[i].want)
            reg_fail(__FILE__, __LINE__, "shape %s on \"%s\": status=%d matched=%d want=%d",
                     c[i].shape, c[i].text, (int)s, r.matched, c[i].want);
        if (c[i].want && s == SLEELA_REGEX_OK) {
            REG_EQ_U(0, r.start); REG_EQ_U(strlen(c[i].text), r.end); REG_EQ_U(strlen(c[i].text), r.length);
        }
    }
    /* the matcher is locale-independent for high bytes (C locale classes) */
    REG_CHECK(!matched("\xC3\xA9", "exact word+", 2));
    REG_CHECK(!matched("\xFF", "exact digit", 2));
    REG_CHECK(matched("\xFF", "exact any", 2));
}

/* Malformed shapes are not errors: they report OK with no match. */
static void t_malformed_shapes_do_not_match(void) {
    static const char *bad[] = {"digit{}", "digit{,3}", "digit{4,2}", "digit{2", "digit{2,3", "digit{2,x}",
                                "digit{x}", "digit+x", "digit++", "foo+", "foo*", "foo{2}", "{3}", "+"};
    for (size_t i = 0; i < sizeof bad / sizeof bad[0]; ++i) {
        char pat[64]; R r; SleelaRegexStatus s;
        strcpy(pat, "exact "); strcat(pat, bad[i]);
        s = run("123", pat, 2, &r);
        reg_note_check();
        if (s != SLEELA_REGEX_OK || r.matched)
            reg_fail(__FILE__, __LINE__, "malformed shape %s: status=%d matched=%d", bad[i], (int)s, r.matched);
    }
}

/* CHARACTERIZATION of current behaviour at SHAPE level: a literal containing
 * one of + * { cannot be matched literally (the shape parse fails and the
 * quantifier characters short-circuit to "no match"). FIND level is exact. */
static void t_shape_level_literal_with_quantifier_chars(void) {
    REG_CHECK(matched("a+b", "exact a+b", 1));
    REG_CHECK(!matched("a+b", "exact a+b", 2));
    REG_CHECK(matched("digit5", "exact digit5", 2));   /* no quantifier char: literal */
    REG_CHECK(matched("digit", "exact digit", 2));     /* text 'digit' is not 1 digit char; literal fallback */
}

/* Regression (was REGEX-QUANT-OVERFLOW): quantifier counts are accumulated with
 * an overflow check, so a count that does not fit size_t is an invalid shape
 * (no match) and never wraps around to a small number. */
static void t_quantifier_overflow_is_not_wrapped(void) {
    static const char *huge[] = {
        "exact digit{18446744073709551617}",        /* 2^64 + 1 would wrap to {1} */
        "exact digit{18446744073709551616}",        /* 2^64 would wrap to {0}      */
        "exact digit{99999999999999999999}",
        "exact digit{1,18446744073709551617}",
        "exact digit{18446744073709551617,}",
        "exact digit{0,36893488147419103232}",
    };
    for (size_t i = 0; i < sizeof huge / sizeof huge[0]; ++i) {
        R r; SleelaRegexStatus s;
        s = run("5", huge[i], 2, &r);
        REG_EQ_I(SLEELA_REGEX_OK, s); REG_CHECK(!r.matched);
        s = run("", huge[i], 2, &r);
        REG_EQ_I(SLEELA_REGEX_OK, s); REG_CHECK(!r.matched);
    }
    /* the largest representable counts still parse (they just cannot match short text) */
    REG_CHECK(!matched("5", "exact digit{18446744073709551615}", 2));
    REG_CHECK(matched("5", "exact digit{1,18446744073709551615}", 2));
    REG_CHECK(matched("55", "exact digit{2,18446744073709551615}", 2));
    REG_CHECK(matched("", "exact digit{0,18446744073709551615}", 2));
    REG_CHECK(matched("5", "exact digit{1}", 2));
}

/* Differential: find-family against libc strstr/strncmp over random text. */
static void t_differential_find_family(void) {
    static const char alpha[] = "ab1 _";
    reg_seed(0xFEEDF00Dull);
    for (int it = 0; it < 20000; ++it) {
        char t[10], lit[5], pat[16]; size_t tn = reg_below(10), ln = reg_below(5);
        R r; const char *kind[] = {"contains ", "starts ", "ends "}; unsigned k = reg_below(3);
        int want; size_t wstart = 0;
        for (size_t i = 0; i < tn; ++i) t[i] = alpha[reg_below(5)];
        for (size_t i = 0; i < ln; ++i) lit[i] = alpha[reg_below(5)];
        t[tn] = 0; lit[ln] = 0;
        strcpy(pat, kind[k]); strcat(pat, lit);
        if (k == 0) { const char *p = strstr(t, lit); want = p != NULL; if (p) wstart = (size_t)(p - t); }
        else if (k == 1) { want = strncmp(t, lit, ln) == 0 && ln <= tn; wstart = 0; }
        else { want = ln <= tn && strcmp(t + tn - ln, lit) == 0; wstart = tn - ln; }
        REG_EQ_I(0, run(t, pat, 1 + (int)reg_below(2), &r));
        REG_EQ_I(want, r.matched);
        if (want) { REG_EQ_U(wstart, r.start); REG_EQ_U(wstart + ln, r.end); REG_EQ_U(ln, r.length); }
    }
}

/* Differential: shapes against an independent counting oracle. */
static void t_differential_shapes(void) {
    static const char alpha[] = "0 9a_-\n";
    static const char *cls[] = {"digit", "word", "space", "any"};
    reg_seed(0xBADC0FFEull);
    for (int it = 0; it < 30000; ++it) {
        char t[9], pat[96]; size_t tn = reg_below(9); R r; unsigned c = reg_below(4), q = reg_below(7);
        size_t lo = 1, hi = 1, cnt = 0; int all = 1, want;
        char suffix[64] = "";
        for (size_t i = 0; i < tn; ++i) t[i] = alpha[reg_below(7)];
        t[tn] = 0;
        switch (q) {
            case 0: break;
            case 1: strcpy(suffix, "+"); lo = 1; hi = (size_t)-1; break;
            case 2: strcpy(suffix, "*"); lo = 0; hi = (size_t)-1; break;
            case 3: lo = hi = reg_below(5); snprintf(suffix, sizeof suffix, "{%zu}", lo); break;
            case 4: lo = reg_below(4); hi = lo + reg_below(3); snprintf(suffix, sizeof suffix, "{%zu,%zu}", lo, hi); break;
            case 5: lo = reg_below(4); hi = (size_t)-1; snprintf(suffix, sizeof suffix, "{%zu,}", lo); break;
            default: strcpy(suffix, "+"); lo = 1; hi = (size_t)-1; break;
        }
        for (size_t i = 0; i < tn; ++i) {
            unsigned char ch = (unsigned char)t[i]; int ok;
            switch (c) {
                case 0: ok = ch >= '0' && ch <= '9'; break;
                case 1: ok = (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'; break;
                case 2: ok = ch == ' ' || (ch >= 9 && ch <= 13); break;
                default: ok = 1; break;
            }
            if (!ok) all = 0;
            cnt++;
        }
        want = all && cnt >= lo && cnt <= hi;
        snprintf(pat, sizeof pat, "exact %s%s", cls[c], suffix);
        REG_EQ_I(0, run(t, pat, 2, &r));
        reg_note_check();
        if (r.matched != want) {
            reg_fail(__FILE__, __LINE__, "pattern \"%s\" text \"%s\": matched=%d want=%d", pat, t, r.matched, want);
            return;
        }
    }
}

/* Scale: linear-time behaviour on a 1 MiB input (bounded runtime). */
static void t_large_text(void) {
    const size_t n = 1u << 20; char *t = (char *)malloc(n + 2); R r;
    REG_REQUIRE(t != NULL);
    memset(t, 'a', n); t[n] = 'b'; t[n + 1] = 0;
    REG_EQ_I(0, run(t, "contains ab", 1, &r)); REG_CHECK(r.matched); REG_EQ_U(n - 1, r.start);
    REG_EQ_I(0, run(t, "ends ab", 1, &r)); REG_CHECK(r.matched); REG_EQ_U(n - 1, r.start);
    REG_EQ_I(0, run(t, "exact word+", 2, &r)); REG_CHECK(r.matched); REG_EQ_U(n + 1, r.end);
    t[n / 2] = '-';
    REG_EQ_I(0, run(t, "exact word+", 2, &r)); REG_CHECK(!r.matched);
    free(t);
}

int main(void) {
    REG_RUN(t_status_names);
    REG_RUN(t_argument_and_level_validation);
    REG_RUN(t_pattern_prefix_grammar);
    REG_RUN(t_find_family_positions);
    REG_RUN(t_exact_literal);
    REG_RUN(t_shapes);
    REG_RUN(t_malformed_shapes_do_not_match);
    REG_RUN(t_shape_level_literal_with_quantifier_chars);
    REG_RUN(t_quantifier_overflow_is_not_wrapped);
    REG_RUN(t_differential_find_family);
    REG_RUN(t_differential_shapes);
    REG_RUN(t_large_text);
    return reg_finish("regex");
}
