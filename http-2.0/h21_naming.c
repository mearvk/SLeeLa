/* ==========================================================================
 * h21_naming.c -- HTTP 2.1 fast naming (SKETCH).
 *
 * The function bodies below are STUBS that define the 2.1 shape. Fill them in
 * to mirror the HTTP 3.0 naming dictionary (name -> compact id with caching,
 * id -> name reverse lookup). Marked TODO where the logic belongs.
 * ========================================================================== */
#include "h21_naming.h"

#include <string.h>

void h21_naming_init(h21_naming_t *dict)
{
    if (dict == NULL) {
        return;
    }
    memset(dict, 0, sizeof(*dict));
    dict->next_service_id = 1u; /* ids start at 1; 0 == unresolved */
}

uint32_t h21_naming_intern_service(h21_naming_t *dict, const char *service_name)
{
    (void)dict; (void)service_name;
    /* TODO(2.1): find existing service by name -> return its id (cache hit);
     * otherwise allocate next_service_id, store the name, return the new id. */
    return 0u;
}

uint32_t h21_naming_intern_op(h21_naming_t *dict, const char *service_name, const char *op_name)
{
    (void)dict; (void)service_name; (void)op_name;
    /* TODO(2.1): intern the service, then allocate/return a service-local op id
     * (op ids start at 1 within each service). */
    return 0u;
}

uint32_t h21_naming_lookup_service(const h21_naming_t *dict, const char *service_name)
{
    (void)dict; (void)service_name;
    /* TODO(2.1): non-allocating lookup; return 0 if not yet interned. */
    return 0u;
}

const char *h21_naming_service_name(const h21_naming_t *dict, uint32_t service_id)
{
    (void)dict; (void)service_id;
    /* TODO(2.1): reverse lookup id -> name; NULL if unknown. */
    return NULL;
}
