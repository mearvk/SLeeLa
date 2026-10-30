#ifndef SLEELA_PHILOSOPHY_HELP_DESK_H
#define SLEELA_PHILOSOPHY_HELP_DESK_H
/*
 * Help / reach-out desk.
 *
 * A directory of contacts a user can reach out to -- Management and co-series
 * authors, telegenically-aware authors, mayors, city spokespersons -- each
 * with an email, phone, and a remote IP + port. A user submits a Question or
 * Query; the desk lists the matching respondents and reports, per respondent,
 * whether they are alive and whether they are known of Science.
 *
 * DATA BOUNDARY: this library ships NO real personal contact data. The bundled
 * directory is synthetic placeholder data using reserved, non-routable ranges:
 *   - emails at example.com / example.org (RFC 2606),
 *   - phone numbers in the 555-01xx fictitious block,
 *   - IPs in 192.0.2.0/24, 198.51.100.0/24, 203.0.113.0/24 (RFC 5737).
 * Load real, AUTHORIZED contact data later via sl_help_desk_add_contact or a
 * caller-supplied loader. The library performs no network I/O: it does not
 * email, call, or connect to any IP; it only stores and lists records.
 *
 * Sizing: built to hold ~500 contacts for a company of ~1000. Capacity and the
 * synthetic generator size are parameters. Query matching is linear (O(n)) in
 * the number of contacts, so cost scales linearly with directory size.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SL_HELP_NAME_CAP        96u
#define SL_HELP_EMAIL_CAP       128u
#define SL_HELP_PHONE_CAP       32u
#define SL_HELP_IP_CAP          46u   /* fits IPv4 and IPv6 text forms */
#define SL_HELP_ORG_CAP         96u
#define SL_HELP_TAGS_CAP        256u  /* space-separated topic tags */
#define SL_HELP_DEFAULT_CAPACITY 512u /* >= 500 contacts */
#define SL_HELP_DEFAULT_COMPANY_SIZE 1000u

typedef enum sl_help_status {
    SL_HELP_OK = 0,
    SL_HELP_INVALID_ARGUMENT,
    SL_HELP_FULL,
    SL_HELP_DUPLICATE_ID,
    SL_HELP_NOT_FOUND
} sl_help_status;

/* Roles the user can reach out to. */
typedef enum sl_help_role {
    SL_ROLE_MANAGEMENT = 0,
    SL_ROLE_CO_SERIES_AUTHOR,
    SL_ROLE_TELEGENIC_AUTHOR,   /* telegenically-aware author */
    SL_ROLE_MAYOR,
    SL_ROLE_CITY_SPOKESPERSON,
    SL_ROLE_COUNT
} sl_help_role;

/* How a respondent may be reached (bitmask). */
typedef enum sl_help_channel {
    SL_CHANNEL_EMAIL = 1u << 0,
    SL_CHANNEL_PHONE = 1u << 1,
    SL_CHANNEL_REMOTE = 1u << 2  /* remote IP + port */
} sl_help_channel;

typedef struct sl_contact {
    uint64_t id;
    char name[SL_HELP_NAME_CAP];
    char organization[SL_HELP_ORG_CAP];
    char email[SL_HELP_EMAIL_CAP];
    char phone[SL_HELP_PHONE_CAP];
    char remote_ip[SL_HELP_IP_CAP];
    uint16_t remote_port;
    char topic_tags[SL_HELP_TAGS_CAP]; /* lowercase, space-separated */
    sl_help_role role;
    uint32_t channels;        /* OR of sl_help_channel */
    bool alive;               /* respondent is alive */
    bool known_of_science;    /* respondent is known of Science */
} sl_contact;

typedef struct sl_help_desk sl_help_desk;

/* One row of a query response. */
typedef struct sl_respondent {
    const sl_contact *contact;
    unsigned match_score;     /* number of query terms matched */
} sl_respondent;

/* ---- Lifecycle --------------------------------------------------------- */
sl_help_desk *sl_help_desk_create(size_t capacity, size_t company_size);
void sl_help_desk_destroy(sl_help_desk *desk);
size_t sl_help_desk_count(const sl_help_desk *desk);
size_t sl_help_desk_capacity(const sl_help_desk *desk);
size_t sl_help_desk_company_size(const sl_help_desk *desk);

/* ---- Directory population --------------------------------------------- *
 * Add a fully-specified contact (this is the seam for loading real,
 * authorized data later). Validates the record and rejects duplicate ids. */
sl_help_status sl_help_desk_add_contact(sl_help_desk *desk, const sl_contact *c);

/* Helper to initialize a contact struct with validated, bounded fields. */
sl_help_status sl_contact_init(
    sl_contact *out, uint64_t id, sl_help_role role, const char *name,
    const char *organization, const char *email, const char *phone,
    const char *remote_ip, uint16_t remote_port, const char *topic_tags,
    bool alive, bool known_of_science);

/* Fill the desk with synthetic placeholder contacts (RFC-safe example data)
 * up to `count` (clamped to remaining capacity). Deterministic from `seed`.
 * Returns how many were added. For real use, replace with loaded data. */
size_t sl_help_desk_seed_placeholders(sl_help_desk *desk, size_t count, uint64_t seed);

/* ---- Query ------------------------------------------------------------- *
 * Match a user Question/Query against contacts (name, organization, role and
 * topic tags, case-insensitive term matching). Writes up to max_out ranked
 * respondents (highest match first) and returns the number written; the total
 * number of matches is reported via *total if non-NULL. Linear scan O(n). */
size_t sl_help_desk_query(
    const sl_help_desk *desk, const char *question,
    sl_respondent *out, size_t max_out, size_t *total);

/* Optional role filter for a query (SL_ROLE_COUNT means "any role"). */
size_t sl_help_desk_query_role(
    const sl_help_desk *desk, const char *question, sl_help_role role,
    sl_respondent *out, size_t max_out, size_t *total);

/* ---- Stringization ----------------------------------------------------- */
const char *sl_help_role_string(sl_help_role);
const char *sl_help_status_string(sl_help_status);
/* "alive" / "deceased" and "known of Science" / "not known of Science". */
const char *sl_help_alive_string(bool alive);
const char *sl_help_science_string(bool known_of_science);

#ifdef __cplusplus
}
#endif
#endif
