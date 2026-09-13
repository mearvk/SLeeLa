#ifndef SLEELA_HTTP3_KEY_DISTRIBUTION_CLIENT_HPP
#define SLEELA_HTTP3_KEY_DISTRIBUTION_CLIENT_HPP

#include "crypto_openssl.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

namespace sleela::http3 {

/*
 * Minimal C++ bootstrap client for the KDS protocol.
 * The network transport is intentionally separate: callers may use HTTP/3,
 * HTTPS, or another authenticated transport without changing the DH contract.
 */
class KeyDistributionBootstrap {
public:
    KeyDistributionBootstrap() {
        if (!http3_openssl_x25519_generate(private_key_.data(), public_key_.data()))
            throw std::runtime_error("X25519 key generation failed");
    }

    KeyDistributionBootstrap(const KeyDistributionBootstrap&) = delete;
    KeyDistributionBootstrap& operator=(const KeyDistributionBootstrap&) = delete;

    ~KeyDistributionBootstrap() { clear(); }

    std::array<uint8_t, HTTP3_OPENSSL_X25519_KEY_SIZE> public_key() const {
        return public_key_;
    }

    std::string public_key_hex() const {
        static constexpr char hex[] = "0123456789abcdef";
        std::string out;
        out.resize(64);
        for (size_t i = 0; i < 32; ++i) {
            out[i * 2] = hex[public_key_[i] >> 4];
            out[i * 2 + 1] = hex[public_key_[i] & 0x0f];
        }
        return out;
    }

    std::string bootstrap_request(const std::string& client_id,
                                  const std::string& jurisdiction) const {
        if (client_id.empty() || jurisdiction.empty() ||
            client_id.find('"') != std::string::npos ||
            jurisdiction.find('"') != std::string::npos)
            throw std::invalid_argument("invalid bootstrap identity fields");
        return std::string("{\"client_public\":\"") + public_key_hex() +
               "\",\"client_id\":\"" + client_id +
               "\",\"jurisdiction\":\"" + jurisdiction + "\"}";
    }

    /* Derive the same contract key used by the C KDS server. */
    bool derive_contract_key(const uint8_t server_public[32],
                             const uint8_t key_id[16],
                             uint8_t output[32]) const {
        uint8_t shared[32];
        const uint8_t info[] = "SLeeLa/HTTP3/KDS/bootstrap/v1";
        bool ok = http3_openssl_x25519_derive(private_key_.data(), server_public, shared) &&
                  http3_openssl_hkdf_sha256(shared, sizeof(shared), key_id, 16,
                                            info, sizeof(info) - 1, output);
        std::memset(shared, 0, sizeof(shared));
        return ok;
    }

    bool decrypt_contract(const uint8_t server_public[32],
                          const uint8_t key_id[16],
                          const uint8_t* ciphertext,
                          size_t ciphertext_len,
                          uint8_t* plaintext,
                          size_t plaintext_capacity,
                          size_t* plaintext_len) const {
        uint8_t key[32];
        if (!ciphertext || ciphertext_len < 28U || !plaintext || !plaintext_len ||
            plaintext_capacity < ciphertext_len - 28U) return false;
        if (!derive_contract_key(server_public, key_id, key)) return false;
        const size_t encrypted_len = ciphertext_len - 12U - 16U;
        bool ok = http3_openssl_aes256gcm_decrypt(
            key, ciphertext, key_id, 16,
            ciphertext + 12U, encrypted_len,
            ciphertext + 12U + encrypted_len, plaintext) != 0;
        std::memset(key, 0, sizeof(key));
        if (ok) *plaintext_len = encrypted_len;
        return ok;
    }

    void clear() {
        volatile uint8_t* p = private_key_.data();
        for (size_t i = 0; i < private_key_.size(); ++i) p[i] = 0;
        public_key_.fill(0);
    }

private:
    std::array<uint8_t, 32> private_key_{};
    std::array<uint8_t, 32> public_key_{};
};

} // namespace sleela::http3

#endif
