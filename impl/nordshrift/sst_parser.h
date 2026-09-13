// ===========================================================================
// sst_parser.h  --  Spec-conformant .sst parser (NS-SST-0001 Part XIV).
//
// The parser reads a full .sst control sheet, including the first-class
// `network:` and `finance:` declaration blocks. Both blocks carry a closed
// object series so a sheet cannot silently request an unsupported runtime or
// financial component; unknown members are reported with structured NSS-E-NET
// / NSS-E-FIN diagnostics. The block bodies are parsed inline by the same
// indentation-aware parser as every other section (see sst_parser.cpp).
// ===========================================================================
#ifndef NORDSHRIFT_SST_PARSER_H
#define NORDSHRIFT_SST_PARSER_H

#include "sheet_model.h"
#include "sst_lexer.h"
#include "diagnostics.h"

namespace nordshrift {

// Parse a token stream into a Sheet, appending diagnostics to `diags`.
Sheet parseSheet(const std::vector<Token>& toks, const std::string& file,
                 DiagnosticBag& diags);

// ---------------------------------------------------------------------------
// Enum name mappings for the closed network + finance object series. These are
// shared by the parser and any consumer (e.g. the CLI) that needs to render or
// validate the declared component sets.
// ---------------------------------------------------------------------------

bool networkObjectFromName(const std::string& s, NetworkObject& out);
bool networkTransportFromName(const std::string& s, NetworkTransport& out);
bool networkAddressFamilyFromName(const std::string& s, NetworkAddressFamily& out);
const char* networkObjectName(NetworkObject o);
const char* networkTransportName(NetworkTransport t);
const char* networkAddressFamilyName(NetworkAddressFamily a);

bool financeObjectFromName(const std::string& s, FinanceObject& out);
bool financePeriodFromName(const std::string& s, FinancePeriodConvention& out);
bool financeDiscountingFromName(const std::string& s, FinanceDiscounting& out);
const char* financeObjectName(FinanceObject o);
const char* financePeriodName(FinancePeriodConvention p);
const char* financeDiscountingName(FinanceDiscounting d);

} // namespace nordshrift

#endif // NORDSHRIFT_SST_PARSER_H
