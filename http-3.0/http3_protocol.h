/* ==========================================================================
 * http3_protocol.h -- aggregation header for the SLeeLa HTTP 3.0 application
 * protocol core: the compact envelope (§5), fast naming (§4), the response
 * model (§7), retry classes (§9), and the §19 processing pipeline.
 *
 * This is the data model that DRIVES HTTP 3.0. It is independent of the
 * cryptographic substrate (see http3_crypto.h) and carries no OpenSSL
 * dependency, matching the spec's principle (§20) that a C/C++/Java/other
 * client can implement the connector without becoming a SLeeLa runtime.
 * ========================================================================== */
#ifndef SLEELA_HTTP3_PROTOCOL_H
#define SLEELA_HTTP3_PROTOCOL_H

#include "http3_envelope.h"
#include "http3_naming.h"
#include "http3_intactx.h"
#include "http3_pipeline.h"

#endif
