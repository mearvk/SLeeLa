// ===========================================================================
// source_resolve.h  --  Resolve a sheet's `source` section into a file set.
//
// Applies source.root + source.glob(s) minus source.exclude over the actual
// filesystem (NS-SST-0001 §V). Emits NSS-E-0030 (root missing) and
// NSS-E-0031 (empty source set). Glob patterns support `*`, `?`, and `**`
// (recursive), evaluated relative to root (SRC-03 forbids absolute globs,
// already reported by the parser).
// ===========================================================================
#ifndef NORDSHRIFT_SOURCE_RESOLVE_H
#define NORDSHRIFT_SOURCE_RESOLVE_H

#include <string>
#include <vector>

#include "sheet_model.h"
#include "diagnostics.h"

namespace nordshrift {

// Returns the sorted, de-duplicated set of source file paths (relative to the
// sheet directory) selected by the source section. `sheetDir` is the directory
// containing the .sst file (paths are resolved relative to it). Diagnostics are
// appended to `diags`.
std::vector<std::string> resolveSources(const Source& src,
                                        const std::string& sheetDir,
                                        DiagnosticBag& diags);

} // namespace nordshrift

#endif // NORDSHRIFT_SOURCE_RESOLVE_H
