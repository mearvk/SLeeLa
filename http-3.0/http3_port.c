#include "http3_port.h"
#include <string.h>

static const uint8_t k_max[HTTP3_PORT_BYTES] = {
    0xaf,0x29,0x8d,0x05,0x0e,0x43,0x95,0xd6,0x96,0x70,
    0xb1,0x2b,0x7f,0x40,0xff,0xff,0xff,0xff,0xff,0xff
};

static int decimal_valid(const char *s) {
    size_t i, n;
    if (!s) return 0;
    n = strlen(s);
    if (n == 0 || n > 48u) return 0;
    for (i = 0; i < n; ++i)
        if (s[i] < '0' || s[i] > '9') return 0;
    return 1;
}

static int mul10_add(http3_port_t *p, unsigned digit) {
    int i;
    unsigned carry = digit;
    for (i = (int)HTTP3_PORT_BYTES - 1; i >= 0; --i) {
        unsigned v = (unsigned)p->bytes[i] * 10u + carry;
        p->bytes[i] = (uint8_t)v;
        carry = v >> 8;
    }
    return carry == 0u;
}

void http3_port_zero(http3_port_t *port) {
    if (port) memset(port, 0, sizeof(*port));
}

void http3_port_from_u64(http3_port_t *port, uint64_t value) {
    int i;
    if (!port) return;
    memset(port, 0, sizeof(*port));
    for (i = 0; i < 8; ++i) {
        port->bytes[HTTP3_PORT_BYTES - 1u - (unsigned)i] = (uint8_t)(value & 0xffu);
        value >>= 8;
    }
}

int http3_port_compare(const http3_port_t *a, const http3_port_t *b) {
    int i;
    if (!a || !b) return 0;
    for (i = 0; i < (int)HTTP3_PORT_BYTES; ++i) {
        if (a->bytes[i] < b->bytes[i]) return -1;
        if (a->bytes[i] > b->bytes[i]) return 1;
    }
    return 0;
}

int http3_port_is_valid(const http3_port_t *port) {
    return port != NULL && http3_port_compare(port, (const http3_port_t *)&(http3_port_t){ { 
        0xaf,0x29,0x8d,0x05,0x0e,0x43,0x95,0xd6,0x96,0x70,
        0xb1,0x2b,0x7f,0x40,0xff,0xff,0xff,0xff,0xff,0xff
    } }) <= 0;
}

int http3_port_from_decimal(http3_port_t *port, const char *decimal) {
    size_t i = 0;
    http3_port_t value;
    if (!port || !decimal_valid(decimal)) return -1;
    while (decimal[i] == '0' && decimal[i + 1u] != '\0') ++i;
    memset(&value, 0, sizeof(value));
    for (; decimal[i] != '\0'; ++i)
        if (!mul10_add(&value, (unsigned)(decimal[i] - '0'))) return -1;
    if (http3_port_compare(&value, (const http3_port_t *)&(http3_port_t){ { 
        0xaf,0x29,0x8d,0x05,0x0e,0x43,0x95,0xd6,0x96,0x70,
        0xb1,0x2b,0x7f,0x40,0xff,0xff,0xff,0xff,0xff,0xff
    } }) > 0) return -1;
    *port = value;
    return 0;
}

int http3_port_to_decimal(const http3_port_t *port, char *out, size_t out_size) {
    char digits[49];
    size_t len = 0, i;
    http3_port_t value;
    if (!port || !out || !http3_port_is_valid(port)) return -1;
    value = *port;
    do {
        unsigned remainder = 0;
        int j;
        for (j = 0; j < (int)HTTP3_PORT_BYTES; ++j) {
            unsigned cur = (remainder << 8) | value.bytes[j];
            value.bytes[j] = (uint8_t)(cur / 10u);
            remainder = cur % 10u;
        }
        if (len >= sizeof(digits)) return -1;
        digits[len++] = (char)('0' + remainder);
        for (j = 0; j < (int)HTTP3_PORT_BYTES; ++j)
            if (value.bytes[j] != 0) break;
        if (j == (int)HTTP3_PORT_BYTES) break;
    } while (len < sizeof(digits));
    if (len + 1u > out_size) return -1;
    for (i = 0; i < len; ++i) out[i] = digits[len - 1u - i];
    out[len] = '\0';
    return 0;
}

int http3_port_increment(http3_port_t *port) {
    int i;
    if (!port || !http3_port_is_valid(port)) return -1;
    if (http3_port_compare(port, (const http3_port_t *)&(http3_port_t){ { 
        0xaf,0x29,0x8d,0x05,0x0e,0x43,0x95,0xd6,0x96,0x70,
        0xb1,0x2b,0x7f,0x40,0xff,0xff,0xff,0xff,0xff,0xff
    } }) == 0) return -1;
    for (i = (int)HTTP3_PORT_BYTES - 1; i >= 0; --i) {
        if (++port->bytes[i] != 0u) return 0;
    }
    return -1;
}

void http3_port_encode(const http3_port_t *port, uint8_t out[HTTP3_PORT_BYTES]) {
    if (port && out) memcpy(out, port->bytes, HTTP3_PORT_BYTES);
}
void http3_port_decode(http3_port_t *port, const uint8_t in[HTTP3_PORT_BYTES]) {
    if (port && in) memcpy(port->bytes, in, HTTP3_PORT_BYTES);
}
