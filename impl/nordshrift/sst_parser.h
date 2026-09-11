// ===========================================================================
// sst_parser.h  --  Spec-conformant .sst parser plus network declaration set.
// ===========================================================================
#ifndef NORDSHRIFT_SST_PARSER_H
#define NORDSHRIFT_SST_PARSER_H

#include "sheet_model.h"
#include "sst_lexer.h"
#include "diagnostics.h"
#include <algorithm>

namespace nordshrift {

// The implementation in sst_parser.cpp is renamed to parseSheetBase below.
// The public wrapper consumes the SST network section first, removes that
// declaration block from the token stream, and then delegates the remainder to
// the established NS-SST parser. This keeps the original grammar behavior
// intact while making network declarations first-class SST metadata.
Sheet parseSheet(const std::vector<Token>& toks, const std::string& file,
                 DiagnosticBag& diags);

#define parseSheet parseSheetBase

namespace sst_network_detail {

inline bool objectFromName(const std::string& s, NetworkObject& out) {
    if (s == "Endpoint")     { out = NetworkObject::Endpoint; return true; }
    if (s == "NIC")         { out = NetworkObject::NIC; return true; }
    if (s == "Link")        { out = NetworkObject::Link; return true; }
    if (s == "Packet")      { out = NetworkObject::Packet; return true; }
    if (s == "Queue")       { out = NetworkObject::Queue; return true; }
    if (s == "Switch")      { out = NetworkObject::Switch; return true; }
    if (s == "Router")      { out = NetworkObject::Router; return true; }
    if (s == "Fabric")      { out = NetworkObject::Fabric; return true; }
    if (s == "Listener")    { out = NetworkObject::Listener; return true; }
    if (s == "Connector")   { out = NetworkObject::Connector; return true; }
    if (s == "Gateway")     { out = NetworkObject::Gateway; return true; }
    if (s == "LoadBalancer"){ out = NetworkObject::LoadBalancer; return true; }
    if (s == "Service")     { out = NetworkObject::Service; return true; }
    if (s == "TLS")         { out = NetworkObject::TLS; return true; }
    if (s == "DNS")         { out = NetworkObject::DNS; return true; }
    return false;
}

inline bool transportFromName(const std::string& s, NetworkTransport& out) {
    if (s == "tcp")  { out = NetworkTransport::TCP;  return true; }
    if (s == "udp")  { out = NetworkTransport::UDP;  return true; }
    if (s == "tls")  { out = NetworkTransport::TLS;  return true; }
    if (s == "quic") { out = NetworkTransport::QUIC; return true; }
    return false;
}

inline bool addressFamilyFromName(const std::string& s, NetworkAddressFamily& out) {
    if (s == "ipv4") { out = NetworkAddressFamily::IPv4; return true; }
    if (s == "ipv6") { out = NetworkAddressFamily::IPv6; return true; }
    if (s == "dual") { out = NetworkAddressFamily::Dual; return true; }
    return false;
}

inline std::string scalar(const std::vector<Token>& t, size_t& i) {
    if (i >= t.size()) return {};
    const Token& x = t[i];
    if (x.kind == Tok::String || x.kind == Tok::Integer ||
        x.kind == Tok::Version || x.kind == Tok::Ident) { ++i; return x.text; }
    return {};
}

inline std::vector<std::string> list(const std::vector<Token>& t, size_t& i) {
    std::vector<std::string> out;
    if (i >= t.size() || t[i].kind != Tok::LBracket) return out;
    ++i;
    while (i < t.size() && t[i].kind != Tok::RBracket && t[i].kind != Tok::Eof) {
        if (t[i].kind == Tok::Comma || t[i].kind == Tok::Newline) { ++i; continue; }
        std::string s = scalar(t, i);
        if (!s.empty()) out.push_back(s);
        else ++i;
    }
    if (i < t.size() && t[i].kind == Tok::RBracket) ++i;
    return out;
}

inline bool boolValue(const std::string& s) {
    return s == "true" || s == "on" || s == "yes";
}

inline Sheet parseWithNetwork(const std::vector<Token>& toks,
                              const std::string& file,
                              DiagnosticBag& diags) {
    Sheet sheet;
    std::vector<Token> filtered;
    filtered.reserve(toks.size());

    for (size_t i = 0; i < toks.size();) {
        if (toks[i].kind == Tok::Ident && toks[i].text == "network") {
            size_t start = i;
            size_t p = i + 1;
            if (p < toks.size() && toks[p].kind == Tok::Colon) ++p;
            if (p < toks.size() && toks[p].kind == Tok::Newline) ++p;
            if (p < toks.size() && toks[p].kind == Tok::Indent) {
                sheet.network.present = true;
                sheet.network.line = toks[start].line;
                ++p;
                int depth = 1;
                while (p < toks.size() && depth > 0) {
                    if (toks[p].kind == Tok::Indent) { ++depth; ++p; continue; }
                    if (toks[p].kind == Tok::Dedent) { --depth; ++p; continue; }

                    if (depth == 1 && toks[p].kind == Tok::Ident) {
                        int ln = toks[p].line;
                        std::string key = toks[p].text;
                        ++p;
                        if (p < toks.size() && toks[p].kind == Tok::Colon) ++p;
                        if (p < toks.size() && toks[p].kind == Tok::Newline) ++p;

                        if (key == "objects" || key == "object") {
                            std::vector<std::string> names;
                            if (key == "objects") names = list(toks, p);
                            else { std::string one = scalar(toks, p); if (!one.empty()) names.push_back(one); }
                            for (const auto& name : names) {
                                NetworkObject o;
                                if (!objectFromName(name, o)) {
                                    diags.error("NSS-E-NET-001", file, ln,
                                        "unknown network object '" + name + "'",
                                        "SST-NET-OBJECT", true);
                                } else if (std::find(sheet.network.objects.begin(),
                                                     sheet.network.objects.end(), o) == sheet.network.objects.end()) {
                                    sheet.network.objects.push_back(o);
                                }
                            }
                        } else if (key == "transports" || key == "transport") {
                            std::vector<std::string> names;
                            if (key == "transports") names = list(toks, p);
                            else { std::string one = scalar(toks, p); if (!one.empty()) names.push_back(one); }
                            for (const auto& name : names) {
                                NetworkTransport tr;
                                if (!transportFromName(name, tr)) {
                                    diags.error("NSS-E-NET-002", file, ln,
                                        "unknown network transport '" + name + "'",
                                        "SST-NET-TRANSPORT", true);
                                } else if (std::find(sheet.network.transports.begin(),
                                                     sheet.network.transports.end(), tr) == sheet.network.transports.end()) {
                                    sheet.network.transports.push_back(tr);
                                }
                            }
                        } else if (key == "address-family") {
                            std::string v = scalar(toks, p);
                            if (!addressFamilyFromName(v, sheet.network.addressFamily)) {
                                diags.error("NSS-E-NET-003", file, ln,
                                    "unknown network address-family '" + v + "'",
                                    "SST-NET-ADDRESS-FAMILY", true);
                            }
                        } else if (key == "tls") {
                            sheet.network.tls = boolValue(scalar(toks, p));
                        }
                        while (p < toks.size() && toks[p].kind != Tok::Newline &&
                               toks[p].kind != Tok::Dedent) ++p;
                        if (p < toks.size() && toks[p].kind == Tok::Newline) ++p;
                    } else {
                        ++p;
                    }
                }
                i = p;
                continue;
            }
        }
        filtered.push_back(toks[i++]);
    }

    sheet = parseSheetBase(filtered, file, diags);
    sheet.network.present = sheet.network.present || false;
    return sheet;
}

} // namespace sst_network_detail

#undef parseSheet

inline Sheet parseSheet(const std::vector<Token>& toks, const std::string& file,
                        DiagnosticBag& diags) {
    return sst_network_detail::parseWithNetwork(toks, file, diags);
}

} // namespace nordshrift

#endif // NORDSHRIFT_SST_PARSER_H
