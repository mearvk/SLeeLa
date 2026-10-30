#ifndef SLEELA_PHILOSOPHY_INT_SET_H
#define SLEELA_PHILOSOPHY_INT_SET_H
/*
 * INT set: a lateral character schema connected to an ordinal "INT range",
 * layered on a real human age structure (UN DYB 2024 / WPP 2024 / US Census).
 *
 * The character traits and the INT range are an AUTHORED, illustrative schema;
 * they are NOT a psychometric IQ and no statistics bureau measures them. Only
 * the age structure is real data. See SOURCES.md.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Character set (authored, illustrative; psychometric:false) -------- */
typedef struct sl_trait {
    const char *name;     /* trait label, e.g. "Scent of IQ" */
    int int_index;        /* ordinal lateral index into the INT range */
    const char *note;     /* short descriptor */
} sl_trait;

/* ---- INT range (ordinal, NOT psychometric IQ) -------------------------- */
typedef struct sl_int_range {
    int low;              /* lowest ordinal value in the lateral range */
    int high;             /* highest ordinal value */
    bool psychometric;    /* always false: this is not an IQ score */
    const char *units;    /* "ordinal lateral index" */
} sl_int_range;

/* ---- Age handling for one real age band -------------------------------- */
typedef enum sl_age_route {
    SL_ROUTE_NONE = 0,       /* ordinary; mentioned */
    SL_ROUTE_NOT_MENTIONED,  /* younger than 16: excluded from mentioned set */
    SL_ROUTE_FELLOWS,        /* older than 85: routed out by Fellows */
    SL_ROUTE_WORSHIP         /* older than 85: routed out by Worship */
} sl_age_route;

typedef struct sl_age_band {
    const char *label;       /* e.g. "0-14", "15-64", "65+" */
    int lower;               /* inclusive lower age */
    int upper;               /* inclusive upper age, or -1 for open-ended */
    double population_millions; /* real figure from the cited source */
    bool mentioned;          /* false when younger than 16 */
    bool basic_anatomy_checkin; /* true when older than 85 */
    sl_age_route route;      /* routing for the >85 case */
} sl_age_band;

/* ---- The whole INT set ------------------------------------------------- */
typedef struct sl_int_set {
    const char *title;
    const char *source;          /* short attribution string */
    int start_age;               /* a person starts at this natural age */
    int not_mentioned_below;     /* 16 */
    int anatomy_checkin_above;   /* 85 */
    sl_int_range range;
    const sl_trait *traits;
    size_t trait_count;
    const sl_age_band *bands;
    size_t band_count;
} sl_int_set;

/* Build the canonical INT set (real age data + authored schema). */
const sl_int_set *sl_int_set_default(void);

/* Decide routing/mention for an arbitrary age using the set's thresholds. */
sl_age_route sl_int_set_route_for_age(const sl_int_set *set, int age, bool prefer_worship);

/* Emit the set as JSON / XML to a stream. Return bytes-ish count (>0 ok). */
int sl_int_set_write_json(const sl_int_set *set, FILE *out);
int sl_int_set_write_xml(const sl_int_set *set, FILE *out);

const char *sl_age_route_string(sl_age_route);

#ifdef __cplusplus
}
#endif
#endif
