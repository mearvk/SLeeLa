# HTTP/8.0 Cryptography Support

HTTP/8.0 provides a crypto abstraction layer for major standardized cryptographic families and provider-backed implementations.

Catalogued families include SHA-2, SHA-3/SHAKE, HMAC/CMAC/KMAC, HKDF/PBKDF2, AES-GCM/CCM, ChaCha20-Poly1305, AES, Camellia, ARIA, SM4, X25519, X448, ECDH, DH, RSA, ECDSA, Ed25519, Ed448, ML-KEM, ML-DSA and SLH-DSA. Legacy entries such as SHA-1, MD5, DES, 3DES, RC4 and DSA are retained for migration and interoperability but are not approved for new sessions by this catalog.

This is a registry and adapter contract, not an independent implementation of every cryptographic primitive. Actual operations must be supplied by a reviewed cryptographic library/provider. OpenSSL 3 EVP/provider is a suitable backend. Supported means catalogued; runtime provider availability still must be checked.

Transition: handshake -> profile negotiation -> DH/KEM -> transcript/key confirmation -> KDF -> authenticated packet protection.

Hybrid post-quantum profiles may combine X25519/X448 with ML-KEM before KDF.
