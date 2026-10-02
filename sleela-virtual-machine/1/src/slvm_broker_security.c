#include "slvm_broker_security.h"
#include <string.h>
void slvm_broker_security_init(slvm_broker_security_t *s){if(!s)return;memset(s,0,sizeof(*s));s->mode=SLVM_BROKER_CREDENTIAL_NONE;s->require_encryption=1;s->require_peer_identity=1;s->allow_password_fallback=0;}
int slvm_broker_security_validate(const slvm_broker_security_t *s){if(!s||!s->require_encryption||!s->require_peer_identity||s->mode==SLVM_BROKER_CREDENTIAL_NONE||!s->reference||!s->reference[0])return 0;return 1;}