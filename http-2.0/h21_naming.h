/* ==========================================================================
 * h21_naming.h -- SLeeLa HTTP 2.1 fast naming (SKETCH).
 *
 * Same design goal as HTTP 3.0 fast naming: service/operation names are
 * performance-critical. The first request may carry full names; later requests
 * over the same connection use compact numeric ids negotiated here. The ids are
 * connection/service-local and MUST NOT be assumed globally stable.
 * ========================================================================== */
#ifndef H21_NAMING_H
#define H21_NAMING_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define H21_NAME_MAX        64u
#define H21_MAX_SERVICES    64u
#define H21_MAX_OPS_PER_SVC 64u

typedef struct {
    uint32_t op_id;
    char     name[H21_NAME_MAX];
} h21_op_entry_t;

typedef struct {
    uint32_t       service_id;
    char           name[H21_NAME_MAX];
    h21_op_entry_t ops[H21_MAX_OPS_PER_SVC];
    size_t         op_count;
} h21_service_entry_t;

/* The connection-local naming dictionary. Compact ids start at 1; 0 means
 * "unresolved". */
typedef struct {
    h21_service_entry_t services[H21_MAX_SERVICES];
    size_t              service_count;
    uint32_t            next_service_id;
} h21_naming_t;

void     h21_naming_init(h21_naming_t *dict);
uint32_t h21_naming_intern_service(h21_naming_t *dict, const char *service_name);
uint32_t h21_naming_intern_op(h21_naming_t *dict, const char *service_name, const char *op_name);
uint32_t h21_naming_lookup_service(const h21_naming_t *dict, const char *service_name);
const char *h21_naming_service_name(const h21_naming_t *dict, uint32_t service_id);

#ifdef __cplusplus
}
#endif

#endif /* H21_NAMING_H */
