/*
 * native/src/sleela_crypto_cli.cpp
 * SLeeLa Native Cryptography Bridge — command adapter and self-test.
 *
 * Exercises the C ABI the SLeeLa /lib/crypto classes bind to: radix round-trips
 * across the 1..2055 span, the ordered block advance, both intermix stages, the
 * comparison gate, and the national-register call.
 *
 * Author: Max Rupplin — MEARVK LLC — 2026
 */
#include "sleela_crypto.h"

#include <cstdio>
#include <cstring>

static int failures = 0;

static void check(const char *name, bool cond) {
    std::printf("[%s] %s\n", cond ? "pass" : "FAIL", name);
    if (!cond) ++failures;
}

int main(int argc, char **argv) {
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
        std::printf("usage: sleela-crypto-native [--help]\n");
        std::printf("  runs the SLeeLa crypto bridge self-test (radix 1..2055, intermix, compare, register)\n");
        return 0;
    }

    char buf[256];

    /* Radix round-trips at representative bases including the 1 and 2055 bounds. */
    int bases[] = {1, 2, 6, 11, 12, 13, 17, 18, 36, 37, 100, 1000, 2055};
    for (int b : bases) {
        /* Base 1 is unary: use a small magnitude so the tally fits the buffer.
         * Positional bases exercise a larger value. */
        int64_t sample = (b == 1) ? 42 : 123456;
        int n = sleela_crypto_radix_to_base(b, sample, buf, sizeof buf);
        bool enc_ok = n >= 0;
        int ok = 0;
        int64_t v = sleela_crypto_radix_from_base(b, buf, &ok);
        char name[64];
        std::snprintf(name, sizeof name, "radix %d round-trip", b);
        check(name, enc_ok && ok == 1 && v == sample);
    }

    /* Out-of-range bases are rejected. */
    check("radix 0 rejected", sleela_crypto_radix_to_base(0, 10, buf, sizeof buf) == -1);
    check("radix 2056 rejected", sleela_crypto_radix_to_base(2056, 10, buf, sizeof buf) == -1);

    /* Ordered block advance (reference pass-one shape: base 12, mask 0x88034321). */
    int n = sleela_crypto_block_advance(1, 12, 0x88034321, "100", buf, sizeof buf);
    check("block advance order 1", n > 0);
    check("block advance order 0 rejected",
          sleela_crypto_block_advance(0, 12, 0, "1", buf, sizeof buf) == -1);
    check("block advance order 256 rejected",
          sleela_crypto_block_advance(256, 12, 0, "1", buf, sizeof buf) == -1);

    /* Two-row symmetry intermix. */
    n = sleela_crypto_block_two_rows("85", 18, 0x166F2, 13, 0x0134431, 6, 0x45344321,
                                     buf, sizeof buf);
    check("two-rows intermix", n > 0);

    /* Primary and secondary intermix. */
    n = sleela_crypto_intermix_primary("42", 16,
                                       7, 11, 0x7716,
                                       2, 11, 0x77223,
                                       6, 11, 0x7766,
                                       1, 12, 0x771c,
                                       buf, sizeof buf);
    check("primary intermix", n > 0);

    n = sleela_crypto_intermix_secondary("42", 19,
                                         17, 17, 0x771321a,
                                         2,  11, 0x7722321,
                                         3,  17, 0x77321a,
                                         buf, sizeof buf);
    check("secondary intermix", n > 0);

    /* Comparison gate. */
    check("compare equal", sleela_crypto_compare("abc", "abc") == 1);
    check("compare unequal", sleela_crypto_compare("abc", "abd") == 0);
    check("compare length", sleela_crypto_compare("abc", "abcd") == 0);

    /* National register. */
    check("register accepts non-empty",
          sleela_crypto_national_register("US-NATIONAL-CRYPTO-REGISTER", "result") == 1);
    check("register refuses empty",
          sleela_crypto_national_register("US-NATIONAL-CRYPTO-REGISTER", "") == 0);

    std::printf("%s (%d failure(s))\n", failures == 0 ? "ALL PASS" : "FAILURES", failures);
    return failures == 0 ? 0 : 1;
}
