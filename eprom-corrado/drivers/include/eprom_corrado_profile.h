/*
 * eprom_corrado_profile.h
 *
 * SLeeLa-side device profile for the eprom-corrado driver family: the EPROM
 * programmer hardware identity and image mappings, recorded as a header so the
 * family's facts live next to SLeeLa's /drivers architecture.
 *
 * This is a REFERENCE record, congruent with the authoritative implementation
 * in the mearvk/Corrado repository (corrado_eprom.h / corrado_usb.h). It does
 * not re-implement the driver; it records the mappings SLeeLa recognises for
 * this family. Values are marked Actual / Provisional / Unknown per
 * TERMINOLOGY.md; see docs/COMPLETENESS.md.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef SLEELA_EPROM_CORRADO_PROFILE_H
#define SLEELA_EPROM_CORRADO_PROFILE_H

#include <stdint.h>
#include <stddef.h>

/* ---- Programmer identity (Actual: USB IDs) ---------------------------- */
#define EPROM_TL866A_VID   0x04D8u   /* Microchip */
#define EPROM_TL866A_PID   0xE11Cu
#define EPROM_TL866II_VID  0xA466u
#define EPROM_TL866II_PID  0x0A53u

/* Bulk endpoints (Actual). */
#define EPROM_EP_OUT       0x01u
#define EPROM_EP_IN        0x81u

/* Default transfer timeout, ms (Standard/Expected Default). */
#define EPROM_TIMEOUT_MS   5000

/* ---- Target devices (Actual sizes) ------------------------------------ */
typedef enum {
    EPROM_DEV_27C128 = 0,   /* 16384 bytes - early/alternate ROMs      */
    EPROM_DEV_27C256,       /* 32768 bytes - standard Corrado ECU chip */
    EPROM_DEV_27C512,       /* 65536 bytes - twin-tune / 512 adapters  */
    EPROM_DEV_UNKNOWN
} eprom_device_t;

static inline size_t eprom_device_size(eprom_device_t d)
{
    switch (d) {
        case EPROM_DEV_27C128: return 16384u;
        case EPROM_DEV_27C256: return 32768u;
        case EPROM_DEV_27C512: return 65536u;
        default:               return 0u;
    }
}

/* ---- Image conventions (Actual) --------------------------------------- */
/* An erased EPROM cell reads as 1, so a blank image is all 0xFF. */
#define EPROM_BLANK_BYTE   0xFFu
/* Digifant/Motronic-style trailing checksum word: last two bytes,
 * little-endian, chosen so the 16-bit word reconciliation verifies. */
#define EPROM_CHECKSUM_WORD_BYTES 2u

/* ---- Erase capability (Actual: capability class, not a fixed value) --- */
/* A genuine 27C-series part is UV-erasable / often one-time-programmable and
 * CANNOT be erased electrically; reusable pin-compatible replacements can. */
typedef enum {
    EPROM_ERASE_UV_ONLY = 0,   /* genuine 27C: electrical erase UNSUPPORTED */
    EPROM_ERASE_ELECTRICAL     /* reusable replacement: electrical erase OK  */
} eprom_erase_class_t;

#endif /* SLEELA_EPROM_CORRADO_PROFILE_H */
