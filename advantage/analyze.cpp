// analyze.cpp - Load advantages.xml into memory and analyze it.
//
// Reads the XML store of source advantages and prints a summary:
// per-source counts of assumptions and knowledge strings, totals across
// the document, and the full detail of each source. Takes the XML path as
// an optional first argument (defaults to advantages.xml).
#include <iostream>
#include <string>

#include "advantage_store.hpp"

int main(int argc, char** argv) {
    const std::string path = (argc > 1) ? argv[1] : "advantages.xml";

    advantage::AdvantageStore store;
    std::string error;
    if (!advantage::loadFromFile(path, store, error)) {
        std::cerr << "load failed: " << error << "\n";
        return 1;
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
        std::cout << "\n";
    }

    return 0;
}
