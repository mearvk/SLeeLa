/*
 * reg_path.c -- regression suite for impl/core/sleela_path.c  (POSIX build)
 *
 * Contract (impl/core/sleela_path.h): platform-neutral lexical path helpers
 * (normalize / join / absolute) and thin filesystem predicates.  Every
 * function returns 0 on success and -1 on bad arguments or a buffer that is
 * too small; it never writes past out_size.
 *
 * Security-relevant behaviour pinned here:
 *   - join() strips leading separators from the right operand, so an absolute
 *     right operand cannot replace the left one;
 *   - normalize()/join() are purely lexical and do NOT resolve "." or "..";
 *     callers that need containment must resolve and compare themselves.
 */
#if !defined(_WIN32) && !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 700
#endif
#if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
#define _DARWIN_C_SOURCE 1
#endif
#include "../../../impl/core/sleela_path.h"
#include "regression.h"

#ifdef _WIN32
int main(void) { puts("SUITE path: skipped (POSIX-only suite)"); return 0; }
#else
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char g_root[1024];

static void touch(const char *path, const char *content) {
    FILE *f = fopen(path, "wb");
    if (f) { fputs(content, f); fclose(f); }
}

static void t_platform_identity(void) {
#if defined(__APPLE__)
    REG_EQ_I(SL_PATH_MACOS, slpath_platform()); REG_EQ_S("macos", slpath_platform_name());
#else
    REG_EQ_I(SL_PATH_LINUX, slpath_platform()); REG_EQ_S("linux", slpath_platform_name());
#endif
    REG_EQ_I('/', slpath_separator());
    REG_EQ_I(1, slpath_platform_is_available(SL_PATH_AUTO));
    REG_EQ_I(1, slpath_platform_is_available(slpath_platform()));
    REG_EQ_I(0, slpath_platform_is_available(SL_PATH_WINDOWS));
}

static void t_normalize_golden(void) {
    static const struct { const char *in, *out; } g[] = {
        {"a", "a"}, {"a/b", "a/b"}, {"a//b", "a/b"}, {"a///b////c", "a/b/c"},
        {"a\\b", "a/b"}, {"a\\\\b", "a/b"}, {"a/\\b", "a/b"}, {"a\\/\\b", "a/b"},
        {"a/", "a"}, {"a//", "a"}, {"a\\", "a"}, {"/", "/"}, {"//", "/"}, {"///", "/"},
        {"\\", "/"}, {"/a", "/a"}, {"//a", "/a"}, {"/a/", "/a"},
        {".", "."}, {"./a", "./a"},
        /* lexical only: dot segments are preserved, never resolved */
        {"a/./b", "a/./b"}, {"a/../b", "a/../b"}, {"../a", "../a"}, {"/..", "/.."},
        {"C:\\dir\\file", "C:/dir/file"},
    };
    char out[64];
    for (size_t i = 0; i < sizeof g / sizeof g[0]; ++i) {
        reg_note_check();
        if (slpath_normalize(out, sizeof out, g[i].in) != 0 || strcmp(out, g[i].out) != 0)
            reg_fail(__FILE__, __LINE__, "normalize(\"%s\") = \"%s\", want \"%s\"", g[i].in, out, g[i].out);
    }
    /* in-place use is supported */
    strcpy(out, "x//y\\z/");
    REG_EQ_I(0, slpath_normalize(out, sizeof out, out)); REG_EQ_S("x/y/z", out);
}

static void t_normalize_arguments_and_bounds(void) {
    char out[16]; unsigned char canary[8];
    REG_EQ_I(-1, slpath_normalize(NULL, 16, "a"));
    REG_EQ_I(-1, slpath_normalize(out, 0, "a"));
    REG_EQ_I(-1, slpath_normalize(out, 16, NULL));
    REG_EQ_I(-1, slpath_normalize(out, 16, ""));
    /* exact fit: 7 chars + NUL */
    REG_EQ_I(0, slpath_normalize(out, 8, "abc/def")); REG_EQ_S("abc/def", out);
    REG_EQ_I(-1, slpath_normalize(out, 7, "abc/def"));
    REG_EQ_I(-1, slpath_normalize(out, 1, "a"));
    REG_EQ_I(0, slpath_normalize(out, 2, "a")); REG_EQ_S("a", out);
    /* never writes beyond out_size */
    {
        struct { char buf[4]; unsigned char guard[8]; } s;
        memset(&s, 0x5A, sizeof s);
        REG_EQ_I(-1, slpath_normalize(s.buf, sizeof s.buf, "abcdefghij"));
        memset(canary, 0x5A, sizeof canary);
        REG_EQ_MEM(canary, s.guard, sizeof canary);
    }
}

static void t_join_golden(void) {
    static const struct { const char *l, *r, *out; } g[] = {
        {"a", "b", "a/b"}, {"a/", "b", "a/b"}, {"a\\", "b", "a/b"}, {"a", "/b", "a/b"},
        {"a", "\\\\b", "a/b"}, {"a//", "//b", "a/b"}, {"/", "b", "/b"}, {"/a", "b/c", "/a/b/c"},
        {"a/b", "c/d", "a/b/c/d"}, {"a", "", "a"}, {"a", "///", "a"},
        /* the right operand can never replace the left one */
        {"/base", "/etc/passwd", "/base/etc/passwd"},
        {"/base", "\\windows\\x", "/base/windows/x"},
        /* ... but ".." is NOT resolved: containment is the caller's job */
        {"/base", "../../etc", "/base/../../etc"},
    };
    char out[64];
    for (size_t i = 0; i < sizeof g / sizeof g[0]; ++i) {
        reg_note_check();
        if (slpath_join(out, sizeof out, g[i].l, g[i].r) != 0 || strcmp(out, g[i].out) != 0)
            reg_fail(__FILE__, __LINE__, "join(\"%s\",\"%s\") = \"%s\", want \"%s\"", g[i].l, g[i].r, out, g[i].out);
    }
}

static void t_join_arguments_and_bounds(void) {
    char out[16];
    REG_EQ_I(-1, slpath_join(NULL, 16, "a", "b"));
    REG_EQ_I(-1, slpath_join(out, 0, "a", "b"));
    REG_EQ_I(-1, slpath_join(out, 16, NULL, "b"));
    REG_EQ_I(-1, slpath_join(out, 16, "a", NULL));
    REG_EQ_I(-1, slpath_join(out, 16, "", "b"));          /* empty left is invalid */
    REG_EQ_I(0, slpath_join(out, 8, "abc", "def")); REG_EQ_S("abc/def", out);   /* exact fit */
    REG_EQ_I(-1, slpath_join(out, 7, "abc", "def"));                           /* one short */
    REG_EQ_I(-1, slpath_join(out, 3, "abc", "def"));
    {   /* no overflow past the buffer on truncation */
        struct { char buf[6]; unsigned char guard[8]; } s; unsigned char canary[8];
        memset(&s, 0x5A, sizeof s); memset(canary, 0x5A, sizeof canary);
        REG_EQ_I(-1, slpath_join(s.buf, sizeof s.buf, "abcdefgh", "ijklmnop"));
        REG_EQ_MEM(canary, s.guard, sizeof canary);
    }
    {   /* long components */
        char big[5000], left[2100], right[2100], expect[4300];
        memset(left, 'l', sizeof left - 1); left[sizeof left - 1] = 0;
        memset(right, 'r', sizeof right - 1); right[sizeof right - 1] = 0;
        REG_EQ_I(0, slpath_join(big, sizeof big, left, right));
        snprintf(expect, sizeof expect, "%s/%s", left, right);
        REG_EQ_S(expect, big);
        /* 2099 + '/' + 2099 = 4199 chars: 4200 is the exact fit, 4199 is one short */
        REG_EQ_I(0, slpath_join(big, 4200, left, right));
        REG_EQ_I(-1, slpath_join(big, 4199, left, right));
    }
}

/* Property: normalize against an independent reference over random inputs. */
static void ref_normalize(const char *in, char *out) {
    size_t j = 0; int prev = 0;
    for (size_t i = 0; in[i]; ++i) {
        int sep = in[i] == '/' || in[i] == '\\';
        if (sep) { if (prev) continue; out[j++] = '/'; prev = 1; }
        else { out[j++] = in[i]; prev = 0; }
    }
    while (j > 1 && out[j - 1] == '/') --j;
    out[j] = 0;
}

static void t_normalize_properties(void) {
    static const char alpha[] = "a/\\.b";
    reg_seed(0x9A7Full);
    for (int it = 0; it < 20000; ++it) {
        char in[16], got[64], want[64], again[64]; size_t n = 1 + reg_below(14);
        for (size_t i = 0; i < n; ++i) in[i] = alpha[reg_below(5)];
        in[n] = 0;
        REG_REQUIRE(slpath_normalize(got, sizeof got, in) == 0);
        ref_normalize(in, want);
        REG_EQ_S(want, got);
        REG_REQUIRE(slpath_normalize(again, sizeof again, got) == 0);
        REG_EQ_S(got, again);                         /* idempotent */
        REG_CHECK(strlen(got) <= strlen(in));         /* never grows */
        REG_CHECK(strchr(got, '\\') == NULL && strstr(got, "//") == NULL);
        REG_CHECK(strlen(got) == 1 || got[strlen(got) - 1] != '/');
    }
}

static void t_join_properties(void) {
    static const char alpha[] = "ab/\\.";
    reg_seed(0x10111ull);
    for (int it = 0; it < 20000; ++it) {
        char l[10], r[10], out[64]; size_t ln = 1 + reg_below(8), rn = reg_below(8);
        size_t rs = 0; char norm_l[32];
        for (size_t i = 0; i < ln; ++i) l[i] = alpha[reg_below(5)];
        for (size_t i = 0; i < rn; ++i) r[i] = alpha[reg_below(5)];
        l[ln] = 0; r[rn] = 0;
        REG_REQUIRE(slpath_join(out, sizeof out, l, r) == 0);
        REG_CHECK(strchr(out, '\\') == NULL && strstr(out, "//") == NULL);
        ref_normalize(l, norm_l);
        /* the left operand is always preserved as a prefix of the result */
        REG_CHECK(strncmp(out, norm_l, strlen(norm_l) > 1 ? strlen(norm_l) : 1) == 0 ||
                  strcmp(norm_l, "/") == 0);
        while (r[rs] == '/' || r[rs] == '\\') ++rs;
        if (r[rs] == 0) { REG_EQ_S(norm_l, out); }   /* empty/separator-only right */
    }
}

static void t_absolute(void) {
    char cwd[PATH_MAX - 64], out[PATH_MAX], want[PATH_MAX], sub[PATH_MAX], q[PATH_MAX];
    REG_REQUIRE(getcwd(cwd, sizeof cwd) != NULL);
    REG_EQ_I(0, slpath_current_directory(out, sizeof out)); REG_EQ_S(cwd, out);
    REG_EQ_I(-1, slpath_current_directory(out, 1));           /* too small */

    /* existing: symlinks and dot segments are resolved by realpath */
    snprintf(sub, sizeof sub, "%s/abs_sub", g_root);
    REG_EQ_I(0, slpath_make_directory(sub));
    snprintf(q, sizeof q, "%s/abs_sub/../abs_sub/.", g_root);
    REG_EQ_I(0, slpath_absolute(out, sizeof out, q));
    REG_REQUIRE(realpath(sub, want) != NULL);
    REG_EQ_S(want, out);

    /* non-existent absolute: lexical normalisation, no filesystem access */
    REG_EQ_I(0, slpath_absolute(out, sizeof out, "/no/such//dir/x/")); REG_EQ_S("/no/such/dir/x", out);
    /* non-existent relative: anchored at the current directory */
    REG_EQ_I(0, slpath_absolute(out, sizeof out, "definitely_missing_zz/child"));
    snprintf(want, sizeof want, "%s/definitely_missing_zz/child", cwd);
    REG_EQ_S(want, out);

    REG_EQ_I(-1, slpath_absolute(NULL, sizeof out, "a"));
    REG_EQ_I(-1, slpath_absolute(out, 0, "a"));
    REG_EQ_I(-1, slpath_absolute(out, sizeof out, NULL));
    REG_EQ_I(-1, slpath_absolute(out, 4, "/no/such/dir"));  /* too small */
}

static void t_filesystem_predicates_and_mutations(void) {
    char d[PATH_MAX], f[PATH_MAX], f2[PATH_MAX], deep[PATH_MAX], link[PATH_MAX], dangling[PATH_MAX];
    snprintf(d, sizeof d, "%s/fs_dir", g_root);
    snprintf(f, sizeof f, "%s/fs_dir/file.txt", g_root);
    snprintf(f2, sizeof f2, "%s/fs_dir/renamed.txt", g_root);
    snprintf(deep, sizeof deep, "%s/fs_missing_parent/child", g_root);
    snprintf(link, sizeof link, "%s/fs_dir/link", g_root);
    snprintf(dangling, sizeof dangling, "%s/fs_dir/dangling", g_root);

    REG_EQ_I(0, slpath_exists(d)); REG_EQ_I(0, slpath_is_directory(d));
    REG_EQ_I(0, slpath_make_directory(d));
    REG_EQ_I(0, slpath_make_directory(d));                 /* idempotent on an existing directory */
    REG_EQ_I(1, slpath_exists(d)); REG_EQ_I(1, slpath_is_directory(d)); REG_EQ_I(0, slpath_is_file(d));

    REG_EQ_I(-1, slpath_make_directory(deep));             /* parents are not created */
    REG_EQ_I(0, slpath_exists(deep));
    REG_EQ_I(-1, slpath_make_directory(""));
    REG_EQ_I(-1, slpath_make_directory(NULL));

    touch(f, "x");
    REG_EQ_I(1, slpath_is_file(f)); REG_EQ_I(0, slpath_is_directory(f)); REG_EQ_I(1, slpath_exists(f));
    REG_EQ_I(-1, slpath_make_directory(f));                /* a file blocks directory creation */

    REG_EQ_I(0, symlink(f, link));
    REG_EQ_I(1, slpath_is_file(link));                     /* predicates follow symlinks */
    REG_EQ_I(0, symlink("fs_no_target", dangling));
    REG_EQ_I(0, slpath_exists(dangling));
    REG_EQ_I(0, slpath_is_file(dangling));
    REG_EQ_I(0, slpath_remove(dangling)); REG_EQ_I(0, slpath_remove(link));

    REG_EQ_I(-1, slpath_remove(d));                        /* non-empty directory */
    REG_EQ_I(0, slpath_rename(f, f2));
    REG_EQ_I(0, slpath_exists(f)); REG_EQ_I(1, slpath_is_file(f2));
    REG_EQ_I(-1, slpath_rename(f, f2));                    /* source is gone */
    REG_EQ_I(0, slpath_remove(f2));
    REG_EQ_I(-1, slpath_remove(f2));                       /* already removed */
    REG_EQ_I(0, slpath_remove(d));                         /* empty directory */
    REG_EQ_I(0, slpath_exists(d));

    REG_EQ_I(0, slpath_exists(NULL)); REG_EQ_I(0, slpath_is_file(NULL)); REG_EQ_I(0, slpath_is_directory(NULL));
    REG_EQ_I(-1, slpath_remove(NULL)); REG_EQ_I(-1, slpath_rename(NULL, f)); REG_EQ_I(-1, slpath_rename(f, NULL));
}

int main(void) {
    const char *base = getenv("REG_TMPDIR");
    char tmpl[1024];
    snprintf(tmpl, sizeof tmpl, "%s/sleela_path_XXXXXX", base && *base ? base : ".");
    if (!mkdtemp(tmpl)) { perror("mkdtemp"); return 2; }
    snprintf(g_root, sizeof g_root, "%s", tmpl);

    REG_RUN(t_platform_identity);
    REG_RUN(t_normalize_golden);
    REG_RUN(t_normalize_arguments_and_bounds);
    REG_RUN(t_join_golden);
    REG_RUN(t_join_arguments_and_bounds);
    REG_RUN(t_normalize_properties);
    REG_RUN(t_join_properties);
    REG_RUN(t_absolute);
    REG_RUN(t_filesystem_predicates_and_mutations);

    {   char sub[PATH_MAX]; snprintf(sub, sizeof sub, "%s/abs_sub", g_root); (void)rmdir(sub); (void)rmdir(g_root); }
    return reg_finish("path");
}
#endif
