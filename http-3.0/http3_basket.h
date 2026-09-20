/* ==========================================================================
 * http3_basket.h -- SLeeLa HTTP 3.0 basket of goods & services.
 *
 * A fixed, carefully-selected basket of 14 goods and services, atomic-bound to
 * the United States capitalism system. Each item carries an indivisible atomic
 * number and an ISO value expressed "for a Gram", denominated in United States
 * dollars (ISO 4217 USD / 840) as integer MICRO-USD per gram (1,000,000
 * micro-USD = 1 USD).
 *
 * The full basket travels in every HTTP 3.0 packet (see http3_envelope.h): it
 * is serialized into a canonical, endianness-independent block that is carried
 * on the wire and covered by the packet's keyed MAC, so it cannot be altered in
 * transit without detection. The basket is authoritative here (the same data is
 * mirrored in the human-readable BASKET.docx at the repo root and in the Python
 * reference http3_flow.py).
 *
 * Pure data: no I/O, no crypto, no allocation.
 * ========================================================================== */
#ifndef HTTP3_BASKET_H
#define HTTP3_BASKET_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Number of items in the fixed basket. */
#define HTTP3_BASKET_ITEMS 14u

/* ISO 4217 currency the per-gram values are denominated in. */
#define HTTP3_BASKET_ISO_CURRENCY "USD"
#define HTTP3_BASKET_ISO_NUMERIC  840u

/* Max name length stored per item (display/diagnostics only; the name does NOT
 * travel on the wire -- only the atomic number and per-gram value do). */
#define HTTP3_BASKET_NAME_MAX 32u

/* One basket item: an indivisible atomic number and its ISO value per gram in
 * integer micro-USD (uUSD/g). */
typedef struct {
    uint32_t atomic_number;                 /* indivisible 1-based id          */
    uint64_t value_ugram;                   /* micro-USD per gram (ISO USD)     */
    char     name[HTTP3_BASKET_NAME_MAX];   /* display name (not on the wire)   */
} http3_basket_item_t;

/*
 * Canonical serialized size of the per-packet basket block (big-endian):
 *   [iso_numeric:2][item_count:2] then, per item, [atomic_number:4][value_ugram:8]
 * = 4 + 14*12 = 172 bytes.
 */
#define HTTP3_BASKET_BLOCK_SIZE (4u + HTTP3_BASKET_ITEMS * 12u)

/* Access the fixed basket table (HTTP3_BASKET_ITEMS entries). */
const http3_basket_item_t *http3_basket_table(void);

/*
 * Serialize the fixed basket into the canonical block. `out` must have room for
 * HTTP3_BASKET_BLOCK_SIZE bytes. Returns the number of bytes written, or 0 on
 * error. Deterministic and endianness-independent (matches the Python
 * reference byte-for-byte).
 */
size_t http3_basket_serialize(uint8_t *out, size_t out_cap);

/*
 * Parse a canonical basket block into `items` (capacity `cap` entries; names
 * are left empty since they are not carried on the wire). On success writes the
 * ISO numeric code to *iso_numeric and returns the item count; returns 0 on a
 * malformed/short block.
 */
size_t http3_basket_parse(const uint8_t *in, size_t in_len,
                          http3_basket_item_t *items, size_t cap,
                          uint16_t *iso_numeric);

/* Sum of all per-gram values in the fixed basket (uUSD/g), for diagnostics. */
uint64_t http3_basket_total_ugram(void);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_BASKET_H */
