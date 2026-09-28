/* ==========================================================================
 * http3_naming.h -- SLeeLa HTTP 3.0 fast naming (spec §4).
 *
 * HTTP 3.0 treats service and operation names as performance-critical data.
 * The first request may carry full names; later requests over the same
 * connection use compact numeric identifiers negotiated here:
 *
 *     First request:   service="orders",  operation="calculate"
 *     Later requests:  service=7,          operation=3
 *
 * The numeric ids are connection/service-local and MUST NOT be assumed
 * globally stable (spec §4). This dictionary resolves names -> compact ids
 * (caching the result) and ids -> names (for dispatch and diagnostics).
 * ========================================================================== */
#ifndef HTTP3_NAMING_H
#define HTTP3_NAMING_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP3_NAME_MAX        64u   /* max length of a service/operation name  */
#define HTTP3_MAX_SERVICES    64u   /* dictionary capacity: services           */
#define HTTP3_MAX_OPS_PER_SVC 64u   /* dictionary capacity: ops per service    */

/* An operation entry: its local id and name. */
typedef struct {
    uint32_t op_id;
    char     name[HTTP3_NAME_MAX];
} http3_op_entry_t;

/* A service entry: its local id, name, and its operation table. */
typedef struct {
    uint32_t         service_id;
    char             name[HTTP3_NAME_MAX];
    http3_op_entry_t ops[HTTP3_MAX_OPS_PER_SVC];
    size_t           op_count;
} http3_service_entry_t;

/* The connection-local naming dictionary. Compact ids are assigned in the
 * order names are first seen (ids start at 1; 0 means "unresolved"). */
typedef struct {
    http3_service_entry_t services[HTTP3_MAX_SERVICES];
    size_t                service_count;
    uint32_t              next_service_id;
} http3_naming_t;

/* Initialize an empty dictionary. */
void http3_naming_init(http3_naming_t *dict);

/*
 * Resolve a service name to its compact id, allocating a new id on first use
 * (this is the caching step of §4). Returns the id, or 0 on capacity/input
 * error.
 */
uint32_t http3_naming_intern_service(http3_naming_t *dict, const char *service_name);

/*
 * Resolve an operation name within a service to its compact id, allocating on
 * first use. Returns the op id, or 0 on error.
 */
uint32_t http3_naming_intern_op(http3_naming_t *dict,
                                const char *service_name,
                                const char *op_name);

/* Look up ids WITHOUT allocating (returns 0 if not yet interned). */
uint32_t http3_naming_lookup_service(const http3_naming_t *dict, const char *service_name);
uint32_t http3_naming_lookup_op(const http3_naming_t *dict, uint32_t service_id, const char *op_name);

/* Reverse lookups: compact id -> name (for dispatch and §17 diagnostics).
 * Returns NULL if the id is unknown. */
const char *http3_naming_service_name(const http3_naming_t *dict, uint32_t service_id);
const char *http3_naming_op_name(const http3_naming_t *dict, uint32_t service_id, uint32_t op_id);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_NAMING_H */
