// analyze.cpp - Load advantages.xml into memory and analyze it.
//
// Reads the XML store of source advantages and prints a summary: per-source
// counts of assumptions and knowledge strings, totals, and full detail.
//
// Optionally enriches the store from declared remote <knowledgeSource>
// elements via the secure transport layer. Enrichment is OFF unless --enrich
// is passed, and even then only https/file schemes and explicitly allowed
// hosts are permitted.
//
// Usage:
//   analyze [advantages.xml]
//           [--enrich]
//           [--allow-host HOST]...   (repeatable; required to reach a host)
//           [--allow-http]           (also permit plaintext http -- discouraged)
//           [--store DIR]            (content-addressed download store)
#include <iostream>
#include <string>
#include <vector>

#include "advantage_store.hpp"
#include "enrich.hpp"

int main(int argc, char** argv) {
    std::string path = "advantages.xml";
    advantage::EnrichPolicy policy;  // enabled == false by default (safe)
    bool pathSet = false;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--enrich") {
            policy.enabled = true;
        } else if (a == "--allow-host" && i + 1 < argc) {
            policy.allowedHosts.push_back(argv[++i]);
        } else if (a == "--allow-http") {
            policy.allowedSchemes.push_back("http");
        } else if (a == "--store" && i + 1 < argc) {
            policy.storeDir = argv[++i];
        } else if (!a.empty() && a[0] == '-') {
            std::cerr << "unknown option: " << a << "\n";
            return 2;
        } else if (!pathSet) {
            path = a;
            pathSet = true;
        } else {
            std::cerr << "unexpected argument: " << a << "\n";
            return 2;
        }
    }

    advantage::AdvantageStore store;
    std::string error;
    if (!advantage::loadFromFile(path, store, error)) {
        std::cerr << "load failed: " << error << "\n";
        return 1;
    }

    // Optional, safe, opt-in enrichment.
    if (policy.enabled) {
        advantage::EnrichReport rep = advantage::enrich(store, policy);
        std::cout << "Enrichment: considered " << rep.sourcesConsidered
                  << ", attempted " << rep.fetchesAttempted
                  << ", succeeded " << rep.fetchesSucceeded
                  << " (cached " << rep.fetchesFromCache << ")"
                  << ", lines added " << rep.linesAdded << "\n";
        for (const auto& w : rep.warnings) std::cout << "  ! " << w << "\n";
        std::cout << "----------------------------------------\n";
    }

    std::cout << "Loaded advantages.xml (version " << store.version << ")\n";
    std::cout << "Source advantages: " << store.sources.size() << "\n";
    std::cout << "Total assumptions: " << store.assumptionCount() << "\n";
    std::cout << "Total knowledge strings: " << store.knowledgeCount() << "\n";
    std::cout << "----------------------------------------\n";

    for (const auto& s : store.sources) {
        std::cout << s.name << "  [id=" << s.id << ", kind=" << s.kind << "]\n";
        std::cout << "  " << s.description << "\n";
        std::cout << "  assumptions (" << s.assumptions.size() << "):\n";
        for (const auto& a : s.assumptions) {
            std::cout << "    - " << a << "\n";
        }
        std::cout << "  knowledge (" << s.knowledge.size() << "):\n";
        for (const auto& k : s.knowledge) {
            std::cout << "    * " << k << "\n";
        }
        if (!s.knowledgeSources.empty()) {
            std::cout << "  knowledge sources (" << s.knowledgeSources.size() << "):\n";
            for (const auto& ks : s.knowledgeSources) {
                std::cout << "    @ " << ks.url;
                if (!ks.sha256.empty()) std::cout << "  (sha256=" << ks.sha256 << ")";
                std::cout << "\n";
            }
        }
        if (!s.remoteKnowledge.empty()) {
            std::cout << "  remote knowledge (" << s.remoteKnowledge.size()
                      << ", fetched & sanitized):\n";
            for (const auto& k : s.remoteKnowledge) {
                std::cout << "    + " << k << "\n";
            }
        }
        std::cout << "\n";
    }

    return 0;
}
