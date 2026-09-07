// ===========================================================================
// object_compat.h  --  Nordshrift's object compatibility list.
//
// Nordshrift carries every SHEET.sheet object in a compatibility list and can
// convert each into a "relevance" for a chosen target language -- the bridge
// from a catalogued system object to something a target can eventually compile
// and an OS can execute.
//
// A relevance is one of:
//   Direct  -- the object maps to a concrete construct/keyword/type in the
//              target (e.g. Class -> Java `class`; Thread -> C `pthread_t`).
//   Model   -- the object maps only to an abstract role/pattern; there is no
//              single target construct, so it is realized as a modeled shape
//              (e.g. Pipeline -> a staged model; CircuitBreaker -> a pattern).
//   None    -- the object is not (yet) compatible with the target.
// ===========================================================================
#ifndef NORDSHRIFT_OBJECT_COMPAT_H
#define NORDSHRIFT_OBJECT_COMPAT_H

#include <string>
#include <vector>

#include "sheet_model.h"                 // TargetLang
#include "../catalog/sheet_catalog.h"    // catalog::Catalog / Object

namespace nordshrift {

enum class Relevance { Direct, Model, None };

struct ObjectRelevance {
    std::string object;      // the catalogued object name
    std::string role;        // its conduct role
    Relevance   relevance;   // Direct / Model / None for the chosen target
    std::string mapping;     // the concrete construct (Direct) or model shape (Model)
    std::string note;        // the object's insight/gloss, carried through
};

// Is a catalogued object compatible with Nordshrift at all? (Every catalogued
// object is on the compatibility list; this returns true for known names.)
bool isCompatible(const catalog::Catalog& cat, const std::string& object);

// Convert one object into its relevance for `target`.
ObjectRelevance relevanceOf(const catalog::Catalog& cat,
                            const std::string& object, TargetLang target);

// Convert the whole catalog into a relevance list for `target`.
std::vector<ObjectRelevance> relevanceList(const catalog::Catalog& cat,
                                           TargetLang target);

const char* relevanceName(Relevance r);

} // namespace nordshrift

#endif // NORDSHRIFT_OBJECT_COMPAT_H
