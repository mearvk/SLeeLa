/* ==========================================================================
 * http3_naming.c -- implementation of HTTP 3.0 fast naming (§4).
 * Connection-local name<->compact-id dictionary with first-use caching.
 * ========================================================================== */
#define _POSIX_C_SOURCE 200809L
#include "http3_naming.h"

#include <string.h>

void http3_naming_init(http3_naming_t *dict)
{
    if (dict == NULL) {
        return;
    }
    memset(dict, 0, sizeof(*dict));
    dict->next_service_id = 1u; /* 0 is reserved for "unresolved" */
}

static int name_ok(const char *name)
{
    size_t len;
    if (name == NULL) {
        return 0;
    }
    len = strnlen(name, HTTP3_NAME_MAX);
    return len > 0u && len < HTTP3_NAME_MAX;
}

static http3_service_entry_t *find_service(http3_naming_t *dict, const char *name)
{
    size_t i;
    for (i = 0; i < dict->service_count; ++i) {
        if (strncmp(dict->services[i].name, name, HTTP3_NAME_MAX) == 0) {
            return &dict->services[i];
        }
    }
    return NULL;
}

static http3_service_entry_t *find_service_by_id(http3_naming_t *dict, uint32_t id)
{
    size_t i;
    for (i = 0; i < dict->service_count; ++i) {
        if (dict->services[i].service_id == id) {
            return &dict->services[i];
        }
    }
    return NULL;
}

uint32_t http3_naming_intern_service(http3_naming_t *dict, const char *service_name)
{
    http3_service_entry_t *svc;
    if (dict == NULL || !name_ok(service_name)) {
        return 0u;
    }
    svc = find_service(dict, service_name);
    if (svc != NULL) {
        return svc->service_id; /* cached from a prior request */
    }
    if (dict->service_count >= HTTP3_MAX_SERVICES) {
        return 0u;
    }
    svc = &dict->services[dict->service_count++];
    svc->service_id = dict->next_service_id++;
    strncpy(svc->name, service_name, HTTP3_NAME_MAX - 1u);
    svc->name[HTTP3_NAME_MAX - 1u] = '\0';
    svc->op_count = 0u;
    return svc->service_id;
}

uint32_t http3_naming_intern_op(http3_naming_t *dict,
                                const char *service_name,
                                const char *op_name)
{
    http3_service_entry_t *svc;
    size_t i;
    uint32_t sid;
    if (dict == NULL || !name_ok(op_name)) {
        return 0u;
    }
    sid = http3_naming_intern_service(dict, service_name);
    if (sid == 0u) {
        return 0u;
    }
    svc = find_service_by_id(dict, sid);
    if (svc == NULL) {
        return 0u;
    }
    for (i = 0; i < svc->op_count; ++i) {
        if (strncmp(svc->ops[i].name, op_name, HTTP3_NAME_MAX) == 0) {
            return svc->ops[i].op_id; /* cached */
        }
    }
    if (svc->op_count >= HTTP3_MAX_OPS_PER_SVC) {
        return 0u;
    }
    /* Op ids are local to a service and start at 1. */
    svc->ops[svc->op_count].op_id = (uint32_t)(svc->op_count + 1u);
    strncpy(svc->ops[svc->op_count].name, op_name, HTTP3_NAME_MAX - 1u);
    svc->ops[svc->op_count].name[HTTP3_NAME_MAX - 1u] = '\0';
    ++svc->op_count;
    return svc->ops[svc->op_count - 1u].op_id;
}

uint32_t http3_naming_lookup_service(const http3_naming_t *dict, const char *service_name)
{
    size_t i;
    if (dict == NULL || !name_ok(service_name)) {
        return 0u;
    }
    for (i = 0; i < dict->service_count; ++i) {
        if (strncmp(dict->services[i].name, service_name, HTTP3_NAME_MAX) == 0) {
            return dict->services[i].service_id;
        }
    }
    return 0u;
}

uint32_t http3_naming_lookup_op(const http3_naming_t *dict, uint32_t service_id, const char *op_name)
{
    size_t i, j;
    if (dict == NULL || !name_ok(op_name)) {
        return 0u;
    }
    for (i = 0; i < dict->service_count; ++i) {
        if (dict->services[i].service_id != service_id) {
            continue;
        }
        for (j = 0; j < dict->services[i].op_count; ++j) {
            if (strncmp(dict->services[i].ops[j].name, op_name, HTTP3_NAME_MAX) == 0) {
                return dict->services[i].ops[j].op_id;
            }
        }
    }
    return 0u;
}

const char *http3_naming_service_name(const http3_naming_t *dict, uint32_t service_id)
{
    size_t i;
    if (dict == NULL) {
        return NULL;
    }
    for (i = 0; i < dict->service_count; ++i) {
        if (dict->services[i].service_id == service_id) {
            return dict->services[i].name;
        }
    }
    return NULL;
}

const char *http3_naming_op_name(const http3_naming_t *dict, uint32_t service_id, uint32_t op_id)
{
    size_t i, j;
    if (dict == NULL) {
        return NULL;
    }
    for (i = 0; i < dict->service_count; ++i) {
        if (dict->services[i].service_id != service_id) {
            continue;
        }
        for (j = 0; j < dict->services[i].op_count; ++j) {
            if (dict->services[i].ops[j].op_id == op_id) {
                return dict->services[i].ops[j].name;
            }
        }
    }
    return NULL;
}
