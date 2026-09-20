/* ==========================================================================
 * http3_basket.c -- the fixed HTTP 3.0 basket table and canonical (de)serialize.
 *
 * GENERATED DATA: the 14 items below were selected once from a fixed seed
 * (0xcab17a115) and frozen; the same values appear in BASKET.docx and in the Python
 * reference (http3_flow.py). Do not edit by hand -- regenerate all three
 * together if the basket ever changes.
 * ========================================================================== */
#include "http3_basket.h"

#include <string.h>

static const http3_basket_item_t k_basket[HTTP3_BASKET_ITEMS] = {
    { 1u, 56249759ull, "Bleached pulp paper" },
    { 2u, 34243099ull, "Broadband gigabyte (svc)" },
    { 3u, 69987683ull, "Cane sugar" },
    { 4u, 43104945ull, "Cobalt metal" },
    { 5u, 29864648ull, "Cured tobacco leaf" },
    { 6u, 17981345ull, "Freight ton-mile (svc)" },
    { 7u, 6610716ull, "Gold bullion" },
    { 8u, 12259381ull, "Legal counsel hour (svc)" },
    { 9u, 65041790ull, "Managed cloud-compute (svc)" },
    { 10u, 31566795ull, "Natural rubber" },
    { 11u, 24592204ull, "Portland cement" },
    { 12u, 65211966ull, "Raw cotton" },
    { 13u, 35009362ull, "Roasted coffee" },
    { 14u, 35480545ull, "Structural steel" },
};

const http3_basket_item_t *http3_basket_table(void)
{
    return k_basket;
}

static void put_u16(uint8_t *b, uint16_t v)
{
    b[0] = (uint8_t)(v >> 8); b[1] = (uint8_t)v;
}
static void put_u32(uint8_t *b, uint32_t v)
{
    b[0] = (uint8_t)(v >> 24); b[1] = (uint8_t)(v >> 16);
    b[2] = (uint8_t)(v >> 8);  b[3] = (uint8_t)v;
}
static void put_u64(uint8_t *b, uint64_t v)
{
    b[0] = (uint8_t)(v >> 56); b[1] = (uint8_t)(v >> 48);
    b[2] = (uint8_t)(v >> 40); b[3] = (uint8_t)(v >> 32);
    b[4] = (uint8_t)(v >> 24); b[5] = (uint8_t)(v >> 16);
    b[6] = (uint8_t)(v >> 8);  b[7] = (uint8_t)v;
}
static uint16_t get_u16(const uint8_t *b)
{
    return (uint16_t)(((uint16_t)b[0] << 8) | (uint16_t)b[1]);
}
static uint32_t get_u32(const uint8_t *b)
{
    return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) |
           ((uint32_t)b[2] << 8)  |  (uint32_t)b[3];
}
static uint64_t get_u64(const uint8_t *b)
{
    return ((uint64_t)b[0] << 56) | ((uint64_t)b[1] << 48) |
           ((uint64_t)b[2] << 40) | ((uint64_t)b[3] << 32) |
           ((uint64_t)b[4] << 24) | ((uint64_t)b[5] << 16) |
           ((uint64_t)b[6] << 8)  |  (uint64_t)b[7];
}

size_t http3_basket_serialize(uint8_t *out, size_t out_cap)
{
    size_t i, pos;
    if (out == NULL || out_cap < HTTP3_BASKET_BLOCK_SIZE) {
        return 0;
    }
    put_u16(out, (uint16_t)HTTP3_BASKET_ISO_NUMERIC);
    put_u16(out + 2, (uint16_t)HTTP3_BASKET_ITEMS);
    pos = 4;
    for (i = 0; i < HTTP3_BASKET_ITEMS; ++i) {
        put_u32(out + pos, k_basket[i].atomic_number); pos += 4;
        put_u64(out + pos, k_basket[i].value_ugram);   pos += 8;
    }
    return pos;
}

size_t http3_basket_parse(const uint8_t *in, size_t in_len,
                          http3_basket_item_t *items, size_t cap,
                          uint16_t *iso_numeric)
{
    uint16_t count;
    size_t i, pos;
    if (in == NULL || in_len < 4u) {
        return 0;
    }
    if (iso_numeric != NULL) {
        *iso_numeric = get_u16(in);
    }
    count = get_u16(in + 2);
    if (in_len < (size_t)4u + (size_t)count * 12u) {
        return 0;
    }
    pos = 4;
    for (i = 0; i < count; ++i) {
        if (items != NULL && i < cap) {
            items[i].atomic_number = get_u32(in + pos);
            items[i].value_ugram   = get_u64(in + pos + 4);
            items[i].name[0] = '\0';
        }
        pos += 12;
    }
    return count;
}

uint64_t http3_basket_total_ugram(void)
{
    uint64_t total = 0;
    size_t i;
    for (i = 0; i < HTTP3_BASKET_ITEMS; ++i) {
        total += k_basket[i].value_ugram;
    }
    return total;
}
