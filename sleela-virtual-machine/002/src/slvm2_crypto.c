/* ==========================================================================
 * slvm2_crypto.c  --  SLVM/2 "Operator" cryptographic provider validation.
 *
 * The Operator generation's distinguishing contract is a cryptographic +
 * VM-link layer over the Foundation: before the VM admits a program it must
 * have a usable crypto provider (digest + signature verification) bound. This
 * implements the validator declared in slvm2_crypto.h.
 *
 * Contract: a provider is valid iff it is non-NULL, names itself, declares a
 * non-zero ABI version, and exposes a complete API vtable (both digest and
 * verify_signature callbacks present). Returns 0 on success, -1 otherwise.
 * ========================================================================== */
#include "slvm2_crypto.h"

int slvm2_crypto_validate_provider(const slvm2_crypto_provider_t *p) {
    if (!p) return -1;
    if (!p->name || p->name[0] == '\0') return -1;
    if (p->version == 0u) return -1;
    if (!p->api) return -1;
    if (!p->api->digest || !p->api->verify_signature) return -1;
    return 0;
}
