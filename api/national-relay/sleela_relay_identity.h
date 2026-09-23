#ifndef SLEELA_RELAY_IDENTITY_H
#define SLEELA_RELAY_IDENTITY_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_RELAY_ID_MAX 256u
typedef struct {
    char national_id_ref[SLEELA_RELAY_ID_MAX];
    char citizen_id_ref[SLEELA_RELAY_ID_MAX];
    char bank_id_ref[SLEELA_RELAY_ID_MAX];
    char jurisdiction[SLEELA_RELAY_ID_MAX];
    char relay_operator_ref[SLEELA_RELAY_ID_MAX];
    char purpose[SLEELA_RELAY_ID_MAX];
} sleela_relay_identity_t;
int sleela_relay_identity_init(sleela_relay_identity_t *, const char *, const char *, const char *);
int sleela_relay_identity_set_national_ref(sleela_relay_identity_t *, const char *);
int sleela_relay_identity_set_citizen_ref(sleela_relay_identity_t *, const char *);
int sleela_relay_identity_set_bank_ref(sleela_relay_identity_t *, const char *);
int sleela_relay_identity_validate(const sleela_relay_identity_t *);
#ifdef __cplusplus
}
#endif
#endif
