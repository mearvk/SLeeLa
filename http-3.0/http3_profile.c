/* ==========================================================================
 * http3_profile.c -- HTTP 3.0 "colors": the shipped palette, name resolver,
 * config-file loader, and pipeline application.
 * ========================================================================== */
#include "http3_profile.h"
#include "http3_pipeline.h"
#include "http3_intactx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* The shipped palette. Kept in sync with http3_colors.conf, http3_flow.py, and
 * HTTP-COLORS.md. */
static const http3_profile_t k_palette[HTTP3_COLOR_COUNT] = {
    /* name     wire               flags                             mac intactx thr   replay basket */
    { "green", HTTP3_WIRE_TEXT,   HTTP3_FLAG_NONE,                   0,  0,      0u,   0,     0 },
    { "amber", HTTP3_WIRE_TEXT,   HTTP3_FLAG_NONE,                   1,  0,      0u,   1,     0 },
    { "red",   HTTP3_WIRE_BINARY, HTTP3_FLAG_BINARY,                 1,  1,      0u,   1,     1 },
    { "black", HTTP3_WIRE_BINARY, (uint8_t)(HTTP3_FLAG_BINARY |
                                             HTTP3_FLAG_COMPRESSED), 1,  1,      0u,   1,     1 }
};

const http3_profile_t *http3_profile_palette(size_t i)
{
    if (i >= HTTP3_COLOR_COUNT) {
        return NULL;
    }
    return &k_palette[i];
}

int http3_profile_by_color(const char *color, http3_profile_t *out)
{
    size_t i;
    if (color == NULL || out == NULL) {
        return -1;
    }
    for (i = 0; i < HTTP3_COLOR_COUNT; ++i) {
        if (strncmp(color, k_palette[i].name, HTTP3_COLOR_NAME_MAX) == 0) {
            *out = k_palette[i];
            return 0;
        }
    }
    return -1;
}

/* ---- Config-file loader --------------------------------------------------
 * Line format (see HTTP-COLORS.md), one color per line, '#' comments allowed:
 *   <name> <wire> <flags> <mac> <intactx> <threshold> <replay> <basket>
 * where wire is "text"/"binary", flags is a decimal bitfield, and the rest are
 * 0/1 (threshold is decimal, 0 => library default).
 */
static int parse_wire(const char *tok, http3_wire_form_t *w)
{
    if (strcmp(tok, "text") == 0)   { *w = HTTP3_WIRE_TEXT;   return 0; }
    if (strcmp(tok, "binary") == 0) { *w = HTTP3_WIRE_BINARY; return 0; }
    return -1;
}

int http3_profile_load(const char *path, const char *color, http3_profile_t *out)
{
    FILE *f;
    char line[256];
    if (color == NULL || out == NULL) {
        return -1;
    }
    if (path == NULL) {
        path = HTTP3_COLORS_DEFAULT_PATH;
    }
    f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        char name[HTTP3_COLOR_NAME_MAX];
        char wire[16];
        unsigned flags, mac, intactx, replay, basket;
        unsigned threshold;
        int n;
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
            continue;
        }
        n = sscanf(line, "%23s %15s %u %u %u %u %u %u",
                   name, wire, &flags, &mac, &intactx, &threshold, &replay, &basket);
        if (n != 8) {
            continue; /* skip malformed lines */
        }
        if (strncmp(name, color, HTTP3_COLOR_NAME_MAX) != 0) {
            continue;
        }
        memset(out, 0, sizeof(*out));
        /* `name` came from sscanf "%23s", so it is NUL-terminated and at most
         * 23 chars; a bounded memcpy avoids strncpy truncation diagnostics. */
        {
            size_t nlen = strlen(name);
            if (nlen > HTTP3_COLOR_NAME_MAX - 1) {
                nlen = HTTP3_COLOR_NAME_MAX - 1;
            }
            memcpy(out->name, name, nlen);
            out->name[nlen] = '\0';
        }
        if (parse_wire(wire, &out->wire) != 0) {
            fclose(f);
            return -1;
        }
        out->flags = (uint8_t)flags;
        out->mac_required = mac ? 1 : 0;
        out->intactx_enabled = intactx ? 1 : 0;
        out->intactx_threshold = (uint16_t)threshold;
        out->replay_guard = replay ? 1 : 0;
        out->basket_required = basket ? 1 : 0;
        fclose(f);
        return 0;
    }
    fclose(f);
    return -1; /* color not found in file */
}

int http3_profile_apply(const http3_profile_t *profile, struct http3_pipeline *pipe)
{
    http3_pipeline_t *p = (http3_pipeline_t *)pipe;
    if (profile == NULL || p == NULL) {
        return -1;
    }
    /* Only the INTACTX threshold is pipeline state a color influences here; the
     * MAC key is installed separately, and the color's other switches guide the
     * caller's send/verify choices. */
    if (profile->intactx_enabled) {
        http3_pipeline_set_intactx_threshold(
            p, profile->intactx_threshold /* 0 => library default */);
    }
    return 0;
}
