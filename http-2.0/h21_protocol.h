/* ==========================================================================
 * h21_protocol.h -- aggregation header for the SLeeLa HTTP 2.1 application
 * protocol core (SKETCH): the compact envelope, fast naming, the response
 * model, retry classes, and the processing pipeline.
 *
 * HTTP 2.1 shares HTTP 3.0's basic design goals but is the earlier generation:
 * it carries the clean core only and has no per-packet integrity substrate
 * (no keyed-MAC DIGEST, INTACTX, NONCE, or goods/services BASKET). Like the 3.0
 * core, it is pure data and carries no OpenSSL dependency.
 * ========================================================================== */
#ifndef SLEELA_H21_PROTOCOL_H
#define SLEELA_H21_PROTOCOL_H

#include "h21_envelope.h"
#include "h21_naming.h"
#include "h21_pipeline.h"

#endif
