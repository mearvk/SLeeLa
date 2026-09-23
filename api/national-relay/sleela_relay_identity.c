#include "sleela_relay_identity.h"
#include <string.h>
static int copy_ref(char *dst, size_t cap, const char *src) {
    size_t n;
    if (!dst || !src || !*src) return -1;
    n = strlen(src);
    if (n >= cap) return -1;
    for (size_t j=0; j<n; ++j) if ((unsigned char)src[j] < 0x21 || src[j] == ' ') return -1;
    memcpy(dst, src, n + 1);
    return 0;
}
int sleela_relay_identity_init(sleela_relay_identity_t *i,const char *j,const char *o,const char *p) {
    if (!i || !j || !o || !p) return -1;
    memset(i,0,sizeof(*i));
    if (copy_ref(i->jurisdiction,sizeof(i->jurisdiction),j) ||
        copy_ref(i->relay_operator_ref,sizeof(i->relay_operator_ref),o) ||
        copy_ref(i->purpose,sizeof(i->purpose),p)) return -1;
    return 0;
}
int sleela_relay_identity_set_national_ref(sleela_relay_identity_t *i,const char *v){return i?copy_ref(i->national_id_ref,sizeof(i->national_id_ref),v):-1;}
int sleela_relay_identity_set_citizen_ref(sleela_relay_identity_t *i,const char *v){return i?copy_ref(i->citizen_id_ref,sizeof(i->citizen_id_ref),v):-1;}
int sleela_relay_identity_set_bank_ref(sleela_relay_identity_t *i,const char *v){return i?copy_ref(i->bank_id_ref,sizeof(i->bank_id_ref),v):-1;}
int sleela_relay_identity_validate(const sleela_relay_identity_t *i){return (!i||!i->jurisdiction[0]||!i->relay_operator_ref[0]||!i->purpose[0])?-1:0;}
