#ifndef SLEELA_POST_H
#define SLEELA_POST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *method;
    const char *target;
    const char *content_type;
    const char *body;
} sleela_post_t;

/* Validate the basic SLeeLa posting vocabulary without performing network I/O. */
int sleela_post_validate(const sleela_post_t *post, char *error, size_t error_size);

/* Emit a bounded HTTP-style POST representation to the supplied stream. */
int sleela_post_write(const sleela_post_t *post, int fd, char *error, size_t error_size);

#ifdef __cplusplus
}
#endif
#endif
