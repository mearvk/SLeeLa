/* ==========================================================================
 * h21_naming.c -- HTTP 2.1 fast naming.
 *
 * Connection-local dictionary: service/operation names → compact numeric IDs.
 * IDs start at 1 (0 == unresolved).  The first request may carry full names;
 * later requests over the same connection use the compact IDs negotiated here.
 * IDs are connection-local and MUST NOT be assumed globally stable.
 * ========================================================================== */
#include "h21_naming.h"

#include <string.h>

/* ── init ─────────────────────────────────────────────────────────── */

void h21_naming_init(h21_naming_t *dict)
{
    if (dict == NULL)
        return;
    memset(dict, 0, sizeof(*dict));
    dict->next_service_id = 1u; /* IDs start at 1; 0 == unresolved */
}

/* ── service intern (get-or-create) ───────────────────────────────── */

uint32_t h21_naming_intern_service(h21_naming_t *dict, const char *service_name)
{
    if (!dict || !service_name || !*service_name)
        return 0u;

    /* Cache hit: already interned. */
    for (size_t i = 0; i < dict->service_count; ++i) {
        if (strncmp(dict->services[i].name, service_name, H21_NAME_MAX) == 0)
            return dict->services[i].service_id;
    }

    /* Capacity check. */
    if (dict->service_count >= H21_MAX_SERVICES)
        return 0u;

    /* Allocate a new entry. */
    h21_service_entry_t *entry = &dict->services[dict->service_count];
    memset(entry, 0, sizeof(*entry));
    entry->service_id = dict->next_service_id++;
    strncpy(entry->name, service_name, H21_NAME_MAX - 1);
    entry->name[H21_NAME_MAX - 1] = '\0';
    ++dict->service_count;

    return entry->service_id;
}

/* ── operation intern (get-or-create within a service) ────────────── */

uint32_t h21_naming_intern_op(h21_naming_t *dict,
                              const char   *service_name,
                              const char   *op_name)
{
    if (!dict || !service_name || !*service_name || !op_name || !*op_name)
        return 0u;

    /* Ensure the service exists (creates if needed). */
    uint32_t svc_id = h21_naming_intern_service(dict, service_name);
    if (svc_id == 0)
        return 0u;

    /* Find the service entry. */
    h21_service_entry_t *svc = NULL;
    for (size_t i = 0; i < dict->service_count; ++i) {
        if (dict->services[i].service_id == svc_id) {
            svc = &dict->services[i];
            break;
        }
    }
    if (!svc)
        return 0u;

    /* Cache hit: already interned. */
    for (size_t i = 0; i < svc->op_count; ++i) {
        if (strncmp(svc->ops[i].name, op_name, H21_NAME_MAX) == 0)
            return svc->ops[i].op_id;
    }

    /* Capacity check. */
    if (svc->op_count >= H21_MAX_OPS_PER_SVC)
        return 0u;

    /* Allocate.  Op IDs are 1-based within each service. */
    h21_op_entry_t *op = &svc->ops[svc->op_count];
    op->op_id = (uint32_t)(svc->op_count + 1);
    strncpy(op->name, op_name, H21_NAME_MAX - 1);
    op->name[H21_NAME_MAX - 1] = '\0';
    ++svc->op_count;

    return op->op_id;
}

/* ── non-allocating lookup ────────────────────────────────────────── */

uint32_t h21_naming_lookup_service(const h21_naming_t *dict,
                                   const char         *service_name)
{
    if (!dict || !service_name || !*service_name)
        return 0u;

    for (size_t i = 0; i < dict->service_count; ++i) {
        if (strncmp(dict->services[i].name, service_name, H21_NAME_MAX) == 0)
            return dict->services[i].service_id;
    }

    return 0u;
}

/* ── reverse lookup: id → name ────────────────────────────────────── */

const char *h21_naming_service_name(const h21_naming_t *dict,
                                    uint32_t            service_id)
{
    if (!dict || service_id == 0)
        return NULL;

    for (size_t i = 0; i < dict->service_count; ++i) {
        if (dict->services[i].service_id == service_id)
            return dict->services[i].name;
    }

    return NULL;
}
