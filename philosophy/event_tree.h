#ifndef SLEELA_PHILOSOPHY_EVENT_TREE_H
#define SLEELA_PHILOSOPHY_EVENT_TREE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SL_EVENT_KEY_CAPACITY 48u
#define SL_EVENT_UNIT_CAPACITY 24u
#define SL_EVENT_SOURCE_CAPACITY 128u
#define SL_EVENT_NOTE_CAPACITY 384u
#define SL_EVENT_ROLE_CAPACITY 64u
typedef enum sl_status {
    SL_STATUS_OK=0, SL_STATUS_INVALID_ARGUMENT, SL_STATUS_INVALID_NUMBER,
    SL_STATUS_INVALID_EVENT, SL_STATUS_DUPLICATE_ID, SL_STATUS_NOT_FOUND,
    SL_STATUS_OUT_OF_MEMORY
} sl_status;
typedef enum sl_event_type {
    SL_EVENT_SOCIAL=0, SL_EVENT_ECONOMIC_METRIC, SL_EVENT_HEALTH_CONTEXT,
    SL_EVENT_MEDICAL_REQUEST, SL_EVENT_ASSESSOR_OPINION, SL_EVENT_REFLECTION
} sl_event_type;
typedef enum sl_source_kind {
    SL_SOURCE_SELF_REPORT=0, SL_SOURCE_DIRECT_OBSERVATION,
    SL_SOURCE_MEDIA_OR_PUBLICATION, SL_SOURCE_ASSESSOR_AUTHORED,
    SL_SOURCE_SYSTEM_IMPORT
} sl_source_kind;
typedef enum sl_value_kind { SL_VALUE_NONE=0, SL_VALUE_INTEGER, SL_VALUE_REAL } sl_value_kind;
typedef enum sl_listener_mode { SL_LISTENER_NEUTRAL=0, SL_LISTENER_EVIDENCE_AWARE } sl_listener_mode;
typedef struct sl_numeric_value {
    sl_value_kind kind;
    union { int64_t integer; double real; } as;
} sl_numeric_value;
typedef struct sl_event {
    uint64_t id, occurred_at, ended_at, half_life_seconds, related_event_id;
    char property[SL_EVENT_KEY_CAPACITY];
    char unit[SL_EVENT_UNIT_CAPACITY];
    char source_reference[SL_EVENT_SOURCE_CAPACITY];
    char note[SL_EVENT_NOTE_CAPACITY];
    char assessor_role[SL_EVENT_ROLE_CAPACITY];
    sl_event_type type;
    sl_source_kind source;
    sl_numeric_value numeric;
    bool consent_to_share;
    bool assessor_attested;
} sl_event;
typedef struct sl_event_tree sl_event_tree;
typedef bool (*sl_event_visitor)(const sl_event *, double, void *);
typedef struct sl_tree_stats { size_t count; size_t height; } sl_tree_stats;

sl_status sl_event_init_integer(
    sl_event *, uint64_t, sl_event_type, sl_source_kind, const char *,
    const char *, int64_t, uint64_t, uint64_t, uint64_t, bool,
    const char *, const char *);
sl_status sl_event_init_real(
    sl_event *, uint64_t, sl_event_type, sl_source_kind, const char *,
    const char *, double, uint64_t, uint64_t, uint64_t, bool,
    const char *, const char *);
/* Only supplied, author-attested text is accepted; no identity, credential,
   clinical correctness, or diagnosis is verified by this library. */
sl_status sl_event_set_assessor_opinion(
    sl_event *, const char *, const char *, bool);

sl_event_tree *sl_event_tree_create(void);
void sl_event_tree_destroy(sl_event_tree *);
sl_status sl_event_tree_insert(sl_event_tree *, const sl_event *);
sl_status sl_event_tree_find(const sl_event_tree *, uint64_t, sl_event *);
sl_status sl_event_tree_end(sl_event_tree *, uint64_t, uint64_t);
double sl_event_relevance(const sl_event *, uint64_t);
/* Raw traversal includes private records: caller MUST enforce consent. */
sl_status sl_event_tree_visit(
    const sl_event_tree *, uint64_t, double, sl_event_visitor, void *);
sl_status sl_event_tree_prune(sl_event_tree *, uint64_t, double, size_t *);
sl_tree_stats sl_event_tree_get_stats(const sl_event_tree *);
const char *sl_status_string(sl_status);
const char *sl_event_type_string(sl_event_type);
const char *sl_source_kind_string(sl_source_kind);
#ifdef __cplusplus
}
#endif
#endif
