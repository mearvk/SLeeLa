#ifndef SLEELA_EMAIL_FACADE_H
#define SLEELA_EMAIL_FACADE_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLEELA_EMAIL_TLS_NONE=0, SLEELA_EMAIL_TLS_STARTTLS=1, SLEELA_EMAIL_TLS_IMPLICIT=2 } sleela_email_tls_mode_t;
typedef struct {
 const char *smtp_host; unsigned short smtp_port; const char *helo_name; const char *local_bind_host;
 sleela_email_tls_mode_t tls_mode; const char *username; const char *password;
 const char *from; const char *to; const char *subject; const char *body;
} sleela_email_message_t;
int sleela_email_send(const sleela_email_message_t *message, char *error_text, size_t error_text_size);
const char *sleela_email_tls_mode_name(sleela_email_tls_mode_t mode);
#ifdef __cplusplus
}
#endif
#endif
