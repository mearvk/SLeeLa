// advantage_store.hpp - In-memory model + loader for advantages.xml.
//
// Loads the source advantages (assumptions and strings of knowledge) from
// the XML document into C++ structures so they can be analyzed. The parser
// is a small, dependency-free reader scoped to the advantages.xml schema
// produced by this module; it is not a general-purpose XML engine.
#ifndef ADVANTAGE_STORE_HPP
#define ADVANTAGE_STORE_HPP

#include <string>
#include <vector>

namespace advantage {

// One source of leverage loaded from <sourceAdvantage> in the XML.
struct SourceAdvantage {
    std::string id;
    std::string kind;
    std::string name;
    std::string description;
    std::vector<std::string> assumptions;  // <assumptions><assumption>...
    std::vector<std::string> knowledge;     // <knowledge><string>...
};

// The whole document: a version string and the set of source advantages.
struct AdvantageStore {
    std::string version;
    std::vector<SourceAdvantage> sources;

    // Lookup by id; returns nullptr if absent.
    const SourceAdvantage* find(const std::string& id) const;

    // Total counts across all sources, useful for analysis.
    std::size_t assumptionCount() const;
    std::size_t knowledgeCount() const;
};

// Load from a file path. On failure returns false and fills `error`.
bool loadFromFile(const std::string& path,
                  AdvantageStore& out,
                  std::string& error);

// Load directly from an in-memory XML string.
bool loadFromString(const std::string& xml,
                    AdvantageStore& out,
                    std::string& error);

}  // namespace advantage

#endif  // ADVANTAGE_STORE_HPP
