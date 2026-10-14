/*
 * registry/sleela_registry.h -- Sleela Registry: public C ABI.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * A Windows-style hierarchical registry for SLeeLa: a tree of KEYS (like
 * HKEY\Path\Sub) each holding named VALUES (settings and feature flags), plus a
 * per-entry PROVENANCE record for the registry's own tracked artifacts:
 *
 *     file name, date, author, version, hex id, SHA-256 hash, originality seal.
 *
 * A new SLeeLa install seeds a baseline registry that is reviewable from a
 * terminal (`sleela registry ...`) or a Sleela terminal program. The store is a
 * portable, deterministic text format so it is diffable, hashable, and
 * idempotent to regenerate.
 *
 * Design notes:
 *   - Keys form a path tree; a path is "A/B/C" (forward slashes, root = "").
 *   - A value is (name -> typed scalar: string/int/bool).
 *   - A provenance record is attached to a key by name and seals the identity +
 *     content hash of a tracked file, so tampering or misplacement is visible.
 *   - Pure C, extern "C", opaque handle, no dependencies beyond the C library.
 */
#ifndef SLEELA_REGISTRY_H
#define SLEELA_REGISTRY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_REGISTRY_VERSION   "1.0.0"
#define SLEELA_REGISTRY_MAGIC     "SLREG"      /* file-format magic           */
#define SLEELA_REGISTRY_FORMAT    1            /* on-disk format version      */

/* Status codes. */
typedef enum sl_reg_status {
    SL_REG_OK          =  0,
    SL_REG_INVALID     = -1,   /* bad argument / null handle                  */
    SL_REG_NOT_FOUND   = -2,   /* key or value not found                      */
    SL_REG_EXISTS      = -3,   /* key/value already exists (where unique)     */
    SL_REG_LIMIT       = -4,   /* a bounded table was full                    */
    SL_REG_MALFORMED   = -5,   /* a store file could not be parsed            */
    SL_REG_SEAL_BROKEN = -6    /* an originality seal failed verification     */
} sl_reg_status;

/* Value kinds a registry value can hold. */
typedef enum sl_reg_kind {
    SL_REG_STRING = 0,
    SL_REG_INT    = 1,
    SL_REG_BOOL   = 2
} sl_reg_kind;

/*
 * Provenance of a tracked registry artifact. Every field is recorded so an
 * entry is self-describing and verifiable:
 *   - name     : the file name the entry tracks
 *   - date     : ISO-8601 date/time the entry was recorded
 *   - author   : the authoring party
 *   - version  : the artifact's version string
 *   - hex_id   : a stable hex identity for the entry (16 hex chars)
 *   - hash     : SHA-256 of the tracked file's content (64 hex chars)
 *   - seal     : originality seal = SHA-256 over (hex_id | version | hash |
 *                author), proving the identity and content were sealed together
 */
typedef struct sl_reg_provenance {
    char name[256];
    char date[32];
    char author[128];
    char version[64];
    char hex_id[17];     /* 16 hex + NUL */
    char hash[65];       /* 64 hex + NUL */
    char seal[65];       /* 64 hex + NUL */
} sl_reg_provenance;

/* Opaque registry handle (owns the key tree + values + provenance). */
typedef struct sl_registry sl_registry;

/* ---- lifecycle -------------------------------------------------------- */

sl_registry *sl_reg_create(void);
void         sl_reg_destroy(sl_registry *r);

/* Seed the baseline registry a fresh install starts from (root keys for
 * System, Settings, Features, Artifacts). Idempotent. */
sl_reg_status sl_reg_seed_defaults(sl_registry *r);

/* ---- keys ------------------------------------------------------------- */

/* Create (or no-op if present) a key at `path` ("A/B/C"); parents are made. */
sl_reg_status sl_reg_key_create(sl_registry *r, const char *path);
int           sl_reg_key_exists(const sl_registry *r, const char *path);
/* Number of direct child keys of `path`; fills names[] (each up to cap). */
int           sl_reg_key_children(const sl_registry *r, const char *path,
                                  char **names, int max, size_t name_cap);

/* ---- values (settings / features) ------------------------------------- */

sl_reg_status sl_reg_set_string(sl_registry *r, const char *path, const char *name, const char *value);
sl_reg_status sl_reg_set_int(sl_registry *r, const char *path, const char *name, long value);
sl_reg_status sl_reg_set_bool(sl_registry *r, const char *path, const char *name, int value);

/* Read a value; returns the kind via *kind. String read copies into buf. */
sl_reg_status sl_reg_get_string(const sl_registry *r, const char *path, const char *name, char *buf, size_t cap);
sl_reg_status sl_reg_get_int(const sl_registry *r, const char *path, const char *name, long *out);
sl_reg_status sl_reg_get_bool(const sl_registry *r, const char *path, const char *name, int *out);
sl_reg_status sl_reg_value_kind(const sl_registry *r, const char *path, const char *name, sl_reg_kind *kind);

/* A convenience for feature flags: Features/<name> = bool. */
sl_reg_status sl_reg_feature_set(sl_registry *r, const char *name, int enabled);
int           sl_reg_feature_enabled(const sl_registry *r, const char *name);

/* ---- provenance + originality seal ------------------------------------ */

/* Compute the originality seal for a provenance record (fills p->seal from
 * hex_id|version|hash|author). Deterministic. */
void sl_reg_seal(sl_reg_provenance *p);

/* Verify a record's seal matches its fields (1 = sealed/intact, 0 = broken). */
int  sl_reg_seal_verify(const sl_reg_provenance *p);

/* Attach a provenance record to a key (sealing it if the seal is empty). */
sl_reg_status sl_reg_provenance_set(sl_registry *r, const char *path, const sl_reg_provenance *p);
/* Read a key's provenance record. */
sl_reg_status sl_reg_provenance_get(const sl_registry *r, const char *path, sl_reg_provenance *out);

/* SHA-256 of a buffer, as 64 lowercase hex chars into out (>=65 bytes). */
void sl_reg_sha256_hex(const void *data, size_t len, char *out);

/* ---- persistence (portable, deterministic text store) ----------------- */

/* Save the whole registry to `path` in the .sleelareg text format (sorted keys
 * -> idempotent). Load replaces the current contents. */
sl_reg_status sl_reg_save(const sl_registry *r, const char *path);
sl_reg_status sl_reg_load(sl_registry *r, const char *path);

/* ---- SQL export ------------------------------------------------------- */

/* Export the whole registry as a portable MySQL/MariaDB script (CREATE TABLE
 * registry_entries + deterministic INSERTs), so settings/features/provenance
 * are queryable from a database as well as the .sleelareg text store. Rows are
 * sorted (idempotent). Writes to `path`; loads with `mysql <db> < file`. */
sl_reg_status sl_reg_export_sql(const sl_registry *r, const char *path);

/* ---- review ----------------------------------------------------------- */

/* Render a human-readable review of the whole tree (or a subtree at `path`,
 * NULL = root) into buf; snprintf semantics. Shows keys, values, and for keys
 * with provenance the file/author/version/hexid/hash and seal status. */
long sl_reg_review(const sl_registry *r, const char *path, char *buf, size_t cap);

/* Counts for a quick status line. */
int sl_reg_key_count(const sl_registry *r);
int sl_reg_value_count(const sl_registry *r);
int sl_reg_provenance_count(const sl_registry *r);

const char *sl_reg_status_name(sl_reg_status s);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_REGISTRY_H */
