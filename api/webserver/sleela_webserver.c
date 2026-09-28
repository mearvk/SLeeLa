#include "sleela_webserver.h"

#include <stdio.h>
#include <string.h>

static int bounded_nonempty(const char *s, size_t max)
{
    return s != NULL && *s != '\0' && strnlen(s, max + 1U) <= max;
}

const char *sleela_webserver_kind_name(sleela_webserver_kind_t kind)
{
    switch (kind) {
    case SLEELA_WEBSERVER_APACHE: return "apache";
    case SLEELA_WEBSERVER_TOMCAT: return "tomcat";
    default: return NULL;
    }
}

int sleela_webserver_validate(const sleela_webserver_config_t *config,
                              char *error, size_t error_size)
{
    if (error != NULL && error_size > 0U)
        error[0] = '\0';

    if (config == NULL) {
        if (error != NULL && error_size > 0U)
            snprintf(error, error_size, "configuration is null");
        return -1;
    }

    if (sleela_webserver_kind_name(config->kind) == NULL) {
        if (error != NULL && error_size > 0U)
            snprintf(error, error_size, "unsupported web server");
        return -1;
    }

    if (config->port < 1U || config->port > 65535U) {
        if (error != NULL && error_size > 0U)
            snprintf(error, error_size, "port must be 1..65535");
        return -1;
    }

    if (!bounded_nonempty(config->health_url, 4096U) ||
        config->health_url[0] != '/') {
        if (error != NULL && error_size > 0U)
            snprintf(error, error_size, "health_url must be an absolute path");
        return -1;
    }

    if (config->module_path != NULL &&
        strnlen(config->module_path, 4097U) > 4096U) {
        if (error != NULL && error_size > 0U)
            snprintf(error, error_size, "module_path is too long");
        return -1;
    }

    return 0;
}
