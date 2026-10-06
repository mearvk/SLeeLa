/*
 * crypto/src/pq.c
 * Post-quantum (CNSA 2.0) entry points: ML-KEM (FIPS 203) and ML-DSA
 * (FIPS 204).
 *
 * These are the stable interfaces named by the public NIST post-quantum
 * standards. A faithful implementation requires the full lattice stack
 * (NTT, rejection sampling, SHAKE-based XOF, packing) and should be vendored
 * from the audited pq-crystals / liboqs reference code rather than hand-rolled
 * here. Until that integration lands these entry points return
 * SLCRYPTO_ERR_UNIMPL. See crypto/PQ-INTEGRATION.md.
 */
#include "slcrypto.h"

slcrypto_status slc_mlkem_keygen(slc_mlkem_param p, uint8_t *pk, uint8_t *sk) {
    (void)p; (void)pk; (void)sk; return SLCRYPTO_ERR_UNIMPL;
}
slcrypto_status slc_mlkem_encaps(slc_mlkem_param p, const uint8_t *pk, uint8_t *ct, uint8_t *shared) {
    (void)p; (void)pk; (void)ct; (void)shared; return SLCRYPTO_ERR_UNIMPL;
}
slcrypto_status slc_mlkem_decaps(slc_mlkem_param p, const uint8_t *sk, const uint8_t *ct, uint8_t *shared) {
    (void)p; (void)sk; (void)ct; (void)shared; return SLCRYPTO_ERR_UNIMPL;
}

slcrypto_status slc_mldsa_keygen(slc_mldsa_param p, uint8_t *pk, uint8_t *sk) {
    (void)p; (void)pk; (void)sk; return SLCRYPTO_ERR_UNIMPL;
}
slcrypto_status slc_mldsa_sign(slc_mldsa_param p, const uint8_t *sk,
                               const uint8_t *msg, size_t msg_len,
                               uint8_t *sig, size_t *sig_len) {
    (void)p; (void)sk; (void)msg; (void)msg_len; (void)sig; (void)sig_len;
    return SLCRYPTO_ERR_UNIMPL;
}
slcrypto_status slc_mldsa_verify(slc_mldsa_param p, const uint8_t *pk,
                                 const uint8_t *msg, size_t msg_len,
                                 const uint8_t *sig, size_t sig_len) {
    (void)p; (void)pk; (void)msg; (void)msg_len; (void)sig; (void)sig_len;
    return SLCRYPTO_ERR_UNIMPL;
}
