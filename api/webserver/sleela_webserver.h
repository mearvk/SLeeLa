#ifndef SLEELA_WEBSERVER_H
#define SLEELA_WEBSERVER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLEELA_WEBSERVER_APACHE = 1,
    SLEELA_WEBSERVER_TOMCAT = 2
} sleela_webserver_kind_t;

typedef struct {
    sleela_webserver_kind_t kind;
    unsigned short port;
    const char *health_url;
    const char *module_path;
} sleela_webserver_config_t;

int sleela_webserver_validate(const sleela_webserver_config_t *config,
                              char *error, size_t error_size);

const char *sleela_webserver_kind_name(sleela_webserver_kind_t kind);

#ifdef __cplusplus
}
#endif

#endif
