# SLeeLa National-Grade Cryptography (`/crypto`)

Standards-faithful **reference implementations** of the publicly specified
cryptographic primitives that make up the modern U.S. government / "national
grade" algorithm suites — the NSA **CNSA** (Commercial National Security
Algorithm) Suite 1.0 and 2.0 — built entirely from open **NIST FIPS** and
**SP** standards and IETF RFCs.

> ## ⚠️ Security boundary — read first
>
> This code is a **correct, specification-faithful reference** intended for
> study, interoperability testing, and use inside the SLeeLa toolchain. It is
> **not hardened for production**: the implementations are **not guaranteed
> constant-time** and are **not side-channel resistant**. To protect real data,
> use an audited, hardened library — **BoringSSL, OpenSSL, libsodium, or
> liboqs**.
>
> Everything here implements **public, published standards**. Nothing weakens,
> backdoors, or circumvents cryptography.

## Algorithms

| Category | Algorithm | Standard | Status |
|---|---|---|---|
| Hash | SHA-256 / SHA-384 / SHA-512 | FIPS 180-4 | ✅ implemented, KAT-verified |
| Hash | SHA3-256 / SHA3-512 (Keccak-f[1600]) | FIPS 202 | ✅ implemented, KAT-verified |
| Symmetric | AES-128 / AES-192 / AES-256 (block) | FIPS 197 | ✅ implemented, KAT-verified |
| AEAD | AES-GCM | NIST SP 800-38D | ✅ implemented, KAT-verified |
| MAC | HMAC-SHA-256 / HMAC-SHA-512 | FIPS 198-1 / RFC 2104 | ✅ implemented, KAT-verified |
| KDF | HKDF (extract + expand) | RFC 5869 | ✅ implemented |
| Post-quantum KEM | ML-KEM (Kyber) 512/768/1024 | FIPS 203 | 🔶 interface declared (see below) |
| Post-quantum signature | ML-DSA (Dilithium) 44/65/87 | FIPS 204 | 🔶 interface declared (see below) |

The two post-quantum primitives are the ones named by **CNSA 2.0**. Their stable
C entry points are declared in `include/slcrypto.h` and currently return
`SLCRYPTO_ERR_UNIMPL`; a faithful implementation should be vendored from the
audited pq-crystals / liboqs reference code rather than hand-rolled. See
[`PQ-INTEGRATION.md`](PQ-INTEGRATION.md).

## Layout

```
crypto/
  include/slcrypto.h     Public C API (all primitives)
  src/
    sha2.c               SHA-256/384/512
    sha3.c               SHA3-256/512 (Keccak)
    aes.c                AES core + AES-GCM AEAD
    mac_kdf.c            HMAC, HKDF, constant-time compare
    pq.c                 ML-KEM / ML-DSA entry points (pending)
  cpp/slcrypto.hpp       Header-only C++17 façade (std::vector / std::array)
  test/kat_test.cpp      Known-Answer Tests vs FIPS/NIST/RFC vectors
  Makefile               `make` builds libslcrypto.a; `make test` runs the KATs
```

## Build & test

```sh
cd crypto
make          # builds libslcrypto.a
make test     # builds and runs the Known-Answer Tests
```

The KAT suite checks every implemented primitive against published vectors
(SHA-2/3 from FIPS, AES block from FIPS 197 Appendix C, AES-GCM from SP 800-38D,
HMAC from RFC 4231) and includes decrypt/round-trip and tag-verification checks.
All checks pass.

## C API at a glance

```c
#include "slcrypto.h"

uint8_t md[32];
slc_sha256("abc", 3, md);                 /* one-shot hash */

uint8_t tag[16], ct[LEN];
slc_aes_gcm_encrypt(key, 256, iv, 12, aad, aad_len,
                    pt, LEN, ct, tag, 16); /* AES-256-GCM AEAD */

uint8_t okm[64];
slc_hkdf_sha256(salt, sl, ikm, il, info, inl, okm, 64); /* HKDF */
```

## C++ API at a glance

```cpp
#include "slcrypto.hpp"
using namespace slcrypto;

auto digest = sha256(Bytes{'a','b','c'});
auto enc    = aes_gcm_encrypt(key, iv, aad, plaintext);  // {ciphertext, tag}
auto pt     = aes_gcm_decrypt(key, iv, aad, enc.ciphertext, enc.tag); // throws on bad tag
```

## SLeeLa library equivalents

The SLeeLa-language façades for these primitives live under
[`lib/crypto`](../lib/crypto) as `.sleela` source units (the family already
present in the standard library). They name the algorithms and model the
configure/enable lifecycle; the actual computation crosses the explicit VM/OS
bridge to this native implementation. See `lib/crypto/SLNationalSuite.sleela`
and the per-algorithm units added alongside the existing crypto façades.
