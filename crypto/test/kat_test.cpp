/*
 * crypto/test/kat_test.cpp
 * Known-Answer Tests (KATs) against published FIPS/NIST/RFC test vectors.
 * Verifies the reference implementations produce standards-correct output.
 */
#include "../cpp/slcrypto.hpp"
#include <cstdio>
#include <cstring>
#include <string>

using slcrypto::Bytes;
using slcrypto::to_hex;

static int failures = 0;
static int checks = 0;

static Bytes from_hex(const std::string& h) {
    Bytes b;
    for (size_t i = 0; i + 1 < h.size(); i += 2)
        b.push_back((uint8_t)std::stoi(h.substr(i, 2), nullptr, 16));
    return b;
}
static Bytes str(const char* s) { return Bytes(s, s + std::strlen(s)); }

static void check(const char* name, const std::string& got, const std::string& want) {
    checks++;
    if (got == want) { std::printf("  PASS  %s\n", name); }
    else { failures++; std::printf("  FAIL  %s\n        got : %s\n        want: %s\n",
                                    name, got.c_str(), want.c_str()); }
}

int main() {
    std::printf("SLeeLa crypto Known-Answer Tests\n");

    /* ---- SHA-256 (FIPS 180-4 / NIST) ---- */
    check("SHA-256(\"abc\")", to_hex(slcrypto::sha256(str("abc"))),
          "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    check("SHA-256(\"\")", to_hex(slcrypto::sha256(str(""))),
          "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

    /* ---- SHA-512 (FIPS 180-4) ---- */
    check("SHA-512(\"abc\")", to_hex(slcrypto::sha512(str("abc"))),
          "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
          "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f");

    /* ---- SHA-384 (FIPS 180-4) ---- */
    check("SHA-384(\"abc\")", to_hex(slcrypto::sha384(str("abc"))),
          "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed"
          "8086072ba1e7cc2358baeca134c825a7");

    /* ---- SHA3-256 (FIPS 202) ---- */
    check("SHA3-256(\"\")", to_hex(slcrypto::sha3_256(str(""))),
          "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a");
    check("SHA3-256(\"abc\")", to_hex(slcrypto::sha3_256(str("abc"))),
          "3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431532");

    /* ---- SHA3-512 (FIPS 202) ---- */
    check("SHA3-512(\"abc\")", to_hex(slcrypto::sha3_512(str("abc"))),
          "b751850b1a57168a5693cd924b6b096e08f621827444f70d884f5d0240d2712e"
          "10e116e9192af3c91a7ec57647e3934057340b4cf408d5a56592f8274eec53f0");

    /* ---- HMAC-SHA-256 (RFC 4231 Test Case 2) ---- */
    check("HMAC-SHA-256(RFC4231 TC2)",
          to_hex(slcrypto::hmac_sha256(str("Jefe"), str("what do ya want for nothing?"))),
          "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843");

    /* ---- AES-128 single block (FIPS 197 Appendix B / C.1) ---- */
    {
        Bytes key = from_hex("000102030405060708090a0b0c0d0e0f");
        Bytes pt  = from_hex("00112233445566778899aabbccddeeff");
        slc_aes_key k; slc_aes_set_encrypt_key(&k, key.data(), 128);
        uint8_t ct[16]; slc_aes_encrypt_block(&k, pt.data(), ct);
        check("AES-128 encrypt block (FIPS 197 C.1)",
              to_hex(Bytes(ct, ct + 16)), "69c4e0d86a7b0430d8cdb78070b4c55a");
        uint8_t back[16];
        slc_aes_key dk; slc_aes_set_decrypt_key(&dk, key.data(), 128);
        slc_aes_decrypt_block(&dk, ct, back);
        check("AES-128 decrypt round-trip", to_hex(Bytes(back, back + 16)), to_hex(pt));
    }

    /* ---- AES-256 single block (FIPS 197 C.3) ---- */
    {
        Bytes key = from_hex("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
        Bytes pt  = from_hex("00112233445566778899aabbccddeeff");
        slc_aes_key k; slc_aes_set_encrypt_key(&k, key.data(), 256);
        uint8_t ct[16]; slc_aes_encrypt_block(&k, pt.data(), ct);
        check("AES-256 encrypt block (FIPS 197 C.3)",
              to_hex(Bytes(ct, ct + 16)), "8ea2b7ca516745bfeafc49904b496089");
    }

    /* ---- AES-128-GCM (SP 800-38D test vector; empty PT/AAD) ---- */
    {
        Bytes key = from_hex("00000000000000000000000000000000");
        Bytes iv  = from_hex("000000000000000000000000");
        Bytes aad, pt;
        auto r = slcrypto::aes_gcm_encrypt(key, iv, aad, pt, 16);
        check("AES-128-GCM tag (empty, SP800-38D)",
              to_hex(r.tag), "58e2fccefa7e3061367f1d57a4e7455a");
    }

    /* ---- AES-128-GCM (SP 800-38D Case 3: 64-byte PT, 96-bit IV) ---- */
    {
        Bytes key = from_hex("feffe9928665731c6d6a8f9467308308");
        Bytes iv  = from_hex("cafebabefacedbaddecaf888");
        Bytes aad;
        Bytes pt  = from_hex(
            "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39"
            "1aafd255");
        auto r = slcrypto::aes_gcm_encrypt(key, iv, aad, pt, 16);
        check("AES-128-GCM ct (SP800-38D Case 3)", to_hex(r.ciphertext),
              "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091"
              "473f5985");
        check("AES-128-GCM tag (SP800-38D Case 3)", to_hex(r.tag),
              "4d5c2af327cd64a62cf35abd2ba6fab4");
        /* decrypt round-trip + tag verification */
        auto back = slcrypto::aes_gcm_decrypt(key, iv, aad, r.ciphertext, r.tag);
        check("AES-128-GCM decrypt round-trip", to_hex(back), to_hex(pt));
    }

    std::printf("\n%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
