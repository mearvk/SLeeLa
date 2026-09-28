// ===========================================================================
// component_manifest.h  --  Emit a build artifact from the declared network +
// finance component series (and, in 2.0, the semantic subjects).
//
// A `.sst` sheet can declare a `network:` block, a `finance:` block, and one or
// more `subject:` blocks. On its own that metadata is inert; this module turns
// it into a concrete manifest artifact so declaring a component series has a
// real, inspectable build effect for every target of the triplet.
//
// The manifest is target-aware:
//   TargetLang::Java   -> a Java class exposing the series as string arrays.
//   TargetLang::C      -> C arrays of component name strings.
//   TargetLang::Sleela -> canonical Sleela source with the same shape.
// A target-neutral text summary is also available for `check`.
// ===========================================================================
#ifndef NORDSHRIFT_COMPONENT_MANIFEST_H
#define NORDSHRIFT_COMPONENT_MANIFEST_H

#include <string>

#include "sheet_model.h"

namespace nordshrift {

// True when a sheet declares anything worth emitting a manifest for.
bool hasComponentManifest(const Sheet& sheet);

// Render the manifest for `sheet` in the language named by `lang`. `packageRoot`
// (may be empty) is used for the Java package declaration.
std::string emitComponentManifest(const Sheet& sheet, TargetLang lang,
                                  const std::string& packageRoot);

// A short, target-neutral human summary used by `nordshrift check`.
std::string summarizeComponents(const Sheet& sheet);

} // namespace nordshrift

#endif // NORDSHRIFT_COMPONENT_MANIFEST_H
