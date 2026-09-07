// ===========================================================================
// sheet_catalog.h  --  Shared parser + model for SHEET.sheet.
//
// SHEET.sheet is the catalog of common system objects. Both the Sleela conduct
// registry and the Nordshrift object-compatibility list load it through this
// one parser so they agree object-for-object.
//
// Format (see the parse-contract block at the top of SHEET.sheet):
//   #key value                     header directive
//   section <name>:  # role: R     opens a category with a conduct role
//   <2-space>Word                  an Object
//   <4-space>children: a, b, c     the object's owned/composed objects
//   <4-space>siblings: x, y        the object's peers
//   <4-space>note:     gloss       a one-line insight
//   <2-space>key: value            a section-level value (depth, invariants)
//   # ... and blank lines          ignored
//
// This header has no dependency beyond the C++ standard library so it can be
// linked into either tool.
// ===========================================================================
#ifndef SLEELA_SHEET_CATALOG_H
#define SLEELA_SHEET_CATALOG_H

#include <map>
#include <string>
#include <vector>

namespace catalog {

// One catalogued object (e.g. System, Pipeline, CircuitBreaker).
struct Object {
    std::string name;
    std::string section;              // owning section name (e.g. "pipings")
    std::string role;                 // conduct role of the section
    std::vector<std::string> children;
    std::vector<std::string> siblings;
    std::string note;                 // the "insight" gloss
};

// A category section with its conduct role and member objects.
struct Section {
    std::string name;                 // e.g. "standard-starts"
    std::string role;                 // e.g. "start"
    std::vector<std::string> objects; // object names in declaration order
    std::map<std::string, std::string> values; // section-level key/value
};

// The whole catalog.
struct Catalog {
    std::map<std::string, std::string> header;   // #key -> value
    std::vector<Section> sections;                // in file order
    std::vector<Object>  objects;                 // in file order
    std::map<std::string, int> objectIndex;       // name -> index in objects

    // System invariants (from the `system` section values).
    long long depth = 0;                          // relevant system depth (3024)
    long long congruentLinearMax = 0;             // 3024
    long long complexityDegreeMax = 0;            // 4

    const Object* find(const std::string& name) const {
        auto it = objectIndex.find(name);
        return it == objectIndex.end() ? nullptr : &objects[(size_t)it->second];
    }
    size_t objectCount() const { return objects.size(); }

    // Are two objects congruent (directly connectable)? True when they are
    // siblings of each other, or share the same section role. Used to answer
    // Sleela's `congruent(A, B)` and Nordshrift's compatibility routing.
    bool congruent(const std::string& a, const std::string& b) const;
};

// Parse a SHEET.sheet from text. `ok` (optional) is set false on a hard error;
// the returned Catalog is always usable (possibly empty).
Catalog parseCatalogText(const std::string& text, bool* ok = nullptr);

// Convenience: read a SHEET.sheet from a file path.
Catalog parseCatalogFile(const std::string& path, bool* ok = nullptr);

} // namespace catalog

#endif // SLEELA_SHEET_CATALOG_H
