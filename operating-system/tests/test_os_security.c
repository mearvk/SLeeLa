/* operating-system/tests/test_os_security.c -- SHA-256, verify-before-execute, driver admit. */
#include "sleela_os_security.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    char hex[SLEELA_OS_SHA256_HEX];
    uint8_t digest[SLEELA_OS_SHA256_BYTES];

    /* FIPS 180-4 known-answer vectors. */
    const char *empty_hex = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
    const char *abc_hex   = "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";

    sleela_os_sha256("", 0, digest);
    sleela_os_sha256_hex(digest, hex);
    assert(strcmp(hex, empty_hex) == 0);

    sleela_os_sha256("abc", 3, digest);
    sleela_os_sha256_hex(digest, hex);
    assert(strcmp(hex, abc_hex) == 0);

    /* verify-before-execute gate */
    assert(sleela_os_verify_buffer("abc", 3, abc_hex) == 1);
    assert(sleela_os_verify_buffer("abc", 3, empty_hex) == 0);
    /* case-insensitive expected digest */
    assert(sleela_os_verify_buffer("abc", 3,
        "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD") == 1);
    assert(sleela_os_verify_buffer("abc", 3, 0) == -1);

    /* a larger buffer crossing block boundaries (> 64 bytes) */
    {
        char big[200];
        memset(big, 'x', sizeof big);
        sleela_os_sha256(big, sizeof big, digest);
        sleela_os_sha256_hex(digest, hex);
        /* round-trips through its own verifier */
        assert(sleela_os_verify_buffer(big, sizeof big, hex) == 1);
    }

    /* file digest matches buffer digest */
    {
        FILE *f = fopen("os_security_tmp.bin", "wb");
        assert(f != NULL);
        fwrite("abc", 1, 3, f);
        fclose(f);
        assert(sleela_os_sha256_file("os_security_tmp.bin", hex) == 0);
        assert(strcmp(hex, abc_hex) == 0);
        assert(sleela_os_verify_file("os_security_tmp.bin", abc_hex) == 1);
        remove("os_security_tmp.bin");
        assert(sleela_os_sha256_file("os_security_tmp.bin", hex) == -1); /* gone */
    }

    /* driver-admission review (re-exposed policy) */
    sleela_driver_policy(1, 1); /* permissive: unknown taints, foreign bridges */
    assert(sleela_os_admit_is_load(sleela_os_review_driver(SL_ORIGIN_INTREE, 0, 0)) == 1);
    assert(sleela_os_review_driver(SL_ORIGIN_UNKNOWN, 0, 0) == SL_ADMIT_LOAD_TAINT);
    sleela_driver_policy(0, 0); /* strict: refuse unknown + foreign */
    assert(sleela_os_review_driver(SL_ORIGIN_UNKNOWN, 0, 0) == SL_ADMIT_REFUSED);
    assert(sleela_os_admit_is_load(SL_ADMIT_REFUSED) == 0);

    printf("test_os_security: ok\n");
    return 0;
}
