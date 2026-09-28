/* ==========================================================================
 * http3_handshake.h -- SLeeLa HTTP 3.0 capability-negotiation handshake.
 *
 * Two peers (a client and a router/server) each advertise the set of protocol
 * capability LEVELS they support, as a neutral bitmask. The handshake then
 * deterministically selects the HIGHEST COMMON LEVEL both peers support.
 *
 * Levels are cumulative delivery-efficiency tiers (see QOS.md / the timing
 * work): each higher level implies the ones below it.
 *
 *   L1 BASELINE  : envelope + integrity gate only (always supported).
 *   L2 PER_LEG   : per-leg MSS / path-MTU sizing for goodput.
 *   L3 PACING    : timing-aware pacing (max-speed / balance).
 *   L4 ECHO      : echo-acknowledged, deadline-aware delivery.
 *
 * A peer whose software is not yet updated advertises only the levels it knows
 * (at minimum L1). The negotiation therefore lets an older router run the
 * MODEST BASELINE and transparently move up once its software is apt to update
 * and it advertises higher levels -- no flag day, no identity, just the highest
 * level both ends actually support.
 *
 * The 4-byte capability offer is covered by the connection's keyed MAC
 * (http3_mac.h): a peer cannot have its advertised capabilities silently
 * downgraded/upgraded in flight without invalidating the tag.
 *
 * The capability value is a plain bitmask. It carries NO national, geographic,
 * or identity meaning -- it names supported protocol tiers only.
 *
 * Pure data: no I/O, no allocation.
 * ========================================================================== */
#ifndef HTTP3_HANDSHAKE_H
#define HTTP3_HANDSHAKE_H

#include <stddef.h>
#include <stdint.h>

#include "http3_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Capability levels as bit flags (cumulative tiers). */
typedef enum {
    HTTP3_CAP_NONE     = 0x00,
    HTTP3_CAP_L1_BASE  = 0x01, /* baseline: envelope + integrity gate       */
    HTTP3_CAP_L2_PERLEG= 0x02, /* per-leg MSS / path-MTU sizing             */
    HTTP3_CAP_L3_PACING= 0x04, /* timing-aware pacing (max-speed/balance)   */
    HTTP3_CAP_L4_ECHO  = 0x08  /* echo-acknowledged, deadline-aware delivery*/
} http3_cap_flag_t;

/* The baseline every conforming peer must support. */
#define HTTP3_CAP_BASELINE HTTP3_CAP_L1_BASE

/* Full set a fully-updated peer advertises (L1..L4). */
#define HTTP3_CAP_ALL \
    (HTTP3_CAP_L1_BASE | HTTP3_CAP_L2_PERLEG | HTTP3_CAP_L3_PACING | HTTP3_CAP_L4_ECHO)

/*
 * A peer's capability offer: the 32-bit bitmask it advertises, plus a MAC tag
 * over the offer (computed under the shared connection key).
 */
typedef struct {
    uint32_t capabilities; /* OR of http3_cap_flag_t the peer supports */
    uint64_t mac;          /* keyed MAC over the canonical offer bytes  */
} http3_cap_offer_t;

/*
 * Build a capability offer: normalizes `capabilities` to always include the
 * baseline (L1), then stamps the MAC under `key`. Returns 0 on success, -1 on a
 * NULL argument.
 */
int http3_handshake_make_offer(http3_cap_offer_t *offer,
                               uint32_t capabilities,
                               const uint8_t key[HTTP3_MAC_KEY_BYTES]);

/* Verify an offer's MAC under `key`. Returns 1 if authentic, 0 otherwise. */
int http3_handshake_verify_offer(const http3_cap_offer_t *offer,
                                 const uint8_t key[HTTP3_MAC_KEY_BYTES]);

/*
 * Deterministically select the negotiated level from two offers: the HIGHEST
 * single level present in BOTH peers' capability sets (levels are cumulative,
 * so the result is one of HTTP3_CAP_L1_BASE..L4_ECHO). Because every conforming
 * peer includes the baseline, the result is at least L1 -- an older peer simply
 * caps the pair at the modest baseline until it advertises more.
 *
 * Returns the selected level flag, or HTTP3_CAP_NONE only if a peer failed to
 * include the baseline (non-conforming).
 */
uint32_t http3_handshake_negotiate(uint32_t local_caps, uint32_t remote_caps);

/*
 * Full handshake step over two received offers: verify BOTH MACs under `key`,
 * then negotiate. Writes the agreed level to *agreed. Returns 0 on success, -1
 * if either MAC fails (agreed set to HTTP3_CAP_NONE) or on a NULL argument.
 */
int http3_handshake_resolve(const http3_cap_offer_t *local,
                            const http3_cap_offer_t *remote,
                            const uint8_t key[HTTP3_MAC_KEY_BYTES],
                            uint32_t *agreed);

/* Human-readable level name (e.g. "L3_PACING") for logs/diagnostics. */
const char *http3_handshake_level_name(uint32_t level);

#ifdef __cplusplus
}
#endif

#endif /* HTTP3_HANDSHAKE_H */
