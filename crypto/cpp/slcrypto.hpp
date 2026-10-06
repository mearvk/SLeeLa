/*
 * crypto/cpp/slcrypto.hpp
 * C++ façade over the SLeeLa national-grade crypto C API (crypto/include/slcrypto.h).
 *
 * Idiomatic, header-only std::vector/std::array wrappers. Same SECURITY NOTE
 * applies: reference implementations, not hardened for production use.
 */
#ifndef SLCRYPTO_HPP
#define SLCRYPTO_HPP

#include "slcrypto.h"
#include <array>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace slcrypto {

using Bytes = std::vector<uint8_t>;

/* ---- Hashing ------------------------------------------------------------- */
inline std::array<uint8_t, 32> sha256(const Bytes& d) {
    std::array<uint8_t, 32> o{}; slc_sha256(d.data(), d.size(), o.data()); return o;
}
inline std::array<uint8_t, 48> sha384(const Bytes& d) {
    std::array<uint8_t, 48> o{}; slc_sha384(d.data(), d.size(), o.data()); return o;
}
inline std::array<uint8_t, 64> sha512(const Bytes& d) {
    std::array<uint8_t, 64> o{}; slc_sha512(d.data(), d.size(), o.data()); return o;
}
inline std::array<uint8_t, 32> sha3_256(const Bytes& d) {
    std::array<uint8_t, 32> o{}; slc_sha3_256(d.data(), d.size(), o.data()); return o;
}
inline std::array<uint8_t, 64> sha3_512(const Bytes& d) {
    std::array<uint8_t, 64> o{}; slc_sha3_512(d.data(), d.size(), o.data()); return o;
}

/* ---- HMAC / HKDF --------------------------------------------------------- */
inline std::array<uint8_t, 32> hmac_sha256(const Bytes& key, const Bytes& msg) {
    std::array<uint8_t, 32> o{};
    slc_hmac_sha256(key.data(), key.size(), msg.data(), msg.size(), o.data());
    return o;
}
inline std::array<uint8_t, 64> hmac_sha512(const Bytes& key, const Bytes& msg) {
    std::array<uint8_t, 64> o{};
    slc_hmac_sha512(key.data(), key.size(), msg.data(), msg.size(), o.data());
    return o;
}
inline Bytes hkdf_sha256(const Bytes& salt, const Bytes& ikm, const Bytes& info, size_t out_len) {
    Bytes o(out_len);
    slc_hkdf_sha256(salt.data(), salt.size(), ikm.data(), ikm.size(),
                    info.data(), info.size(), o.data(), out_len);
    return o;
}

/* ---- AES-GCM ------------------------------------------------------------- */
struct GcmResult { Bytes ciphertext; Bytes tag; };

inline GcmResult aes_gcm_encrypt(const Bytes& key, const Bytes& iv, const Bytes& aad,
                                 const Bytes& pt, size_t tag_len = 16) {
    GcmResult r; r.ciphertext.resize(pt.size()); r.tag.resize(tag_len);
    int bits = static_cast<int>(key.size() * 8);
    auto st = slc_aes_gcm_encrypt(key.data(), bits, iv.data(), iv.size(),
                                  aad.data(), aad.size(), pt.data(), pt.size(),
                                  r.ciphertext.data(), r.tag.data(), tag_len);
    if (st != SLCRYPTO_OK) throw std::runtime_error("aes_gcm_encrypt failed");
    return r;
}

/* Returns plaintext; throws on authentication failure (bad tag). */
inline Bytes aes_gcm_decrypt(const Bytes& key, const Bytes& iv, const Bytes& aad,
                             const Bytes& ct, const Bytes& tag) {
    Bytes pt(ct.size());
    int bits = static_cast<int>(key.size() * 8);
    auto st = slc_aes_gcm_decrypt(key.data(), bits, iv.data(), iv.size(),
                                  aad.data(), aad.size(), ct.data(), ct.size(),
                                  tag.data(), tag.size(), pt.data());
    if (st == SLCRYPTO_ERR_AUTH) throw std::runtime_error("aes_gcm_decrypt: authentication failed");
    if (st != SLCRYPTO_OK)       throw std::runtime_error("aes_gcm_decrypt failed");
    return pt;
}

/* ---- hex helper (test/diagnostic convenience) ---------------------------- */
template <typename C>
inline std::string to_hex(const C& bytes) {
    static const char* H = "0123456789abcdef";
    std::string s; s.reserve(bytes.size() * 2);
    for (uint8_t b : bytes) { s.push_back(H[b >> 4]); s.push_back(H[b & 0xf]); }
    return s;
}

} // namespace slcrypto
#endif // SLCRYPTO_HPP
