// digest.cpp - SHA-256 helpers (see digest.h).
#include "digest.h"

#include <array>
#include <cstdio>

#include <openssl/evp.h>

namespace advantage {

static std::string to_hex(const unsigned char *d, unsigned int n) {
    static const char *hx = "0123456789abcdef";
    std::string s;
    s.reserve(n * 2);
    for (unsigned int i = 0; i < n; ++i) {
        s.push_back(hx[d[i] >> 4]);
        s.push_back(hx[d[i] & 0x0f]);
    }
    return s;
}

std::string sha256_hex(const uint8_t *data, size_t len) {
    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int mdlen = 0;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) return "";
    std::string out;
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) == 1 &&
        EVP_DigestUpdate(ctx, data, len) == 1 &&
        EVP_DigestFinal_ex(ctx, md, &mdlen) == 1) {
        out = to_hex(md, mdlen);
    }
    EVP_MD_CTX_free(ctx);
    return out;
}

std::string sha256_file(const std::string &path) {
    FILE *f = std::fopen(path.c_str(), "rb");
    if (!f) return "";
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) { std::fclose(f); return ""; }
    std::string out;
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) == 1) {
        std::array<unsigned char, 65536> buf;
        size_t n;
        bool ok = true;
        while ((n = std::fread(buf.data(), 1, buf.size(), f)) > 0) {
            if (EVP_DigestUpdate(ctx, buf.data(), n) != 1) { ok = false; break; }
        }
        if (ok) {
            unsigned char md[EVP_MAX_MD_SIZE];
            unsigned int mdlen = 0;
            if (EVP_DigestFinal_ex(ctx, md, &mdlen) == 1) out = to_hex(md, mdlen);
        }
    }
    EVP_MD_CTX_free(ctx);
    std::fclose(f);
    return out;
}

}  // namespace advantage
