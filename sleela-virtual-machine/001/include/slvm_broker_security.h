#ifndef SLEELA_SLVM_BROKER_SECURITY_H
#define SLEELA_SLVM_BROKER_SECURITY_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLVM_BROKER_CREDENTIAL_NONE=0, SLVM_BROKER_CREDENTIAL_REFERENCE=1, SLVM_BROKER_CREDENTIAL_MTLS=2, SLVM_BROKER_CREDENTIAL_PSK_REFERENCE=3 } slvm_broker_credential_mode_t;
typedef struct { slvm_broker_credential_mode_t mode; const char *reference; uint8_t require_encryption; uint8_t require_peer_identity; uint8_t allow_password_fallback; } slvm_broker_security_t;
void slvm_broker_security_init(slvm_broker_security_t *);
int slvm_broker_security_validate(const slvm_broker_security_t *);
#ifdef __cplusplus
}
#endif
#endif