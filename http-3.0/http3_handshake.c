/* ==========================================================================
 * http3_handshake.c -- capability-negotiation handshake: highest-common-level
 * selection with a baseline-until-upgrade fallback, MAC-backed offers.
 * ========================================================================== */
#include "http3_handshake.h"

#include <string.h>

/* Canonical offer bytes for the MAC: a 4-byte big-endian capability bitmask,
 * prefixed with a fixed domain tag so an offer MAC can never be confused with
 * an envelope MAC under the same key. */
static uint64_t offer_mac(uint32_t caps, const uint8_t key[HTTP3_MAC_KEY_BYTES])
{
    uint8_t msg[8];
    /* domain tag "H3CAP" folded into 4 bytes, then the caps big-endian. */
    msg[0] = 'H'; msg[1] = '3'; msg[2] = 'C'; msg[3] = 'P';
    msg[4] = (uint8_t)(caps >> 24);
    msg[5] = (uint8_t)(caps >> 16);
    msg[6] = (uint8_t)(caps >> 8);
    msg[7] = (uint8_t)(caps);
    return http3_mac_siphash24(key, msg, sizeof(msg));
}

int http3_handshake_make_offer(http3_cap_offer_t *offer,
                               uint32_t capabilities,
                               const uint8_t key[HTTP3_MAC_KEY_BYTES])
{
    if (offer == NULL || key == NULL) {
        return -1;
    }
    /* A conforming peer always includes the baseline. */
    capabilities |= HTTP3_CAP_BASELINE;
    offer->capabilities = capabilities;
    offer->mac = offer_mac(capabilities, key);
    return 0;
}

int http3_handshake_verify_offer(const http3_cap_offer_t *offer,
                                 const uint8_t key[HTTP3_MAC_KEY_BYTES])
{
    if (offer == NULL || key == NULL) {
        return 0;
    }
    return offer_mac(offer->capabilities, key) == offer->mac ? 1 : 0;
}

uint32_t http3_handshake_negotiate(uint32_t local_caps, uint32_t remote_caps)
{
    uint32_t common = local_caps & remote_caps;
    /* Non-conforming if either side lacks the baseline. */
    if ((common & HTTP3_CAP_BASELINE) == 0u) {
        return HTTP3_CAP_NONE;
    }
    /* Highest single level present in both (levels are cumulative tiers). */
    if (common & HTTP3_CAP_L4_ECHO)   { return HTTP3_CAP_L4_ECHO; }
    if (common & HTTP3_CAP_L3_PACING) { return HTTP3_CAP_L3_PACING; }
    if (common & HTTP3_CAP_L2_PERLEG) { return HTTP3_CAP_L2_PERLEG; }
    return HTTP3_CAP_L1_BASE; /* modest baseline fallback */
}

int http3_handshake_resolve(const http3_cap_offer_t *local,
                            const http3_cap_offer_t *remote,
                            const uint8_t key[HTTP3_MAC_KEY_BYTES],
                            uint32_t *agreed)
{
    if (local == NULL || remote == NULL || key == NULL || agreed == NULL) {
        return -1;
    }
    if (!http3_handshake_verify_offer(local, key) ||
        !http3_handshake_verify_offer(remote, key)) {
        *agreed = HTTP3_CAP_NONE;
        return -1;
    }
    *agreed = http3_handshake_negotiate(local->capabilities, remote->capabilities);
    return 0;
}

const char *http3_handshake_level_name(uint32_t level)
{
    switch (level) {
        case HTTP3_CAP_NONE:      return "NONE";
        case HTTP3_CAP_L1_BASE:   return "L1_BASELINE";
        case HTTP3_CAP_L2_PERLEG: return "L2_PERLEG";
        case HTTP3_CAP_L3_PACING: return "L3_PACING";
        case HTTP3_CAP_L4_ECHO:   return "L4_ECHO";
        default:                  return "MIXED";
    }
}
