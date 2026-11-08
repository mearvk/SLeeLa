// enrich.cpp - Implementation of safe, opt-in enrichment (see enrich.hpp).
#include "enrich.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

#include "transport/acquire.h"
#include "transport/uri.h"

namespace advantage {

namespace {

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower((unsigned char)c));
    return s;
}

bool listed(const std::vector<std::string>& list, const std::string& v) {
    std::string lv = lower(v);
    for (const auto& e : list) if (lower(e) == lv) return true;
    return false;
}

// Strip control characters (except nothing -- newlines are handled by the
// line splitter) and cap length. This neutralizes terminal-escape injection
// and other control-byte tricks in untrusted remote text.
std::string sanitizeLine(const std::string& in, std::size_t maxLen) {
    std::string out;
    out.reserve(std::min(in.size(), maxLen));
    for (char c : in) {
        unsigned char u = static_cast<unsigned char>(c);
        if (u == '\t') { out.push_back(' '); }
        else if (u < 0x20 || u == 0x7f) { continue; }  // drop control bytes
        else { out.push_back(c); }
        if (out.size() >= maxLen) break;
    }
    // Trim trailing whitespace.
    while (!out.empty() && (out.back() == ' ')) out.pop_back();
    return out;
}

// Decide whether a URL is permitted by the policy. Fills `why` on rejection.
bool allowed(const EnrichPolicy& p, const std::string& url, const Uri& u,
             std::string& why) {
    if (!u.valid) { why = "unparseable URL"; return false; }
    if (!listed(p.allowedSchemes, u.scheme)) {
        why = "scheme '" + u.scheme + "' not in allowlist";
        return false;
    }
    // file:// has no host; it is governed solely by the scheme allowlist.
    if (u.scheme != "file") {
        if (u.host.empty()) { why = "missing host"; return false; }
        if (!listed(p.allowedHosts, u.host)) {
            why = "host '" + u.host + "' not in allowlist";
            return false;
        }
    }
    (void)url;
    return true;
}

}  // namespace

EnrichReport enrich(AdvantageStore& store, const EnrichPolicy& policy) {
    EnrichReport rep;

    if (!policy.enabled) {
        rep.warnings.push_back("enrichment disabled (policy.enabled == false); no network access performed");
        return rep;  // ok == true, nothing fetched: the safe default
    }

    AcquisitionStore cas(policy.storeDir);

    for (auto& src : store.sources) {
        if (src.knowledgeSources.empty()) continue;

        for (const auto& ks : src.knowledgeSources) {
            rep.sourcesConsidered++;

            Uri u = parseUri(ks.url);
            std::string why;
            if (!allowed(policy, ks.url, u, why)) {
                std::string w = "skipped [" + src.id + "] " + ks.url + ": " + why;
                rep.warnings.push_back(w);
                if (policy.failFast) { rep.ok = false; return rep; }
                continue;
            }

            // Build transport options from the policy. TLS verification stays
            // ON (transport defaults); we only tighten, never loosen.
            AcquireOptions opts;
            opts.http.maxBodyBytes = policy.maxBytesPerSource;
            opts.expectedSha256 = ks.sha256;  // optional integrity pin

            rep.fetchesAttempted++;
            AcquireResult r = cas.fetch(ks.url, opts);
            if (!r.ok) {
                std::string w = "fetch failed [" + src.id + "] " + ks.url + ": " + r.error;
                rep.warnings.push_back(w);
                if (policy.failFast) { rep.ok = false; return rep; }
                continue;
            }
            rep.fetchesSucceeded++;
            if (r.fromCache) rep.fetchesFromCache++;

            // Read the stored object (never larger than maxBytesPerSource).
            std::ifstream in(r.objectPath, std::ios::binary);
            if (!in) {
                rep.warnings.push_back("could not open stored object for [" + src.id + "] " + ks.url);
                if (policy.failFast) { rep.ok = false; return rep; }
                continue;
            }
            std::ostringstream ss;
            ss << in.rdbuf();
            std::string body = ss.str();

            // Interpret as newline-delimited knowledge strings, sanitized and
            // bounded. Blank lines are ignored.
            std::size_t lines = 0;
            std::size_t pos = 0;
            while (pos < body.size() && lines < policy.maxLinesPerSource) {
                std::size_t nl = body.find('\n', pos);
                std::string raw = (nl == std::string::npos)
                    ? body.substr(pos)
                    : body.substr(pos, nl - pos);
                pos = (nl == std::string::npos) ? body.size() : nl + 1;

                // Drop a trailing CR from CRLF.
                if (!raw.empty() && raw.back() == '\r') raw.pop_back();

                std::string clean = sanitizeLine(raw, policy.maxLineLength);
                if (clean.empty()) continue;

                src.remoteKnowledge.push_back(clean);
                rep.linesAdded++;
                lines++;
            }
        }
    }

    return rep;
}

}  // namespace advantage
